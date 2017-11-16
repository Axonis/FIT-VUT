#include "isamon.h"
#include <netinet/tcp.h>
#include <thread>

using namespace std;

void tcp_recieve(string addr);

void udp(string ip, char* port);


int main(int argc, char *argv[]) {

    cli_parser(argc, argv);
    interface_info();


    /*thread send (tcp, args.ip);
    thread recieve (tcp_recieve, args.ip);

    send.join();
    recieve.join();*/

    //udp(args.ip);

    hosts();


    args.wait = 50000;

    /*for(int i=0; i<ip_hosts.size(); ++i) {
        ping(ip_hosts[i]);
        arp(ip_hosts[i], interface[1]);
    }

    for(int i=0; i<num_of_if; i++)
        cout << interface[i].mask << endl;*/


    return 0;
}


void udp(string ip, char *port) {

    int ttl = 64;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    set1 = setsockopt(sock, SOL_IP, IP_RECVERR, (char *) &on, sizeof(on));
    set2 = setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    if (getaddrinfo(ip.c_str(), port, &hints, &res) != 0)
        print_error(ERR_GAIN);

    if (sock < 0)
        print_error(ERR_SOCK);

    if (set1 != 0 || set2 != 0)
        print_error(ERR_SOPT);


    if (sendto(sock, 0, 0, 0, res->ai_addr, res->ai_addrlen) == 0) {
        /* Structure setup for select */
        FD_ZERO(&fds);
        FD_SET(sock, &fds);

        iov.iov_base = &icmph;              /* ICMP Header */
        iov.iov_len = sizeof(icmph);
        msg.msg_name = &storage;            /* Address target */
        msg.msg_namelen = sizeof(storage);
        msg.msg_iov = &iov;                 /* ICMP header to msg */
        msg.msg_iovlen = 1;                 /* Number of headers */
        msg.msg_flags = 0;                  /* No flags */
        msg.msg_control = buffer;           /* Buffer for control msg */
        msg.msg_controllen = sizeof(buffer);

        /* Set timeout to 0.5 second */
        timeout.tv_usec = args.wait;

        rcv = recvmsg(sock, &msg, MSG_ERRQUEUE);

        if (rcv >= 0) {
            /* Loop through all control headers */
            /* Loop through all control headers */
            for (cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
                /* Filling sock_err from header */
                sock_err = (struct sock_extended_err *) CMSG_DATA(cmsg);

                if (cmsg->cmsg_type == IP_RECVERR && cmsg->cmsg_level == SOL_IP) {

                    if (sock_err && sock_err->ee_origin == SO_EE_ORIGIN_ICMP) {
                        /* If destination is unreachable */
                        if ((sock_err->ee_type == ICMP_DEST_UNREACH)) {
                            printf("ICMP_DEST_UNREACH\n\n");
                        }
                    }
                }


            }


            close(sock);
            return;

            /* Select is not ready to read - timeout */
        }
    }

    else {
        print_error(ERR_SEND);
    }
}

struct pseudoTCPPacket {
    uint32_t srcAddr;
    uint32_t dstAddr;
    uint8_t zero;
    uint8_t protocol;
    uint16_t TCP_len;
};

