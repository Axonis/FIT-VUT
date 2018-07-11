/**
 *
 * Project ISAMON - IPK 2017
 * Author: Jozef Urbanovsky, 3BIT
 * Revision: 1.39 - timeout, final
 * Date: 18.11.2017
 *
 * */

#include "isamon.h"

using namespace std;

int main(int argc, char *argv[]) {

    cli_parser(argc, argv);

    hosts();

    interface_info();

    int start_if, end_if;

    // Set interfaces to scan based on arguments
    if (flags.i) {
        start_if = interface_to_scan;
        end_if = interface_to_scan;
    } else {
        start_if = 0;
        end_if = interface.size() - 1;
    }

    for (int intf = start_if; intf <= end_if; intf++) {
        thread send_arp(arp_send, intf);
        thread recieve_arp(arp_recieve);

        send_arp.join();
        recieve_arp.join();
    }
    thread send_icmp(icmp_send);
    thread recieve_icmp(icmp_recieve);

    send_icmp.join();
    recieve_icmp.join();

    // Sort out vector and delete duplicity to make port scanning more efficient
    sort(ip_active.begin(), ip_active.end());
    ip_active.erase(unique(ip_active.begin(), ip_active.end()), ip_active.end());

    for (int i = 0; i < ip_active.size(); i++)
        cout << ip_active[i] << endl;

    for (int intf = start_if; intf <= end_if; intf++) {
        if (args.tcp)
            for (int i = 0; i < ip_active.size(); i++) {
                thread send(tcp_send, ip_active[i], intf);
                thread recieve(tcp_recieve);

                send.join();
                recieve.join();
            }

        if (args.udp)
            for (int i = 0; i < ip_active.size(); i++) {
                thread send(udp_send, ip_active[i], intf);
                thread recieve(udp_recieve, ip_active[i]);

                send.join();
                recieve.join();
            }
    }

    return 0;
}

void udp_send(string ip_addr, int interface_index) {

    int sock;
    int tmp = 1;

    char *full_packet;

    struct iphdr *ip;
    struct udphdr *udp;

    struct L4_packet packet;
    struct sockaddr_in addr;

    uint32_t init_seq = 1234567890;

    // Create RAW UDP socket
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
        print_error(ERR_SOCK);

    // Option for socket to generate IP header
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &tmp, sizeof(tmp)) < 0)
        print_error(ERR_SOCK);

    // Option for socket to be non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    // Loop through all ports requested
    for (int port = start_port; port <= end_port; port++) {

        // Set port and IP address in each iteration
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

        memset(buffer, 0, sizeof(buffer));
        ip = (struct iphdr *) buffer;
        udp = (struct udphdr *) (buffer + sizeof(struct iphdr));

        // Fill out L3 data
        ip->ihl = 5;
        ip->version = 4;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
        ip->id = htons(MAGIC_PORT);
        ip->ttl = 64;
        ip->protocol = IPPROTO_UDP;
        ip->check = 0;
        ip->saddr = inet_addr(interface[interface_index].ip_addr.c_str());
        ip->daddr = inet_addr(ip_addr.c_str());
        ip->check = checksum((unsigned short *) buffer, ip->tot_len);

        // Fill out L4 data
        udp->source = htons(MAGIC_PORT);
        udp->dest = htons(port);
        udp->len = htons(sizeof(struct udphdr));

        packet.ip_src = inet_addr(interface[interface_index].ip_addr.c_str());
        packet.ip_dst = inet_addr(ip_addr.c_str());
        packet.zero = 0;
        packet.protocol = IPPROTO_UDP;
        packet.length = htons(sizeof(struct udphdr));

        // Allocate memory for full packet constructed and fill it
        full_packet = (char *) malloc((int) (sizeof(struct L4_packet) + sizeof(struct udphdr)));
        memset(full_packet, 0, sizeof(struct L4_packet) + sizeof(struct udphdr));
        memcpy(full_packet, (char *) &packet, sizeof(struct L4_packet));
        memcpy(full_packet + sizeof(struct L4_packet), udp, sizeof(struct udphdr));

        udp->check = (checksum((unsigned short *) full_packet,
                               (int) (sizeof(struct L4_packet) + sizeof(struct udphdr))));

        // Retry if EAGAIN is set to errno, as there are no resources at this moment
        udp_retry:
        if ((sendto(sock, buffer, ip->tot_len, 0, (sockaddr *) &addr, sizeof(addr))) < 0) {
            if (errno == EAGAIN || errno == ENOBUFS)
                goto udp_retry;
        }
    }

    close(sock);
}

