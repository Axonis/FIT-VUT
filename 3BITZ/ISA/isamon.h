#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/icmp.h>
#include <netinet/icmp6.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>

#define ERR_HELP 0
#define ERR_PORT 1
#define ERR_WAIT 2
#define ERR_DUPL 3
#define ERR_NETW 4
#define ERR_SOPT 5
#define ERR_SOCK 6
#define ERR_GAIN 7
#define ERR_SEND 8

#define PORT_CHECK 100
#define MASK_CHECK 101
#define IP_CHECK 102
#define TIME_CHECK 103


void cli_parser(int count, char *argument[]);

void print_error(int error);

int validated_number(char *input, int type);

void network_separator();

int udp(char* ip, char* port);

struct cli_arguments {
    char *interface;
    bool tcp;
    bool udp;
    int port;
    int wait;
    char *network;
    char* ip;
    int mask;
} args;

struct cli_duplicite_flags {
    int i;
    int n;
    int t;
    int u;
    int p;
    int w;
} flags;



struct addrinfo hints;              /* Hints for gedaddrinfo */
struct addrinfo *res;               /* Ouput for getaddrinfo */
struct timeval timeout;             /* Timeout for select */
struct msghdr msg;                  /* Recieved MSG */

int sock = 0;           /* Return value of socket() */
int set1 = 0;           /* Return value of setsockopt() */
int set2 = 0;           /* Return value of setsockopt() */
int on = 1;             /* Auxiliary variable to fill structures correctly */
fd_set fds;             /* Auxiliary variable to fill structures correctl*/

struct sock_extended_err {
    __u32 ee_errno;
    __u8 ee_origin;
    __u8 ee_type;
    __u8 ee_code;
    __u8 ee_pad;
    __u32 ee_info;
    __u32 ee_data;
} *sock_err;

struct sockaddr_storage storage;     /* Universal socket for msg */
struct addrinfo hints;              /* Hints for gedaddrinfo */
struct addrinfo *res;               /* Ouput for getaddrinfo */
struct timeval timeout;             /* Timeout for select */
struct iovec iov;                   /* IO for ICMP header */
struct msghdr msg;                  /* Recieved MSG, contains multiple cmsg */
struct cmsghdr *cmsg;               /* Single cmsg */
struct icmphdr *icmph;              /* ICMP header */
ssize_t rcv = 0;        /* Auxiliary variable to check output of select() */

#define SO_EE_ORIGIN_ICMP           2

char buffer[512];       /* Buffer for control msg */