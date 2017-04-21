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

struct sock_extended_err {
    __u32 ee_errno;
    __u8 ee_origin;
    __u8 ee_type;
    __u8 ee_code;
    __u8 ee_pad;
    __u32 ee_info;
    __u32 ee_data;
} *sock_err;

#define SO_EE_ORIGIN_NONE       0
#define SO_EE_ORIGIN_LOCAL      1
#define SO_EE_ORIGIN_ICMP       2
#define SO_EE_ORIGIN_ICMP6      3
#define SO_EE_ORIGIN_TIMESTAMPING 4


struct timeval send_time, recv_time;
struct sockaddr_storage target;
struct addrinfo hints;
struct addrinfo *res;
int sock = 0;
int set1 = 0;
int set2 = 0;
int on = 1;
int fin = 1;
struct timeval timeout;
fd_set fds;
char buffer[512];
ssize_t rcv = 0;


struct iovec iov; //io štruktúra

struct msghdr msg; //prijatá správa - môže obsahovať viac control hlavičiek
struct cmsghdr *cmsg; //konkrétna control hlavička

struct icmphdr *icmph; //ICMP hlavička

struct cli_arguments {
    int max_ttl;
    int start_ttl;
    char *hostname;
} args;

void cli_parser(int count, char *argument[]);

void help();

void trace();