void udp_recieve(string ip_addr) {
    struct timeval start, end;
    struct icmphdr *icmp;
    struct iphdr *ip, *ip2;
    struct udphdr *udp;
    int sock;

    udp_blocked_port.clear();

    // Create RAW ICMP socket
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        print_error(ERR_SOCK);

    // Set socket to be non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    if (flags.w)
        FD_SET(sock, &mx);


    // Get start time to terminate loop
    gettimeofday(&start, NULL);

    while (true) {
        gettimeofday(&end, NULL);
        // If timeout expires close socket and print out ports that did not respond
        if (end.tv_sec - start.tv_sec > 0) {
            close(sock);
            for (int port = start_port; port <= end_port; port++) {
                if (find(udp_blocked_port.begin(), udp_blocked_port.end(), port) != udp_blocked_port.end()) {
                    continue;
                } else {
                    cout << ip_addr << " UDP " << port << endl;
                }
            }
            return;
        }

        // If flags wait is set, use select
        if (flags.w) {
            if ((select(FD_SETSIZE, &mx, NULL, NULL, &timeout)) == 0) {
                continue;
            }
        }

        if (recv(sock, buffer, sizeof(buffer), 0) > 0) {
            // https://stackoverflow.com/questions/47042355/access-udp-from-icmp-message
            // Pointer arithmetic to use one socket but get data of both UDP and ICMP headers
            ip = (struct iphdr *) buffer;
            icmp = (struct icmphdr *) (buffer + (ip->ihl * 4));
            ip2 = (struct iphdr *) (icmp + 1);
            udp = (struct udphdr *) (((uint8_t *) ip2) + (ip2->ihl * 4));


            // If ICMP 3,3 is returned we can know for sure that port is closed
            if ((icmp->type == ICMP_UNREACH) && (icmp->code == ICMP_UNREACH_PORT)) {
                udp_blocked_port.emplace_back(ntohs(udp->dest));
                if (args.port == ntohs(udp->dest)) {
                    close(sock);
                    return;
                }
            }
        }

        if (flags.p) {
            cout << ip_addr << " UDP " << args.port << endl;
            return;
        }
    }
}