void tcp(string address) {

    int sock;
    int one = 1;

    int first;        // Prvy scannovany port
    int last;        // Posledny scannovany port

    int open_port;    // Otvoreny port (pre vypis)

    char *data;
    char *pseudo_packet;
    char buffer[4096];

    struct iphdr *ip;
    struct tcphdr *tcp;

    struct pseudoTCPPacket pTCPPacket;
    struct sockaddr_in addr;

    uint32_t init_seq = 1138083240;

    /* Vytvorenie socketu */
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        print_error(ERR_SOCK);
    }

    /* Nastavenie socketu */
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *) &one, sizeof(one)) < 0) {
        print_error(ERR_SOCK);
    }

    /* Nastavenie socketu na neblokujuci */
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0) {
        print_error(ERR_SOCK);
    }

    /* Kontrola, ci bol zadany parameter --port */
    if (flags.p == 1) {
        first = args.port;
        last = args.port;
    } else {
        first = 1; //first_port;
        last = 65535; //last_port;
    }


    /* Rozposlanie tcp syn sprav vsetkym pozadovanym portom */
    for (int port = first; port <= last; port++) {

        //thread recieve (tcp_recieve, address);

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(address.c_str());

        memset(buffer, 0, sizeof(buffer));
        ip = (struct iphdr *) buffer;
        tcp = (struct tcphdr *) (buffer + sizeof(struct iphdr));
        data = (char *) (buffer + sizeof(struct iphdr) + sizeof(struct tcphdr));
        strcpy(data, "datastring");

        /* Struktura IP */
        ip->ihl = 5;
        ip->version = 4;
        ip->tos = 0;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
        ip->id = htons(54321);
        ip->frag_off = 0x00;
        ip->ttl = 0xFF;
        ip->protocol = IPPROTO_TCP;
        ip->check = 0; // Done by kernel
        ip->saddr = inet_addr(interface[1].ip_addr.c_str());
        ip->daddr = inet_addr(address.c_str());
        ip->check = checksum((unsigned short *) buffer, ip->tot_len);

        /* Struktura TCP */
        tcp->source = htons(30001);
        tcp->dest = htons(port);
        tcp->seq = htonl(init_seq);
        tcp->ack_seq = 0x0;
        tcp->doff = 5;
        tcp->res1 = 0;
        tcp->urg = 0;
        tcp->ack = 0;
        tcp->psh = 0;
        tcp->rst = 0;
        tcp->syn = 1;    // Nastavenie iba priznaku SYN, aby nemusel prebiehat cely 3-way handshake. Budem cakat na SYN+ACK
        tcp->fin = 0;
        // tcp->th_flags = TH_SYN;
        // tcp->th_seq = htonl(23456);
        // tcp->window = htons(155);  //0xFFFF
        tcp->check = 0;
        tcp->urg_ptr = 0;

        pTCPPacket.srcAddr = inet_addr(interface[1].ip_addr.c_str());
        pTCPPacket.dstAddr = inet_addr(address.c_str());
        pTCPPacket.zero = 0;
        pTCPPacket.protocol = IPPROTO_TCP;
        pTCPPacket.TCP_len = htons(sizeof(struct tcphdr) + strlen(data));

        pseudo_packet = (char *) malloc((int) (sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr) + strlen(data)));
        memset(pseudo_packet, 0, sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr) + strlen(data));

        memcpy(pseudo_packet, (char *) &pTCPPacket, sizeof(struct pseudoTCPPacket));

        memcpy(pseudo_packet + sizeof(struct pseudoTCPPacket), tcp, sizeof(struct tcphdr) + strlen(data));

        tcp->check = (checksum((unsigned short *) pseudo_packet,
                               (int) (sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr) + strlen(data))));


        if ((sendto(sock, buffer, ip->tot_len, 0, (sockaddr *) &addr, sizeof(addr))) < 0) {
            print_error(ERR_SEND);
        }
        usleep(50);
    }

    int i = 0;

    /*while (1){
        i++;
        char rbuffer[4096];
        memset (rbuffer, 0, sizeof(rbuffer));
        if ((recv(sock, rbuffer, sizeof(rbuffer), 0)) > 0) {
            ip = (struct iphdr *) rbuffer;
            tcp = (struct tcphdr *) (rbuffer + (ip->ihl * 4));
            if (tcp->syn == 1) {
                open_port = ntohs(tcp->source);
                cout << address << " TCP " << open_port << endl;
            }
        }
        if (i == 100000)
            return;
    }*/


    close(sock);
}

