#include <unistd.h>
#include <ctime>
#include <cstdio>
#include <sys/time.h>
#include <netdb.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <cctype>
#include <net/if.h>
#include <netinet/ip_icmp.h>
#include <ifaddrs.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <fcntl.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <thread>
#include <algorithm>

struct L4_packet {
    uint32_t ip_src;
    uint32_t ip_dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t length;
};

uint8_t buffer[IP_MAXPACKET];


struct if_info {
    std::string name;
    std::string ip_addr;
    uint32_t ip_bin;
    std::string mask;
    char mac[14];
    int if_index;
};


int num_of_if;
int interface_to_scan;
int start_port, end_port;

std::vector<std::string> ip_hosts;
std::vector<std::string> ip_active;
std::vector<int> udp_blocked_port;

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
    ERR_RECV,
};


#define PORT_CHECK 100
#define MASK_CHECK 101
#define IP_CHECK 102
#define TIME_CHECK 103
#define MAGIC_PORT 12345

void cli_parser(int count, char *argument[]);

void print_error(int error);

int validated_number(char *input, int type);

void network_separator();

void hosts();

void interface_info();

void udp_send(std::string ip_addr);

void udp_recieve(std::string ip_addr);

void tcp_send(std::string ip_addr);

void tcp_recieve();

void icmp_send();

void icmp_recieve();

void arp_send(if_info intf);

void arp_recieve();

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

    uint8_t src_mac[ETH_ALEN];
    uint32_t src_ip;
    uint8_t dst_mac[ETH_ALEN];
    uint32_t dst_ip;

    uint8_t padding[18];           // Padding
} __attribute__ ((__packed__));