void tcp_send(string ip_addr, int interface_index) {

    int sock;
    int tmp = 1;

    char *full_packet;
    uint8_t buffer[IP_MAXPACKET];

    struct iphdr *ip;
    struct tcphdr *tcp;

    struct L4_packet packet;
    struct sockaddr_in addr;

    uint32_t init_seq = 1234567890;

    // Create RAW TCP socket
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
        print_error(ERR_SOCK);

    // Option for socket to generate IP header
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &tmp, sizeof(tmp)) < 0)
        print_error(ERR_SOCK);

    // Option for socket to be non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    // Loop through all ports requested
    for (int port = start_port; port <= end_port; port++) {

        // Set port and IP address in each iteration
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

        memset(buffer, 0, sizeof(buffer));
        ip = (struct iphdr *) buffer;
        tcp = (struct tcphdr *) (buffer + sizeof(struct iphdr));

        // Fill out L3 data
        ip->ihl = 5;
        ip->version = 4;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        ip->id = htons(MAGIC_PORT);
        ip->ttl = 64;
        ip->protocol = IPPROTO_TCP;
        ip->check = 0;
        ip->saddr = inet_addr(interface[interface_index].ip_addr.c_str());
        ip->daddr = inet_addr(ip_addr.c_str());
        ip->check = checksum((unsigned short *) buffer, ip->tot_len);

        // Fill out L4 data
        tcp->source = htons(MAGIC_PORT);
        tcp->dest = htons(port);
        tcp->seq = htonl(init_seq);
        tcp->ack_seq = 0x0;
        tcp->doff = 5;
        tcp->syn = 1;
        tcp->check = 0;

        packet.ip_src = inet_addr(interface[interface_index].ip_addr.c_str());
        packet.ip_dst = inet_addr(ip_addr.c_str());
        packet.zero = 0;
        packet.protocol = IPPROTO_TCP;
        packet.length = htons(sizeof(struct tcphdr));

        // Allocate memory for full packet constructed and fill it
        full_packet = (char *) malloc((int) (sizeof(struct L4_packet) + sizeof(struct tcphdr)));
        memset(full_packet, 0, sizeof(struct L4_packet) + sizeof(struct tcphdr));
        memcpy(full_packet, (char *) &packet, sizeof(struct L4_packet));
        memcpy(full_packet + sizeof(struct L4_packet), tcp, sizeof(struct tcphdr));

        tcp->check = (checksum((unsigned short *) full_packet,
                               (int) (sizeof(struct L4_packet) + sizeof(struct tcphdr))));


        // Retry if EAGAIN is set to errno, as there are no resources at this moment
        tcp_retry:
        if ((sendto(sock, buffer, ip->tot_len, 0, (sockaddr *) &addr, sizeof(addr))) < 0)
            if (errno == EAGAIN || errno == ENOBUFS)
                goto tcp_retry;

    }

    close(sock);
}

void tcp_recieve() {
    struct timeval start, end;
    int open_port;
    int sock;
    struct iphdr *ip;
    struct tcphdr *tcp;
    char str[INET_ADDRSTRLEN];

    // Create RAW TCP socket
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
        print_error(ERR_SOCK);

    // Set it to be non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    if (flags.w)
        FD_SET(sock, &mx);


    // Get start time to terminate loop
    gettimeofday(&start, NULL);

    while (true) {

        // If timeout expires close socket and end loop
        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec > 0) {
            close(sock);
            return;
        }

        // If flags wait is set, use select
        if (flags.w) {
            if ((select(FD_SETSIZE, &mx, NULL, NULL, &timeout)) == 0) {
                close(sock);
                break;
            }
        }

        memset(buffer, 0, sizeof(buffer));

        if ((recv(sock, buffer, sizeof(buffer), 0)) > 0) {

            // Pointer arithmetic to read data from buffer
            ip = (struct iphdr *) buffer;
            tcp = (struct tcphdr *) (buffer + (ip->ihl * 4));

            open_port = ntohs(tcp->source);
            inet_ntop(AF_INET, &ip->saddr, str, INET_ADDRSTRLEN);
            if (flags.p)
                if (args.port != open_port) {
                    close(sock);
                    return;
                }

            // Check if TCP syn has been received to determine if port is open
            if (tcp->syn == 1 && tcp->rst == 0) {
                // Not to see traffic from other socket and thread from same interface
                if (open_port == MAGIC_PORT)
                    continue;
                cout << str << " TCP " << open_port << endl;
                if (args.port == open_port) {
                    close(sock);
                    return;
                }
            }
        }
    }


}

void icmp_send() {

    int sock;
    struct icmp icmp_packet;
    struct sockaddr_in addr;
    struct ifreq ifr;

    // Create RAW ICMP socket
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        print_error(ERR_SOCK);

    // Set it to be non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    on = 1;
    // Allow socket to send to broadcast if scan requires it
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
        print_error(ERR_SOPT);


    // Bind to interface
    if (flags.i) {
        memset(&ifr, 0, sizeof(ifr));
        snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), interface[interface_to_scan].name.c_str());
        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (void *) &ifr, sizeof(ifr)) < 0)
            print_error(ERR_SOCK);
    }

    addr.sin_family = AF_INET;
    memset(&icmp_packet, 0, sizeof(icmp_packet));


    // Iterate over all hosts in network
    for (int num_of_host = 0; num_of_host < ip_hosts.size(); num_of_host++) {

        memset(&icmp_packet, 0, sizeof(icmp_packet));

        addr.sin_addr.s_addr = inet_addr(ip_hosts[num_of_host].c_str());

        // Set ICMP type to echo
        icmp_packet.icmp_type = ICMP_ECHO;
        icmp_packet.icmp_code = 0;
        icmp_packet.icmp_cksum = checksum((unsigned short *) &icmp_packet, sizeof(icmp_packet));

        icmp_retry:
        if (sendto(sock, (char *) &icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            if (errno == ENOBUFS || errno == EAGAIN)
                goto icmp_retry;
            cout << errno << endl;
            print_error(ERR_SEND);
        }
    }
    close(sock);
}

