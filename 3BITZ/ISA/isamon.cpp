#include "isamon.h"

using namespace std;


int main(int argc, char *argv[]) {

    cli_parser(argc, argv);
    interface_info();
    hosts();

    thread send_icmp(icmp_send);
    thread recieve_icmp(icmp_recieve);

    send_icmp.join();
    recieve_icmp.join();

    /*thread send_arp(arp_send, interface[1]);
    thread recieve_arp(arp_recieve);

    send_arp.join();
    recieve_arp.join();*/

    if (args.tcp)
        for (int i = 0; i < ip_active.size(); ++i) {
            thread send(tcp_send, ip_active[i]);
            thread recieve(tcp_recieve);

            send.join();
            recieve.join();
        }

    if (args.udp)
        for (int i = 0; i < ip_active.size(); ++i) {
            thread send(udp_send, ip_active[i]);
            thread recieve(udp_recieve, ip_active[i]);

            send.join();
            recieve.join();
        }

    return 0;
}

void udp_send(string ip_addr) {

    int sock;
    int tmp = 1;

    char *full_packet;

    struct iphdr *ip;
    struct udphdr *udp;

    struct L4_packet packet;
    struct sockaddr_in addr;

    uint32_t init_seq = 1234567890;

    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
        print_error(ERR_SOCK);

    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &tmp, sizeof(tmp)) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);


    for (int port = start_port; port <= end_port; port++) {

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

        memset(buffer, 0, sizeof(buffer));
        ip = (struct iphdr *) buffer;
        udp = (struct udphdr *) (buffer + sizeof(struct iphdr));

        // L3
        ip->ihl = 5;
        ip->version = 4;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
        ip->id = htons(MAGIC_PORT);
        ip->ttl = 64;
        ip->protocol = IPPROTO_UDP;
        ip->check = 0;
        ip->saddr = inet_addr(interface[interface_to_scan].ip_addr.c_str());
        ip->daddr = inet_addr(ip_addr.c_str());
        ip->check = checksum((unsigned short *) buffer, ip->tot_len);

        // L4
        udp->source = htons(MAGIC_PORT);
        udp->dest = htons(port);
        udp->len = htons(sizeof(struct udphdr));

        packet.ip_src = inet_addr(interface[interface_to_scan].ip_addr.c_str());
        packet.ip_dst = inet_addr(ip_addr.c_str());
        packet.zero = 0;
        packet.protocol = IPPROTO_UDP;
        packet.length = htons(sizeof(struct udphdr));

        full_packet = (char *) malloc((int) (sizeof(struct L4_packet) + sizeof(struct udphdr)));

        memset(full_packet, 0, sizeof(struct L4_packet) + sizeof(struct udphdr));

        memcpy(full_packet, (char *) &packet, sizeof(struct L4_packet));

        memcpy(full_packet + sizeof(struct L4_packet), udp, sizeof(struct udphdr));

        udp->check = (checksum((unsigned short *) full_packet,
                               (int) (sizeof(struct L4_packet) + sizeof(struct udphdr))));

        tryagain:
        if ((sendto(sock, buffer, ip->tot_len, 0, (sockaddr *) &addr, sizeof(addr))) < 0) {
            if(errno == EAGAIN)
            goto tryagain;
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


    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    gettimeofday(&start, NULL);

    while (true) {
        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec > 1) {
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
        if (recv(sock, buffer, sizeof(buffer), 0) > 0) {
            // https://stackoverflow.com/questions/47042355/access-udp-from-icmp-message
            ip = (struct iphdr *) buffer;
            icmp = (struct icmphdr *) (buffer + (ip->ihl * 4));
            ip2 = (struct iphdr *) (icmp + 1);
            udp = (struct udphdr *) (((uint8_t *) ip2) + (ip2->ihl * 4));



            if ((icmp->type == ICMP_UNREACH) && (icmp->code == ICMP_UNREACH_PORT)) {
                udp_blocked_port.emplace_back(ntohs(udp->dest));
                //cout << "From IP: " << ip_addr  << " Port " << ntohs(udp->dest)  << " ICMP TYPE: " << (int)icmp->type << endl;
            }
        }
    }
}

void tcp_send(string ip_addr) {

    int sock;
    int tmp = 1;

    char *full_packet;
    char buffer[4096];

    struct iphdr *ip;
    struct tcphdr *tcp;

    struct L4_packet packet;
    struct sockaddr_in addr;

    uint32_t init_seq = 1234567890;

    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
        print_error(ERR_SOCK);

    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &tmp, sizeof(tmp)) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);


    for (int port = start_port; port <= end_port; port++) {

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

        memset(buffer, 0, sizeof(buffer));
        ip = (struct iphdr *) buffer;
        tcp = (struct tcphdr *) (buffer + sizeof(struct iphdr));

        // L3
        ip->ihl = 5;
        ip->version = 4;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        ip->id = htons(MAGIC_PORT);
        ip->ttl = 64;
        ip->protocol = IPPROTO_TCP;
        ip->check = 0;
        ip->saddr = inet_addr(interface[interface_to_scan].ip_addr.c_str());
        ip->daddr = inet_addr(ip_addr.c_str());
        ip->check = checksum((unsigned short *) buffer, ip->tot_len);

        // L4
        tcp->source = htons(MAGIC_PORT);
        tcp->dest = htons(port);
        tcp->seq = htonl(init_seq);
        tcp->ack_seq = 0x0;
        tcp->doff = 5;
        tcp->syn = 1;
        tcp->check = 0;

        packet.ip_src = inet_addr(interface[interface_to_scan].ip_addr.c_str());
        packet.ip_dst = inet_addr(ip_addr.c_str());
        packet.zero = 0;
        packet.protocol = IPPROTO_TCP;
        packet.length = htons(sizeof(struct tcphdr));

        full_packet = (char *) malloc((int) (sizeof(struct L4_packet) + sizeof(struct tcphdr)));
        memset(full_packet, 0, sizeof(struct L4_packet) + sizeof(struct tcphdr));

        memcpy(full_packet, (char *) &packet, sizeof(struct L4_packet));

        memcpy(full_packet + sizeof(struct L4_packet), tcp, sizeof(struct tcphdr));

        tcp->check = (checksum((unsigned short *) full_packet,
                               (int) (sizeof(struct L4_packet) + sizeof(struct tcphdr))));

        tryagain:
        if ((sendto(sock, buffer, ip->tot_len, 0, (sockaddr *) &addr, sizeof(addr))) < 0) {
            if (errno == EAGAIN)
                // resources are blocked, need to retry
                goto tryagain;
            print_error(ERR_SEND);

        }
    }

    close(sock);
}

