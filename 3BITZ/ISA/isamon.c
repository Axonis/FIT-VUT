
#include "isamon.h"
#include <sys/ioctl.h>
#include "netpacket/packet.h"
#include "net/ethernet.h"
#include "net/if_arp.h"


char *hosts[128];
char *interface[] = {};

int get_socket();

void arp();
void ip_convertor();

struct arp_packet {
    struct ether_header ether;      // Ethernet Header
    struct arphdr arp;              // ARP Header

    uint8_t sender_mac[ETH_ALEN];
    uint32_t sender_ip;
    uint8_t target_mac[ETH_ALEN];
    uint32_t target_ip;

    uint8_t padding[18];           // Paddign
} __attribute__ ((__packed__));


int main(int argc, char *argv[]) {


    args.wait = 500000;
    cli_parser(argc, argv);
    //Set up a socket to use.
    int socket = get_socket();

    /*if interface is not set scan all*/
    list_interfaces();

    //start scanning
    //start_scan(socket, args.ip, 80, 80);
    //ping(args.ip);
    //udp(args.ip, "33456");

    //arp();
    ip_convertor();
}

void arp() {


    int arp_fd, if_fd;
    ssize_t tmp;
    struct ifreq ifr;
    uint32_t own_ip;
    int if_index;


    if_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (if_fd < 0)
        print_error(ERR_SOCK);

    // get own IP address
    memcpy(ifr.ifr_name, interface[0], IF_NAMESIZE);
    tmp = ioctl(if_fd, SIOCGIFADDR, &ifr, sizeof(ifr));
    if (tmp < 0) {
        perror("SIOCGIFADDR");
        exit(1);
    }
    struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;
    own_ip = ntohl(sin->sin_addr.s_addr);


    // get index of interface (e.g. eth0 -> 1)
    tmp = ioctl(if_fd, SIOCGIFINDEX, &ifr, sizeof(ifr));
    if (tmp < 0) {
        perror("IOCTL");
        exit(1);
    }
    if_index = ifr.ifr_ifindex;


    // get own MAC address
    tmp = ioctl(if_fd, SIOCGIFHWADDR, &ifr, sizeof(ifr));
    if (tmp < 0) {
        perror("IOCTL");
        exit(1);
    }

    close(if_fd);




    // Socket to send ARP packet
    arp_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (arp_fd == -1)
        print_error(ERR_SOCK);

    struct arp_packet pkt;
    memset(pkt.ether.ether_dhost, 0xFF, sizeof(pkt.ether.ether_dhost)); // destination broadcast
    memcpy(pkt.ether.ether_shost, ifr.ifr_hwaddr.sa_data, sizeof(pkt.ether.ether_dhost)); // source local interface
    pkt.ether.ether_type = htons(ETHERTYPE_ARP);

    pkt.arp.ar_hrd = htons(ARPHRD_ETHER);
    pkt.arp.ar_pro = htons(ETHERTYPE_IP);
    pkt.arp.ar_hln = ETHER_ADDR_LEN;
    pkt.arp.ar_pln = sizeof(pkt.sender_ip);
    pkt.arp.ar_op = htons(ARPOP_REQUEST);

    memcpy(pkt.sender_mac, ifr.ifr_hwaddr.sa_data, sizeof(pkt.sender_mac));
    pkt.sender_ip = htonl(own_ip);
    memset(pkt.target_mac, 0, sizeof(pkt.target_mac));
    pkt.target_ip = inet_addr(args.ip);

    memset(pkt.padding, 0, sizeof(pkt.padding));


    struct sockaddr_ll sa;
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    sa.sll_ifindex = if_index;
    sa.sll_hatype = ARPHRD_ETHER;
    sa.sll_pkttype = PACKET_BROADCAST;
    sa.sll_halen = 0;
    // sa.sll_addr not set


    tmp = sendto(arp_fd, &pkt, sizeof(pkt), 0, (struct sockaddr *) &sa, sizeof(sa));
    if (tmp < 0)
        print_error(ERR_SEND);

    tmp = recvfrom(arp_fd, &pkt, sizeof(pkt), 0, NULL, NULL);
    if (tmp == -1)
        print_error(ERR_SEND);
    else {
        struct in_addr ip_addr;
        ip_addr.s_addr = pkt.sender_ip;
        printf("The IP address is %s\n", inet_ntoa(ip_addr));
    }
    close(arp_fd);

}

void string_to_octet(int ip[4],char *addr) {

    char *tmp;
    char *aux;
    strcpy(aux, addr);

    tmp = strtok(aux, ".");
    ip[0] = atoi(tmp);

    tmp = strtok(NULL, ".");
    ip[1] = atoi(tmp);

    tmp = strtok(NULL, ".");
    ip[2] = atoi(tmp);

    tmp = strtok(NULL, " ");
    ip[3] = atoi(tmp);

}


