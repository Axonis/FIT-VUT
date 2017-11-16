//
// Created by xurban66 on 4.11.17.
//
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <ctype.h>
#include <net/if.h>
#include <netinet/ip_icmp.h>
#include <ifaddrs.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/ioctl.h>
#include "netpacket/packet.h"
#include "net/ethernet.h"
#include "net/if_arp.h"
#include <fcntl.h>
#include <sys/time.h>

struct addrinfo hints;              /* Hints for gedaddrinfo */
struct addrinfo *res;               /* Ouput for getaddrinfo */
struct timeval timeout;             /* Timeout for select */
struct msghdr msg;                  /* Recieved MSG */

int sock = 0;           /* Return value of socket() */
int set1 = 0;           /* Return value of setsockopt() */
int set2 = 0;           /* Return value of setsockopt() */
int on = 1;             /* Auxiliary variable to fill structures correctly */
fd_set fds;             /* Auxiliary variable to fill structures correctl*/




struct sockaddr_storage storage;     /* Universal socket for msg */
struct iovec iov;                   /* IO for ICMP header */
struct cmsghdr *cmsg;               /* Single cmsg */
struct icmphdr *icmph;              /* ICMP header */
ssize_t rcv = 0;        /* Auxiliary variable to check output of select() */

#define SO_EE_ORIGIN_ICMP           2

struct sock_extended_err {
    unsigned char ee_origin;
    unsigned char ee_type;

} *sock_err;

char buffer[512];       /* Buffer for control msg */


struct if_info {
    std::string name;
    std::string ip_addr;
    std::string mask;
    char mac[14];
    int if_index;
};

int num_of_if;

std::vector<std::string> ip_hosts;
std::vector<std::string> ip_active;

if_info interface[10];

enum error_codes {
    ERR_HELP,
    ERR_PORT,
    ERR_WAIT,
    ERR_DUPL,
    ERR_NETW,
    ERR_SOPT,
    ERR_SOCK,
    ERR_GAIN,
    ERR_SEND,
    ERR_IOCTL,
};


#define PORT_CHECK 100
#define MASK_CHECK 101
#define IP_CHECK 102
#define TIME_CHECK 103


void cli_parser(int count, char *argument[]);

void print_error(int error);

int validated_number(char *input, int type);

void network_separator();

void hosts();

void interface_info();

void tcp(std::string address);

void ping(std::string host_ip);

void arp(std::string host_ip, if_info intf);

unsigned short checksum(unsigned short *buff, int size);


struct cli_arguments {
    char *interface;
    bool tcp;
    bool udp;
    int port;
    int wait;
    char *network;
    char *ip;
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

struct arp_packet {
    struct ether_header ether;      // Ethernet Header
    struct arphdr arp;              // ARP Header

    uint8_t sender_mac[ETH_ALEN];
    uint32_t sender_ip;
    uint8_t target_mac[ETH_ALEN];
    uint32_t target_ip;

    uint8_t padding[18];           // Padding
} __attribute__ ((__packed__));