void tcp_recieve() {
    struct timeval start, end;
    int open_port;
    int sock;
    struct iphdr *ip;
    struct tcphdr *tcp;
    char buf[512];
    char str[INET_ADDRSTRLEN];

    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);


    gettimeofday(&start, NULL);

    while (true) {

        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec >= 1) {
            close(sock);
            return;
        }


        memset(buffer, 0, sizeof(buffer));

        if ((recv(sock, buffer, sizeof(buffer), 0)) > 0) {

            ip = (struct iphdr *) buffer;
            tcp = (struct tcphdr *) (buffer + (ip->ihl * 4));

            if (tcp->syn == 1) {
                open_port = ntohs(tcp->source);
                inet_ntop(AF_INET, &ip->saddr, str, INET_ADDRSTRLEN);
                if (open_port == MAGIC_PORT)
                    continue;
                cout << str << " TCP " << open_port << endl;
            }
        }
    }


}

void icmp_send() {

    int sock;
    struct icmp icmp_packet;
    struct sockaddr_in addr;
    struct ifreq ifr;

    struct icmphdr *icmp;
    struct iphdr *ip;

    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);


    // Bind to interface
    if (interface_to_scan >= 0) {
        memset(&ifr, 0, sizeof(ifr));
        snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), interface[interface_to_scan].name.c_str());
        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (void *) &ifr, sizeof(ifr)) < 0)
            print_error(ERR_SOCK);
    }

    addr.sin_family = AF_INET;
    memset(&icmp_packet, 0, sizeof(icmp_packet));


    for (int num_of_host = 0; num_of_host < ip_hosts.size(); num_of_host++) {

        memset(&icmp_packet, 0, sizeof(icmp_packet));

        addr.sin_addr.s_addr = inet_addr(ip_hosts[num_of_host].c_str());

        icmp_packet.icmp_type = ICMP_ECHO;
        icmp_packet.icmp_code = 0;
        icmp_packet.icmp_cksum = checksum((unsigned short *) &icmp_packet, sizeof(icmp_packet));

        if (sendto(sock, (char *) &icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr *) &addr, sizeof(addr)) < 1) {
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

    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    gettimeofday(&start, NULL);

    while (true) {
        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec > 1) {
            close(sock);
            return;
        }
        if (recv(sock, buffer, sizeof(buffer), 0) > 0) {

            ip = (struct iphdr *) buffer;
            icmp = (struct icmphdr *) (buffer + (ip->ihl * 4));

            if (icmp->type == ICMP_ECHOREPLY) {
                cout << inet_ntop(AF_INET, &ip->saddr, str, INET_ADDRSTRLEN) << endl;
                ip_active.emplace_back(str);
            }
        }
    }
}