void ip_convertor() {

    char s_mask[128];
    long bits = 0;
    unsigned int iterator;
    int ip_start[4];
    int ip_end[4];
    struct in_addr struct_host, struct_mask, broadcast;
    char broadcast_address[INET_ADDRSTRLEN];
    bits = 0xffffffff ^ (1 << 32 - args.mask) - 1;
    int mask[4];

    mask[0] = (bits & 0x0000000000ff000000) >> 24;
    mask[1] = (bits & 0x0000000000ff0000) >> 16;
    mask[2] = (bits & 0x0000000000ff00) >> 8;
    mask[3] = bits & 0xff;

    sprintf(s_mask,"%d.%d.%d.%d", mask[0], mask[1], mask[2], mask[3]);


    if (inet_pton(AF_INET, args.ip, &struct_host) == 1 &&
        inet_pton(AF_INET, s_mask, &struct_mask) == 1)
        broadcast.s_addr = struct_host.s_addr | ~struct_mask.s_addr;
    else
        print_error(ERR_NETW);



    if (inet_ntop(AF_INET, &broadcast, broadcast_address, INET_ADDRSTRLEN) != NULL)
        printf("Broadcast address of %s with netmask %s is %s\n",
               args.ip, s_mask, broadcast_address);
    else
        print_error(ERR_NETW);

    if(strcmp(args.ip, broadcast_address) == 0)
        return;

    string_to_octet(ip_start, args.ip);
    string_to_octet(ip_end, broadcast_address);

    for (int i = 0; i < 4; i++) {
        printf("%d.", ip_start[i] & mask[i]);
        if (ip_start[i] == (ip_start[i] & mask[i]))
            ;
        else
            ;//print_error(ERR_NETW);
    }


    int start_ip= (
            ip_start[0] << 24 |
            ip_start[1] << 16 |
            ip_start[2] << 8 |
            ip_start[3]);

    int end_ip= (
            ip_end[0] << 24 |
            ip_end[1] << 16 |
            ip_end[2] << 8 |
            ip_end[3]);

    int i = 0;

    for (iterator=start_ip; iterator < end_ip; iterator++)
    {
        sprintf (s_mask," %d.%d.%d.%d\n",
                (iterator & 0xFF000000)>>24,
                (iterator & 0x00FF0000)>>16,
                (iterator & 0x0000FF00)>>8,
                (iterator & 0x000000FF)
        );
        //hosts[i] = s_mask;
        printf("%s", s_mask);
        i++;
    }
    printf("Number of hosts: %d\n", i);


}



void ping(char *host_ip) {

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
    addr.sin_addr.s_addr = inet_addr(host_ip);

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
        print_error(ERR_SOCK);


    if (args.interface != NULL) {
        memset(&ifr, 0, sizeof(ifr));
        snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "wlp7s0");
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
            printf("received ICMP ECHO REPLY from %s\n", host_ip);
            hosts[i] = host_ip;
        } else {
            printf("received ICMP %d\n", icmphdrptr->type);
        }
        i++;
    } else
        printf("nope\n");

    close(sock);

}

/*
 * Function from IPK traceroute project, modified to determine if host is alive or not by sending empty UDP packet and waiting for ICMP response
 * */
int udp(char *ip, char *port) {

    int ttl = 64;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    set1 = setsockopt(sock, SOL_IP, IP_RECVERR, (char *) &on, sizeof(on));
    set2 = setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    if (getaddrinfo(ip, port, &hints, &res) != 0)
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

        /* Wait for answer, if it comes in 0.5 second receive it */
        int rdy = select(sock + 1, &fds, NULL, NULL, &timeout);

        if (rdy == 1) {
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

            }
            close(sock);
            return 0;

            /* Select is not ready to read - timeout */
        } else if (rdy <= 0) {
            close(sock);
            return 1;
        }
    } else {
        print_error(ERR_SEND);
    }


}

int get_socket() {
    if (socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) == -1)
        return -1;
}


void start_scan(int socketfd, char *host, int start_port, int end_port) {

    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 6; //tcp
    hints.ai_flags = 0;

    //for converting port int to string
    char port_s[6];
    sprintf(port_s, "%d", start_port);
    int address_info = getaddrinfo(host, port_s, &hints, &result);

    if (address_info != 0) {
        puts("Failed to find host exiting...");
        exit(0);
    }

    struct sockaddr_in *port_number = (struct sockaddr_in *) result->ai_addr;

    for (int port = start_port; port <= end_port; port++) {

        port_number->sin_port = htons(port);

        int connection = connect(socketfd, result->ai_addr, result->ai_addrlen);

        if (connection == 0) {
            printf("Port %d is open\n", port);
            close(socketfd);
            socketfd = get_socket();
        } else {
            printf("Port %d is closed\n", port);
        }

    }
    freeaddrinfo(result);

}

void list_interfaces() {
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in *sa;
    int i = 0;

    if (getifaddrs(&ifaddr) == -1) {
        print_error(ERR_GAIN);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            interface[i] = ifa->ifa_name;
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            hosts[i] = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\tAddress: %s\n", interface[i], hosts[i]);
        }
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

        default:
            fprintf(stderr, "Unknown error");
            break;

    }
    exit(error);
}

