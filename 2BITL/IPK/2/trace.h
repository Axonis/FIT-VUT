#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/icmp.h>
#include <netinet/icmp6.h>

/* Structure for extended socket errors, problems with inheritance */
struct sock_extended_err {
    __u32 ee_errno;
    __u8 ee_origin;
    __u8 ee_type;
    __u8 ee_code;
    __u8 ee_pad;
    __u32 ee_info;
    __u32 ee_data;
} *sock_err;

/* Structure for input arguments */
struct cli_arguments {
    int max_ttl;
    int start_ttl;
    char *hostname;
} args;

struct timeval send_time, recv_time;/* Time to measure RTT */
struct sockaddr_storage storage;     /* Universal socket for msg */
struct addrinfo hints;              /* Hints for gedaddrinfo */
struct addrinfo *res;               /* Ouput for getaddrinfo */
struct timeval timeout;             /* Timeout for select */
struct iovec iov;                   /* IO for ICMP header */
struct msghdr msg;                  /* Recieved MSG, contains multiple cmsg */
struct cmsghdr *cmsg;               /* Single cmsg */
struct icmphdr *icmph;              /* ICMP header */

#define BUF_SIZE                    512
#define SO_EE_ORIGIN_NONE           0
#define SO_EE_ORIGIN_LOCAL          1
#define SO_EE_ORIGIN_ICMP           2
#define SO_EE_ORIGIN_ICMP6          3
#define SO_EE_ORIGIN_TIMESTAMPING   4

char cur_name[512];     /* Name of current node */
char ip_addr[512];      /* IP address of current node */
char buffer[512];       /* Buffer for control msg */
int sock = 0;           /* Return value of socket() */
int set1 = 0;           /* Return value of setsockopt() */
int set2 = 0;           /* Return value of setsockopt() */
int on = 1;             /* Auxiliary variable to fill structures correctly */
int fin = 1;            /* Auxiliary variable to check whether destination was reached */
ssize_t rcv = 0;        /* Auxiliary variable to check output of select() */
fd_set fds;             /* Auxiliary variable to fill structures correctl*/
char *port = "33456";   /* Port number */
/* Forum entry says to use dynamic ports, but lot of routers have firewalls to block UDP packets
     * aside from traceroute designed ports. */


void cli_parser(int count, char *argument[]);

void help();

void trace();

double rtt(struct timeval recv_time,struct timeval send_time);