void arp_send(if_info intf) {

    int sock;

    if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);

    struct arp_packet packet;
    memset(packet.ether.ether_dhost, 0xFF, sizeof(packet.ether.ether_dhost)); // destination broadcast
    memcpy(packet.ether.ether_shost, intf.mac, sizeof(packet.ether.ether_dhost)); // source local interface
    packet.ether.ether_type = htons(ETHERTYPE_ARP);
    packet.arp.ar_hrd = htons(ARPHRD_ETHER);
    packet.arp.ar_pro = htons(ETHERTYPE_IP);
    packet.arp.ar_hln = ETHER_ADDR_LEN;
    packet.arp.ar_pln = sizeof(packet.src_ip);
    packet.arp.ar_op = htons(ARPOP_REQUEST);

    memcpy(packet.src_mac, intf.mac, sizeof(packet.src_mac));
    packet.src_ip = htonl(intf.ip_bin);
    memset(packet.dst_mac, 0, sizeof(packet.dst_mac));
    memset(packet.padding, 0, sizeof(packet.padding));

    struct sockaddr_ll sa;
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    sa.sll_ifindex = intf.if_index;
    sa.sll_hatype = ARPHRD_ETHER;
    sa.sll_pkttype = PACKET_BROADCAST;
    sa.sll_halen = 0;


    for (int num_of_host = 0; num_of_host < ip_hosts.size(); num_of_host++) {

        packet.dst_ip = inet_addr(ip_hosts[num_of_host].c_str());
        if (sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *) &sa, sizeof(sa)) < 0)
            print_error(ERR_SEND);

    }
    close(sock);
}

void arp_recieve() {

    int sock;
    struct timeval start, end;
    struct in_addr ip_addr;
    struct arp_packet packet;
    char ip[INET_ADDRSTRLEN];

    if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0)
        print_error(ERR_SOCK);

    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0)
        print_error(ERR_SOCK);


    gettimeofday(&start, NULL);

    while (true) {
        gettimeofday(&end, NULL);
        if (end.tv_sec - start.tv_sec > 1) {
            close(sock);
            return;
        }

        if (recvfrom(sock, &packet, sizeof(packet), 0, NULL, NULL) != -1) {
            ip_addr.s_addr = packet.src_ip;
            strcpy(ip, inet_ntoa(ip_addr));
            ip_active.emplace_back(ip);
            cout << ip << endl;
        }

    }
}