void icmp_recieve() {
    char str[INET_ADDRSTRLEN];

    int sock;

    struct icmphdr *icmp;
    struct iphdr *ip;

    struct timeval start, end;

    // Create ICMP RAW socket
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        print_error(ERR_SOCK);

    // Set it to not block operations
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    if (flags.w)
        FD_SET(sock, &mx);


    // Start timer to have termination point for loop
    gettimeofday(&start, NULL);

    while (true) {
        // If timer expires end loop and close socket
        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec > 0) {
            close(sock);
            return;
        }

        // If flags wait is set use select
        if (flags.w) {
            if ((select(FD_SETSIZE, &mx, NULL, NULL, &timeout)) == 0) {
                close(sock);
                break;
            }
        }

        if (recv(sock, buffer, sizeof(buffer), 0) > 0) {

            // Pointer arithmetic to get data from buffer
            ip = (struct iphdr *) buffer;
            icmp = (struct icmphdr *) (buffer + (ip->ihl * 4));

            // If ICMP ECHO reply is received host is inserted into vector of active hosts
            if (icmp->type == ICMP_ECHOREPLY) {
                inet_ntop(AF_INET, &ip->saddr, str, INET_ADDRSTRLEN);
                // Find if ICMP received is from correct host
                for (int i = 0; i < ip_hosts.size(); i++)
                    if (!strcmp(str, ip_hosts[i].c_str()))
                        ip_active.emplace_back(str);
            }

        }
    }
}

void arp_send(int interface_index) {

    int sock;

    // Create RAW ARP socket
    if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
        print_error(ERR_SOCK);

    // Set it to non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    int mac_sock;
    struct ifreq ifr;

    mac_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        print_error(ERR_SOCK);

    memcpy(ifr.ifr_name, interface[interface_index].name.c_str(), IF_NAMESIZE);

    // Get MAC address of interface
    if (ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr)) < 0)
        print_error(ERR_IOCTL);

    close(mac_sock);


    // Create ARP packet and fill it with proper information
    struct arp_packet packet;
    memset(packet.ether.ether_dhost, 0xFF, sizeof(packet.ether.ether_dhost)); // destination broadcast
    memcpy(packet.ether.ether_shost, ifr.ifr_hwaddr.sa_data,
           sizeof(packet.ether.ether_dhost)); // source local interface
    packet.ether.ether_type = htons(ETHERTYPE_ARP);
    packet.arp.ar_hrd = htons(ARPHRD_ETHER);
    packet.arp.ar_pro = htons(ETHERTYPE_IP);
    packet.arp.ar_hln = ETHER_ADDR_LEN;
    packet.arp.ar_pln = sizeof(packet.src_ip);
    packet.arp.ar_op = htons(ARPOP_REQUEST);

    memcpy(packet.src_mac, ifr.ifr_hwaddr.sa_data, sizeof(packet.src_mac));
    packet.src_ip = inet_addr(interface[interface_index].ip_addr.c_str());
    memset(packet.dst_mac, 0, sizeof(packet.dst_mac));
    memset(packet.padding, 0, sizeof(packet.padding));

    // Set link level header to broadcast and ARP
    struct sockaddr_ll sa;
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    sa.sll_ifindex = interface[interface_index].if_index;
    sa.sll_hatype = ARPHRD_ETHER;
    sa.sll_pkttype = PACKET_BROADCAST;
    sa.sll_halen = 0;

    // Loop over all IP address from given network
    for (int num_of_host = 0; num_of_host < ip_hosts.size(); num_of_host++) {

        packet.dst_ip = inet_addr(ip_hosts[num_of_host].c_str());

        arp_retry:
        if (sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
            if (errno == EAGAIN)
                goto arp_retry;
            print_error(ERR_SEND);
        }

    }
    close(sock);
}