void tcp_recieve(string addr) {
    struct timeval t1, t2;
    int open_port;
    int sock;
    struct iphdr *ip;
    struct tcphdr *tcp;

    /* Vytvorenie socketu */
    if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        print_error(ERR_SOCK);
    }

    /* Nastavenie socketu na neblokujuci */
    if ((fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK)) < 0) {
        print_error(ERR_SOCK);
    }

    gettimeofday(&t1, NULL);
    cout << "T1 SEC:" << t1.tv_sec << endl;
    while (1) {
        gettimeofday(&t2, NULL);
        if (t2.tv_sec - t1.tv_sec >= 4) {
            close(sock);
            return;
        }
        char rbuffer[4096];
        memset(rbuffer, 0, sizeof(rbuffer));
        if ((recv(sock, rbuffer, sizeof(rbuffer), 0)) > 0) {
            ip = (struct iphdr *) rbuffer;
            tcp = (struct tcphdr *) (rbuffer + (ip->ihl * 4));
            if (tcp->syn == 1) {
                open_port = ntohs(tcp->source);
                cout << addr << " TCP " << open_port << endl;
            }
        }
    }


}

void ping(string host_ip) {

    int sock;
    struct icmp hdr;
    struct sockaddr_in addr;
    struct ifreq ifr;

    ssize_t n;
    int i = 0;

    char buf[512];
    struct icmphdr *icmphdrptr;
    struct iphdr *iphdrptr;


    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host_ip.c_str());

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
        print_error(ERR_SOCK);


    if (args.interface != NULL) {
        memset(&ifr, 0, sizeof(ifr));
        snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), interface[1].name.c_str());
        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (void *) &ifr, sizeof(ifr)) < 0) {
            print_error(ERR_SOCK);
        }
    }

    memset(&hdr, 0, sizeof(hdr));


    hdr.icmp_type = ICMP_ECHO;
    hdr.icmp_code = 0;

    hdr.icmp_cksum = checksum((unsigned short *) &hdr, sizeof(hdr));

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    /* Set timeout to 0.5 second */
    timeout.tv_usec = args.wait;

    /* Wait for answer, if it comes in 0.5 second receive it */


    n = sendto(sock, (char *) &hdr, sizeof(hdr), 0, (struct sockaddr *) &addr, sizeof(addr));
    if (n < 1)
        print_error(ERR_SEND);


    int rdy = select(sock + 1, &fds, NULL, NULL, &timeout);

    if (rdy == 1) {

        n = recv(sock, buf, sizeof(buf), 0);
        if (n < 1)
            print_error(ERR_SOCK);

        iphdrptr = (struct iphdr *) buf;
        icmphdrptr = (struct icmphdr *) (buf + (iphdrptr->ihl * 4));

        if (icmphdrptr->type == ICMP_ECHOREPLY) {
            printf("%s\n", host_ip.c_str());
            ip_active.emplace_back(host_ip);
        } else;//printf("Received ICMP %d from %s\n", icmphdrptr->type, host_ip.c_str());

    }

    close(sock);

}