void hosts() {
    // TODO 31 and 32 MASK
    long bits = 0;
    int bin_mask[4];
    int first_ip, last_ip;
    char char_mask[INET_ADDRSTRLEN];
    char broadcast_address[INET_ADDRSTRLEN];
    struct in_addr struct_host, struct_mask, broadcast;

    int ip_start[4];
    int ip_end[4];

    bits = 0xffffffff ^ (1 << 32 - args.mask) - 1;


    bin_mask[0] = (bits & 0x0000000000ff000000) >> 24;
    bin_mask[1] = (bits & 0x0000000000ff0000) >> 16;
    bin_mask[2] = (bits & 0x0000000000ff00) >> 8;
    bin_mask[3] = bits & 0xff;

    sprintf(char_mask, "%d.%d.%d.%d", bin_mask[0], bin_mask[1], bin_mask[2], bin_mask[3]);


    /* Check if IP is IP of network */
    if (inet_pton(AF_INET, args.ip, &struct_host) == 1 &&
        inet_pton(AF_INET, char_mask, &struct_mask) == 1)
        broadcast.s_addr = struct_host.s_addr | ~struct_mask.s_addr;
    else
        print_error(ERR_NETW);

    if (inet_ntop(AF_INET, &broadcast, broadcast_address, INET_ADDRSTRLEN) == NULL)
        print_error(ERR_NETW);


    char *tmp;

    tmp = strtok(args.ip, ".");
    ip_start[0] = atoi(tmp);

    tmp = strtok(NULL, ".");
    ip_start[1] = atoi(tmp);

    tmp = strtok(NULL, ".");
    ip_start[2] = atoi(tmp);

    tmp = strtok(NULL, " ");
    ip_start[3] = atoi(tmp);

    first_ip = (ip_start[0] << 24 | ip_start[1] << 16 | ip_start[2] << 8 | ip_start[3]);

    tmp = strtok(broadcast_address, ".");
    ip_end[0] = atoi(tmp);

    tmp = strtok(NULL, ".");
    ip_end[1] = atoi(tmp);

    tmp = strtok(NULL, ".");
    ip_end[2] = atoi(tmp);

    tmp = strtok(NULL, " ");
    ip_end[3] = atoi(tmp);

    last_ip = (ip_end[0] << 24 | ip_end[1] << 16 | ip_end[2] << 8 | ip_end[3]);

    for (int i = 0; i < 4; i++) {
        if (ip_start[i] == (ip_start[i] & bin_mask[i]))
            continue;
        else
            print_error(ERR_NETW);
    }

    int curr_ip;

    for (curr_ip = first_ip + 1; curr_ip < last_ip; curr_ip++) {
        sprintf(char_mask, "%d.%d.%d.%d",
                (curr_ip & 0xFF000000) >> 24,
                (curr_ip & 0x00FF0000) >> 16,
                (curr_ip & 0x0000FF00) >> 8,
                (curr_ip & 0x000000FF)
        );
        ip_hosts.emplace_back(char_mask);
    }

}