void arp_recieve() {

    int sock;
    struct timeval start, end;
    struct in_addr ip_addr;
    struct arp_packet packet;
    char ip[INET_ADDRSTRLEN];

    // Create RAW ARP socket
    if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
        print_error(ERR_SOCK);

    // Set it to be non-blocking
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    if (flags.w)
        FD_SET(sock, &mx);


    // Start timeout to break from loop
    gettimeofday(&start, NULL);

    while (true) {
        // If timeout expires break loop and close socket
        gettimeofday(&end, NULL);

        if (end.tv_sec - start.tv_sec > 0) {
            close(sock);
            return;
        }

        // If flags wait is set, use select
        if (flags.w) {
            if ((select(FD_SETSIZE, &mx, NULL, NULL, &timeout)) == 0) {
                close(sock);
                break;
            }
        }

        // If socket is received it means active host responded with its IP and MAC
        // IP address is inserted into active hosts vector
        if (recvfrom(sock, &packet, sizeof(packet), 0, NULL, NULL) != -1) {
            ip_addr.s_addr = packet.src_ip;
            strcpy(ip, inet_ntoa(ip_addr));
            // Insert only valid IP from network in argument
            for (int i = 0; i < ip_hosts.size(); i++)
                if (!strcmp(ip, ip_hosts[i].c_str()))
                    ip_active.emplace_back(ip);
        }

    }
}

int ip_octets_binary(char *ip, int *array) {

    char *tmp;

    tmp = strtok(ip, ".");
    array[0] = atoi(tmp);

    tmp = strtok(NULL, ".");
    array[1] = atoi(tmp);

    tmp = strtok(NULL, ".");
    array[2] = atoi(tmp);

    tmp = strtok(NULL, " ");
    array[3] = atoi(tmp);

    // Shift bits of octets to create binary representation
    return (array[0] << 24 | array[1] << 16 | array[2] << 8 | array[3]);
}

void hosts() {

    int bits = 0;
    int bin_mask[4];
    int first_ip, last_ip;
    char char_mask[INET_ADDRSTRLEN];
    char broadcast_address[INET_ADDRSTRLEN];
    struct in_addr struct_host, struct_mask, broadcast;

    int ip_start[4];
    int ip_end[4];

    // Shift bits of CIDR mask to create dotted representation in octets
    bits = 0xffffffff ^ (1 << 32 - args.mask) - 1;
    bin_mask[0] = (bits & 0xff000000) >> 24;
    bin_mask[1] = (bits & 0xff0000) >> 16;
    bin_mask[2] = (bits & 0xff00) >> 8;
    bin_mask[3] = bits & 0xff;

    // Consolidate octets to array of char
    sprintf(char_mask, "%d.%d.%d.%d", bin_mask[0], bin_mask[1], bin_mask[2], bin_mask[3]);


    // Check if IP is IP of network and create broadcast address
    if (inet_pton(AF_INET, args.ip, &struct_host) == 1 &&
        inet_pton(AF_INET, char_mask, &struct_mask) == 1)
        broadcast.s_addr = struct_host.s_addr | ~struct_mask.s_addr;
    else
        print_error(ERR_NETW);

    if (inet_ntop(AF_INET, &broadcast, broadcast_address, INET_ADDRSTRLEN) == NULL)
        print_error(ERR_NETW);


    // Divide IP into octets
    first_ip = ip_octets_binary(args.ip, &ip_start[0]);
    last_ip = ip_octets_binary(broadcast_address, &ip_end[0]);

    // Check if given network is in correct form with mask
    for (int i = 0; i < 4; i++) {
        if (ip_start[i] == (ip_start[i] & bin_mask[i]))
            continue;
        else
            print_error(ERR_NETW);
    }
    // Iterate over IP addresses to fill out vector of all hosts to be scanned
    for (int curr_ip = first_ip; curr_ip <= last_ip; curr_ip++) {
        sprintf(char_mask, "%d.%d.%d.%d",
                (curr_ip & 0xFF000000) >> 24,
                (curr_ip & 0xFF0000) >> 16,
                (curr_ip & 0xFF00) >> 8,
                (curr_ip & 0xFF)
        );
        ip_hosts.emplace_back(char_mask);
    }
}