void arp(string host_ip, if_info intf) {


    int arp_fd;

    // Socket to send ARP packet
    arp_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (arp_fd == -1)
        print_error(ERR_SOCK);

    struct arp_packet pkt;
    memset(pkt.ether.ether_dhost, 0xFF, sizeof(pkt.ether.ether_dhost)); // destination broadcast
    memcpy(pkt.ether.ether_shost, intf.mac, sizeof(pkt.ether.ether_dhost)); // source local interface
    pkt.ether.ether_type = htons(ETHERTYPE_ARP);

    pkt.arp.ar_hrd = htons(ARPHRD_ETHER);
    pkt.arp.ar_pro = htons(ETHERTYPE_IP);
    pkt.arp.ar_hln = ETHER_ADDR_LEN;
    pkt.arp.ar_pln = sizeof(pkt.sender_ip);
    pkt.arp.ar_op = htons(ARPOP_REQUEST);

    memcpy(pkt.sender_mac, intf.mac, sizeof(pkt.sender_mac));
    pkt.sender_ip = htonl(inet_addr(intf.ip_addr.c_str()));
    memset(pkt.target_mac, 0, sizeof(pkt.target_mac));
    pkt.target_ip = inet_addr(host_ip.c_str());

    memset(pkt.padding, 0, sizeof(pkt.padding));


    struct sockaddr_ll sa;
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    sa.sll_ifindex = intf.if_index;
    sa.sll_hatype = ARPHRD_ETHER;
    sa.sll_pkttype = PACKET_BROADCAST;
    sa.sll_halen = 0;
    // sa.sll_addr not set


    if (sendto(arp_fd, &pkt, sizeof(pkt), 0, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        print_error(ERR_SEND);

    if (recvfrom(arp_fd, &pkt, sizeof(pkt), 0, NULL, NULL) == -1)
        print_error(ERR_SEND);
    else {
        struct in_addr ip_addr;
        ip_addr.s_addr = pkt.sender_ip;
        printf("Got ARP reply from: %s\n", inet_ntoa(ip_addr));
    }
    close(arp_fd);
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
        cout << char_mask << endl;
        ip_hosts.emplace_back(char_mask);
    }

}

void interface_info() {

    struct ifaddrs *ifaddr, *ifa;
    num_of_if = 0;

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

            // get own IP address
            memcpy(ifr.ifr_name, ifa->ifa_name, IF_NAMESIZE);
            if (ioctl(sock, SIOCGIFADDR, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            interface[num_of_if].ip_addr = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);

            // get netmask of interface
            if (ioctl(sock, SIOCGIFNETMASK, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            interface[num_of_if].mask = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);

            // get index of interface
            if (ioctl(sock, SIOCGIFINDEX, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            interface[num_of_if].if_index = ifr.ifr_ifindex;

            // get MAC address
            if (ioctl(sock, SIOCGIFHWADDR, &ifr, sizeof(ifr)) < 0)
                print_error(ERR_IOCTL);

            strcpy(interface[num_of_if].mac, ifr.ifr_hwaddr.sa_data);

            close(sock);

            num_of_if++;
        }


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
                printf("option h \n");
                print_error(ERR_HELP);
                break;

            case 'i':
                if (flags.i)
                    print_error(ERR_DUPL);
                args.interface = optarg;
                printf("option i with value '%s'\n", args.interface);
                flags.i++;
                break;

            case 'n':
                if (flags.n)
                    print_error(ERR_DUPL);
                args.network = optarg;
                printf("option n with value '%s'\n", args.network);
                flags.n++;
                break;

            case 't':
                if (flags.t)
                    print_error(ERR_DUPL);
                args.tcp = true;
                printf("option t '%d'\n", args.tcp);
                flags.t++;
                break;

            case 'u':
                if (flags.u)
                    print_error(ERR_DUPL);
                args.udp = true;
                printf("option u '%d'\n", args.udp);
                flags.u++;
                break;


            case 'p':
                if (flags.p)
                    print_error(ERR_DUPL);
                args.port = validated_number(optarg, PORT_CHECK);
                printf("option p with value '%d'\n", args.port);
                flags.p++;
                break;

            case 'w':
                if (flags.w)
                    print_error(ERR_DUPL);
                args.wait = validated_number(optarg, TIME_CHECK);
                printf("option w with value '%d'\n", args.wait);
                flags.w++;
                break;

            case '?':
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < count) {
        printf("non-option ARGV-elements: ");
        while (optind < count)
            printf("%s ", argument[optind++]);
        printf("\n");

        print_error(ERR_HELP);
    }

    network_separator();

    if (validated_number(args.ip, IP_CHECK) != 1)
        print_error(ERR_NETW);

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
    char *ptr = NULL;
    long tmp;

    if (type != IP_CHECK) {
        tmp = strtol(input, &ptr, 10);
        if (*ptr != NULL)
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

        default:
            fprintf(stderr, "Unknown error");
            break;

    }
    exit(error);
}