void interface_info() {

    struct ifaddrs *ifaddr, *ifa;
    num_of_if = 0;
    interface_to_scan = -1;

    if (getifaddrs(&ifaddr) == -1) {
        print_error(ERR_GAIN);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        if (ifa->ifa_addr->sa_family == AF_INET) {

            interface[num_of_if].name = ifa->ifa_name;

            int sock;
            struct ifreq ifr;

            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0)
                print_error(ERR_SOCK);

            // get IP address of interface
            memcpy(ifr.ifr_name, ifa->ifa_name, IF_NAMESIZE);
            if (ioctl(sock, SIOCGIFADDR, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            interface[num_of_if].ip_bin = ntohl(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr);
            interface[num_of_if].ip_addr = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);

            // get netmask of interface
            if (ioctl(sock, SIOCGIFNETMASK, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            interface[num_of_if].mask = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);

            // get index of interface
            if (ioctl(sock, SIOCGIFINDEX, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            interface[num_of_if].if_index = ifr.ifr_ifindex;

            // get MAC address of interface
            if (ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            strcpy(interface[num_of_if].mac, ifr.ifr_hwaddr.sa_data);

            close(sock);

            if (flags.i) {
                if (args.interface == interface[num_of_if].name)
                    interface_to_scan = num_of_if;
            }
            num_of_if++;
        }

    if (!flags.i)
        for (int i = 0; i < num_of_if; i++)
            if (interface[i].name.compare("lo"))
                interface_to_scan = i;

}

void cli_parser(int count, char *argument[]) {

    if (count < 2)
        print_error(ERR_HELP);

    int c;

    while (1) {
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

        c = getopt_long(count, argument, "htui:n:p:w:", long_options, &option_index);
        if (c == -1)
            break;

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

    if (optind < count) {
        printf("Unknown arg: ");
        while (optind < count)
            cout << argument[optind++];
        cout << endl;

        print_error(ERR_HELP);
    }

    network_separator();

    if (validated_number(args.ip, IP_CHECK) != 1)
        print_error(ERR_NETW);

    if (flags.p && !(flags.u || flags.t))
        print_error(ERR_HELP);

    if (flags.p == 1) {
        start_port = args.port;
        end_port = args.port;
    } else {
        start_port = 1;
        end_port = 50;
    }

}

void network_separator() {
    char *token = strtok(args.network, "/");
    int counter = 0;
    /* If mask is not set, it's considered as host */
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

unsigned short checksum(unsigned short *buff, int size) {
    unsigned long sum = 0;

    while (size > 1) {
        sum += *buff;
        buff++;
        size -= 2;
    }

    if (size == 1) {
        sum += *(unsigned char *) buff;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

int validated_number(char *input, int type) {
    char *ptr = nullptr;
    long tmp;

    if (type != IP_CHECK) {
        tmp = strtol(input, &ptr, 10);
        if (*ptr != 0)
            print_error(ERR_HELP);

        if (type == PORT_CHECK && (tmp > 65535 || tmp < 0))
            print_error(ERR_PORT);

        if (type == TIME_CHECK)
            if (tmp > 5000 || tmp < 0)
                print_error(ERR_WAIT);
            else
                tmp = tmp * 1000;
    } else {
        struct sockaddr_in sa;
        // returns 1 on success, 0 if input is not valid IP
        int check = inet_pton(AF_INET, input, &(sa.sin_addr));
        tmp = check;
    }

    return (int) tmp;
}

void print_error(int error) {
    switch (error) {
        case ERR_HELP:
            fprintf(stderr, "Usage:\n"
                    "isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask> \n"
                    "   -h --help -- zobrazí nápovědu \n"
                    "   -i --interface <interface> -- rozhraní na kterém bude nástroj scanovat \n"
                    "   -n --network <net_address/mask> -- ip adresa síťe s maskou definující rozsah pro scanování \n"
                    "   -t --tcp -- použije TCP \n"
                    "   -u --udp -- použije UDP \n"
                    "   -p --port <port> -- specifikace scanovaného portu, pokud není zadaný, scanujte celý rozsah \n"
                    "   -w --wait <ms> -- dodatečná informace pro Váš nástroj jaké je maximální přípustné RTT ");
            break;

        case ERR_PORT:
            fprintf(stderr, "Invalid port number !\n");
            break;

        case ERR_WAIT:
            fprintf(stderr, "Invalid max RTT !\n");
            break;

        case ERR_DUPL:
            fprintf(stderr, "Duplicite declaration of an argument !\n");
            break;

        case ERR_NETW:
            fprintf(stderr, "Network is not in valid format !\n");
            break;

        case ERR_SOPT:
            fprintf(stderr, "Setsockopt failed !\n");
            break;

        case ERR_SOCK:
            fprintf(stderr, "Socket failed !\n");
            break;

        case ERR_GAIN:
            fprintf(stderr, "Getaddrinfo failed !\n");
            break;

        case ERR_SEND:
            fprintf(stderr, "Sento failed !\n");
            break;

        case ERR_IOCTL:
            fprintf(stderr, "Ioctl failed !\n");
            break;

        case ERR_RECV:
            fprintf(stderr, "Recv failed !\n");
            break;

        default:
            fprintf(stderr, "Unknown error");
            break;

    }
    exit(error);
}