void interface_info() {

    struct ifaddrs *ifaddr, *ifa;
    num_of_if = 0;
    interface_to_scan = -1;
    if_info tmp;
    string tmp_ip;

    if (getifaddrs(&ifaddr) == -1)
        print_error(ERR_GAIN);


    // Iterate over all interfaces found by getifaddrs
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        // If interface is IPv4 enabled
        if (ifa->ifa_addr->sa_family == AF_INET) {

            if (strcmp(ifa->ifa_name, "lo") == 0)
                continue;

            tmp.name = ifa->ifa_name;

            int sock;
            struct ifreq ifr;

            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0)
                print_error(ERR_SOCK);

            // Get IP address of interface
            memcpy(ifr.ifr_name, ifa->ifa_name, IF_NAMESIZE);
            if (ioctl(sock, SIOCGIFADDR, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            tmp_ip = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);
            tmp.ip_addr = tmp_ip;

            for (int i = 0; i < ip_hosts.size(); i++) {
                if(tmp_ip.compare(ip_hosts[i]) == 0)
                    ip_active.emplace_back(tmp_ip);
            }

            // Get index of interface
            if (ioctl(sock, SIOCGIFINDEX, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            tmp.if_index = ifr.ifr_ifindex;

            close(sock);

            // If interface argument is set also set auxiliary variable
            if (flags.i) {
                if (strcmp(args.interface, ifa->ifa_name) == 0)
                    interface_to_scan = num_of_if;
            }

            interface.emplace_back(tmp);

            num_of_if++;
        }

}

void cli_parser(int count, char *argument[]) {

    if (count < 2)
        print_error(ERR_HELP);

    int c;

    // Long_options structure with agruments
    while (true) {
        int option_index = 0;
        static struct option long_options[] = {
                {"help",      no_argument,       0, 'h'},
                {"interface", required_argument, 0, 'i'},
                {"network",   required_argument, 0, 'n'},
                {"tcp",       no_argument,       0, 't'},
                {"udp",       no_argument,       0, 'u'},
                {"port",      required_argument, 0, 'p'},
                {"wait",      required_argument, 0, 'w'},
                {0, 0,                           0, 0}
        };

        // Getopt_long with arguments
        c = getopt_long(count, argument, "htui:n:p:w:", long_options, &option_index);
        if (c == -1)
            break;

        // In case of every arguments set its flag, check its validity and insert it into cli_arguments structure
        switch (c) {
            case 'h':
                print_error(ERR_HELP);
                break;

            case 'i':
                if (flags.i)
                    print_error(ERR_DUPL);
                args.interface = optarg;
                flags.i++;
                break;

            case 'n':
                if (flags.n)
                    print_error(ERR_DUPL);
                args.network = optarg;
                flags.n++;
                break;

            case 't':
                if (flags.t)
                    print_error(ERR_DUPL);
                args.tcp = true;
                flags.t++;
                break;

            case 'u':
                if (flags.u)
                    print_error(ERR_DUPL);
                args.udp = true;
                flags.u++;
                break;


            case 'p':
                if (flags.p)
                    print_error(ERR_DUPL);
                args.port = validated_number(optarg, PORT_CHECK);
                flags.p++;
                break;

            case 'w':
                if (flags.w)
                    print_error(ERR_DUPL);
                args.wait = validated_number(optarg, TIME_CHECK);
                flags.w++;
                break;

            case '?':
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    // If unknown arguments is read
    if (optind < count) {
        cout << "Unknown argument: ";
        while (optind < count)
            cout << argument[optind++];
        cout << endl;

        print_error(ERR_HELP);
    }

    network_separator();

    // When IP isn't valid
    if (validated_number(args.ip, IP_CHECK) != 1)
        print_error(ERR_NETW);

    // When port is set without UDP or TCP scap
    if (flags.p && !(flags.u || flags.t))
        print_error(ERR_HELP);

    // If port is not set scan all ports
    if (flags.p == 1) {
        start_port = args.port;
        end_port = args.port;
    } else {
        start_port = 1;
        end_port = 65535;
    }

    // If wait is set use it as timeout
    if (flags.w) {
        timeout.tv_sec = 0;
        timeout.tv_usec = args.wait;

        FD_ZERO(&mx);
    }

}

void network_separator() {
    // Divide by slash
    char *token = strtok(args.network, "/");
    int counter = 0;

    // If mask is not set, it's considered as host
    args.mask = 32;

    while (token != NULL) {
        if (counter == 0)
            args.ip = token;
        else if (counter == 1)
            args.mask = validated_number(token, MASK_CHECK);
        else
            print_error(ERR_NETW);
        counter++;
        token = strtok(NULL, "/");
    }
}

unsigned short checksum(unsigned short *buffer, int size) {
    unsigned long sum = 0;

    while (size > 1) {
        sum += *buffer;
        buffer++;
        size -= 2;
    }

    if (size == 1) {
        sum += *(unsigned char *) buffer;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

int validated_number(char *input, int type) {
    char *ptr = nullptr;
    long tmp;

    if (type != IP_CHECK) {
        // Convert number and check its validity
        tmp = strtol(input, &ptr, 10);
        if (*ptr != 0)
            print_error(ERR_HELP);

        // Port range
        if (type == PORT_CHECK && (tmp > 65535 || tmp < 0))
            print_error(ERR_PORT);

        // Real timeout values
        if (type == TIME_CHECK)
            if (tmp > 5000 || tmp < 0)
                print_error(ERR_WAIT);
            else
                tmp = tmp * 1000;
    } else {
        struct sockaddr_in sa;
        // Returns 1 on success, 0 if input is not valid IP
        int check = inet_pton(AF_INET, input, &(sa.sin_addr));
        tmp = check;
    }

    return (int) tmp;
}

void print_error(int error) {
    switch (error) {
        case ERR_HELP:
            cerr << "Usage:\n"
                    "isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask> \n"
                    "   -h --help -- zobrazí nápovědu \n"
                    "   -i --interface <interface> -- rozhraní na kterém bude nástroj scanovat \n"
                    "   -n --network <net_address/mask> -- ip adresa síťe s maskou definující rozsah pro scanování \n"
                    "   -t --tcp -- použije TCP \n"
                    "   -u --udp -- použije UDP \n"
                    "   -p --port <port> -- specifikace scanovaného portu, pokud není zadaný, scanujte celý rozsah \n"
                    "   -w --wait <ms> -- dodatečná informace pro Váš nástroj jaké je maximální přípustné RTT "
                 << endl;
            break;

        case ERR_PORT:
            cerr << "Invalid port number !" << endl;
            break;

        case ERR_WAIT:
            cerr << "Invalid max RTT !" << endl;
            break;

        case ERR_DUPL:
            cerr << "Duplicite declaration of an argument !" << endl;
            break;

        case ERR_NETW:
            cerr << "Network is not in valid format !" << endl;
            break;

        case ERR_SOPT:
            cerr << "Setsockopt failed !" << endl;
            break;

        case ERR_SOCK:
            cerr << "Socket failed !" << endl;
            break;

        case ERR_GAIN:
            cerr << "Getaddrinfo failed !" << endl;
            break;

        case ERR_SEND:
            cerr << "Sento failed !" << endl;
            break;

        case ERR_IOCTL:
            cerr << "Ioctk failed !" << endl;
            break;

        case ERR_RECV:
            cerr << "Recv failed !" << endl;
            break;

        default:
            cerr << "Unknown fatal error !" << endl;
            break;

    }
    exit(error);
}