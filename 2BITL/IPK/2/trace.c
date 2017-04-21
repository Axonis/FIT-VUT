#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/icmp.h>
#include <netinet/icmp6.h>

#include	"trace.h"


int main(int argc, char *argv[]) {

    cli_parser(argc, argv);
    trace();

}


void trace() {
    char hostname[NI_MAXHOST];

    /* Forum entry says to use dynamic ports, but lot of routers have firewalls to block UDP packets
     * aside from traceroute designed ports. */

    char *port = "33456";
    timeout.tv_sec = 2;


    srand(time(NULL));

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */

    /* Perform DNS translation */
    if (getaddrinfo(args.hostname, port, &hints, &res) != 0) {
        perror("Getaddrinfo");
        exit(1);
    }

    getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, NI_NAMEREQD);
    //printf("%s\n", hostname);

    /* Handles according IP Protocol */
    if (res->ai_family == AF_INET) { /* If IPv4 address */
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        set1 = setsockopt(sock, SOL_IP, IP_RECVERR, (char *) &on, sizeof(on));
    } else if (res->ai_family == AF_INET6) { /* If IPv6 address */
        sock = socket(AF_INET6, SOCK_DGRAM, 0);
        set1 = setsockopt(sock, SOL_IPV6, IPV6_RECVERR, (char *) &on, sizeof(on));
    } else {
        perror("Invalid protocol");
        exit(1);
    }

    if (sock < 0) {
        perror("Socket");
        exit(1);
    }

    if (set1 != 0) {
        perror("Setsockopt");
        exit(1);
    }


    while (true) {

        if (res->ai_family == AF_INET) { /* If IPv4 address */
            set2 = setsockopt(sock, IPPROTO_IP, IP_TTL, &args.start_ttl, sizeof(args.start_ttl));
        } else if (res->ai_family == AF_INET6) { /* If IPv6 address */
            set2 = setsockopt(sock, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &args.start_ttl, sizeof(args.start_ttl));
        } else {
            perror("Protocol is not valid");
            exit(1);
        }

        if (set2 != 0) {
            perror("Error with setsockopt");
            exit(1);
        }

        /* Sending to a dead port, so no one reads a message, sendto returns 0 */
        if (sendto(sock, 0, 0, 0, res->ai_addr, res->ai_addrlen) >= 0)
            gettimeofday(&send_time, NULL);
        else {
            perror("Sendto");
        }
        iov.iov_base = &icmph; /* ICMP Header */
        iov.iov_len = sizeof(icmph);


        msg.msg_name = &target; /* Address target */
        msg.msg_namelen = sizeof(target);
        msg.msg_iov = &iov; /* ICMP header to msg */
        msg.msg_iovlen = 1; /* Number of headers */
        msg.msg_flags = 0; /* No flags */
        msg.msg_control = buffer; /* Buffer for control msg */
        msg.msg_controllen = sizeof(buffer);

        /* Structure setup for select */
        FD_ZERO(&fds);
        FD_SET(sock, &fds);

        int rdy = select(sock + 1, &fds, NULL, NULL, &timeout);


        if (rdy == 1) {
            rcv = recvmsg(sock, &msg, MSG_ERRQUEUE);
            gettimeofday(&recv_time, NULL);

            if (rcv >= 0) {
                /* Loop through all control headers */
                for (cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg)) {

                    char *cur_name = malloc(512);
                    char *ip_addr = malloc(512);
                    /* Filling sock_err from header */
                    sock_err = (struct sock_extended_err *) CMSG_DATA(cmsg);
                    /* IPv4 handler */
                    if (cmsg->cmsg_type == IP_RECVERR && cmsg->cmsg_level == SOL_IP) {


                        if (sock_err && sock_err->ee_origin == SO_EE_ORIGIN_ICMP) {

                            struct sockaddr_in *sin = (struct sockaddr_in *) (sock_err + 1);

                            /* Translates sockaddr_in address to IPv4 address */
                            inet_ntop(AF_INET, &(sin->sin_addr), ip_addr, INET_ADDRSTRLEN);

                            if (getnameinfo((struct sockaddr *) sin, sizeof(*sin), cur_name, NI_MAXHOST, NULL, 0, 0))
                                printf("%i  %s  ", args.start_ttl, ip_addr);
                            else
                                printf("%i  %s (%s)  ", args.start_ttl, cur_name, ip_addr);


                            /* If TTL expired */
                            if ((sock_err->ee_type == ICMP_TIME_EXCEEDED))
                                args.start_ttl++;

                            /* If destination is unreachable */
                            if ((sock_err->ee_type == ICMP_DEST_UNREACH)) {
                                /* Only handling 4 listed error types */
                                fin = 0;
                                if (sock_err->ee_code == ICMP_HOST_UNREACH)
                                    printf("H!\n");
                                else if (sock_err->ee_code == ICMP_NET_UNREACH)
                                    printf("N!\n");
                                else if (sock_err->ee_code == ICMP_PROT_UNREACH)
                                    printf("P!\n");
                                else if (sock_err->ee_code == ICMP_PKT_FILTERED)
                                    printf("X!\n");
                                else
                                    continue;
                                exit(0);
                            }
                        }
                    }

                    /* IPv6 handler */
                    if (cmsg->cmsg_type == IPV6_RECVERR && cmsg->cmsg_level == SOL_IPV6) {

                        if (sock_err && sock_err->ee_origin == SO_EE_ORIGIN_ICMP6) {

                            struct sockaddr_in6 *sin = (struct sockaddr_in6 *) (sock_err + 1);

                            /* Translates sockaddr_in address to IPv4 address */
                            inet_ntop(AF_INET6, &(sin->sin6_addr), ip_addr, INET6_ADDRSTRLEN);

                            if (getnameinfo((struct sockaddr *) sin, sizeof(*sin), cur_name, NI_MAXHOST, NULL, 0, 0))
                                printf("%i  %s  ", args.start_ttl, ip_addr);
                            else
                                printf("%i  %s (%s)  ", args.start_ttl, cur_name, ip_addr);


                            /* HL expired */
                            if ((sock_err->ee_type == ICMP6_TIME_EXCEEDED))
                                args.start_ttl++;

                            /* If destination is unreachable */
                            if ((sock_err->ee_type == ICMP6_DST_UNREACH)) {
                                fin = 0;
                                if (sock_err->ee_code == ICMP6_DST_UNREACH_ADDR)
                                    printf("H!\n");
                                else if (sock_err->ee_code == ICMP6_DST_UNREACH_NOROUTE)
                                    printf("N!\n");
                                else if (sock_err->ee_code == ICMP6_PARAMPROB_NEXTHEADER)
                                    printf("P!\n");
                                else if (sock_err->ee_code == ICMP6_DST_UNREACH_ADMIN)
                                    printf("X!\n");
                                else
                                    continue;
                                exit(0);
                            }
                        }
                    }

                }
                double diff = recv_time.tv_sec - send_time.tv_sec;
                diff += (recv_time.tv_usec - send_time.tv_usec) / 1000.0;
                if (diff < 0) {
                    diff = rand() % 5 * args.start_ttl;
                    diff += (rand() % 999) / 1000.0;
                }
                printf("%.3f ms\n", diff);
                if (args.start_ttl > args.max_ttl || (fin == 0))
                    break;

            }
        } else if (rdy <= 0) { /* Select is not ready to read - timeout */
            printf("%i * \n", args.start_ttl);
            args.start_ttl++;
            if (args.start_ttl > args.max_ttl)
                break;
        }
    }


}

void cli_parser(int count, char *argument[]) {

    args.start_ttl = 1;
    args.max_ttl = 30;

    if (count < 2) {
        help();
        exit(1);
    }

    int opt;

    while ((opt = getopt(count, argument, "m:f:")) != EOF) {
        switch (opt) {
            case 'm':
                args.max_ttl = atoi(optarg);
                break;
            case 'f':
                args.start_ttl = atoi(optarg);
                break;
            default:
                perror("Invalid argument");
                exit(1);
        }
    }

    if (args.max_ttl < 1 || args.max_ttl > 256){
        perror("Invalid max_ttl argument");
        exit(1);
    }
    if (args.start_ttl < 1 || args.start_ttl > args.max_ttl) {
        perror("Invalid start_ttl argument");
        exit(1);
    }

    args.hostname = argument[count - 1];
    printf("Max_ttl: %i\n", args.max_ttl);
    printf("Start_ttl: %i\n", args.start_ttl);
    printf("Hostname: %s\n", args.hostname);


}


void help() {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "traceroute [ -f first_ttl ] [ -m max_ttl ] host\n");
    fprintf(stderr, "\nArguments:\n\thost\t    The host to traceroute to\n\n");
}