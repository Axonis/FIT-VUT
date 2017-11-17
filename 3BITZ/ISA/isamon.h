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

#define PORT_CHECK 100
#define MASK_CHECK 101
#define IP_CHECK 102
#define TIME_CHECK 103
#define MAGIC_PORT 12345 // Port from which UDP and TCP packets are being sent

struct L4_packet {
    uint32_t ip_src;
    uint32_t ip_dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t length;
};

struct if_info {
    std::string name;
    std::string ip_addr;
    uint32_t ip_bin;
    std::string mask;
    char mac[14];
    int if_index;
};

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
    struct ether_header ether; // Ethernet Header
    struct arphdr arp; // ARP Header

    uint8_t src_mac[ETH_ALEN];
    uint32_t src_ip;
    uint8_t dst_mac[ETH_ALEN];
    uint32_t dst_ip;

    uint8_t padding[18]; // Padding
} __attribute__ ((__packed__));

if_info interface[10];

uint8_t buffer[IP_MAXPACKET]; // Buffer for all communication

int num_of_if;
int interface_to_scan;
int start_port, end_port;

std::vector<std::string> ip_hosts;
std::vector<std::string> ip_active;
std::vector<int> udp_blocked_port;

/**
 * @brief Parses out stdin arguments and sets configuration info
 *
 * Function checks if correct arguments have been set and loops
 * through whole stdin. Every arguments has element in cli_arguments
 * structure and flag in cli_duplicite_flags to check for duplicity in
 * proposed configuration. Basic checks are performed to determine if
 * proper numbers, IP adress, mask and combination of arguments are set.
 *
 * @param count - number of arguments
 * @param argument[] - array of arguments
 *
 * @return Void.
 *
 */
void cli_parser(int count, char *argument[]);

/**
 * @brief Prints out error to stderr and terminates program
 *
 * Function checks parameter given to determine which
 * enumerator was selected. Appropriate text is printed
 * and program ends with number of this error.
 *
 * @param error - specified error type
 *
 * @return Void.
 *
 */
void print_error(int error);

/**
 * @brief Validates given number according to its needs
 *
 * Function determines what kind of check and validation is required
 * to be perfomed and checks for correctness in data.
 * If no problem is found they are converted into integers.
 *
 * @param input - number in array of char to be converted and checked
 * @param type - type of check and conversion
 *
 * @return tmp - validated and converted number
 *
 */
int validated_number(char *input, int type);

/**
 * @brief Divides IP with CIDR mask
 *
 * Function uses  slash in  CIDR notation as delimeter
 * to divide IP address section and mask to their
 * respective elements in cli_arguments strcture.
 * Basic check if mask is in valid format is performed.
 *
 * @return Void.
 *
 */
void network_separator();

/**
 * @brief Create a vector of all hosts from given network
 *
 * Function transforms netmask to suitable form with
 * bit shifts and binary operations.
 * Later is performed check is given IP from argument is IP of
 * network with given netmask. First and last IP's from network
 * are calculated to be used for loop to list out all IP's in
 * this range. All IP addresses are inserted into vector of
 * hosts to be scanned.
 *
 * @param Void.
 *
 * @return Void.
 *
 */
void hosts();

/**
 * @brief Saves all interfaces and their properties
 *
 * Function uses a STREAM socket to get information from
 * active interfaces such as its name, netmask, ip address
 * mac address and index of interface.
 * All of these information are saved into array of structure
 * if_info which contains all these elements.
 *
 * @param Void.
 *
 * @return Void.
 *
 */
void interface_info();

/**
 * @brief Send empty UDP packets to active hosts
 *
 * Function creates RAW UDP non-blocking socket,
 * which is later filled with layer 3 and layer 4 data,
 * to be sent to host.
 * All requested ports are scanned in loop and socket
 * is closed after every port from given host has been scanned.
 *
 * @param ip_addr
 *
 * @return Void.
 *
 * @bug If sendto sets errno to EAGAIN, there are no available resources. Keeps trying until successful
 */
void udp_send(std::string ip_addr);

/**
 * @brief Recieve UDP packets from given host
 *
 * Function creates RAW ICMP non-blocking socket,
 * which is trying to receive data in an infinite loop
 * until terminated. If data are received, they are parsed
 * out with pointer arithmetic to have both UDP and ICMP data
 * available to use for output.
 * After given timeout expires socket is closed and opened or filtered
 * ports are written to the stdout
 *
 * @param ip_addr
 *
 * @return Void.
 *
 */
void udp_recieve(std::string ip_addr);

/**
 * @brief Send TCP SYN packets to active hosts
 *
 * Function creates RAW TCP non-blocking socket,
 * which is later filled with layer 3 and layer 4 data,
 * and syn flag is set to true, to serve as TCP syn scan.
 * All requested ports are scanned in loop and socket
 * is closed after every port from given host has been scanned.
 *
 * @param ip_addr
 *
 * @return Void.
 *
 * @bug If sendto sets errno to EAGAIN, there are no available resources. Keeps trying until successful.
 */
void tcp_send(std::string ip_addr);

/**
 * @brief Recieve TCP packets in non-blocking manner
 *
 * Function creates RAW TCP non-blocking socket,
 * which is trying to receive data in an infinite loop
 * until terminated. If data are received, they are parsed
 * out with pointer arithmetic to see if TCP syn flag has
 * been received to indicate opened port, or rst otherwise.
 * After given timeout expires socket is closed and opened
 * ports are written to the stdout.
 *
 * @param Void.
 *
 * @return Void.
 *
 * @bug This socket is able to receive sockets that are sent to local interface
 * from same interface therefore check is required to skip them.
 *
 */
void tcp_recieve();

/**
 * @brief Send ICMP packets to all hosts in given network
 *
 * Function creates RAW ICMP non-blocking socket,
 * which is later filled with ICMP appropriate data,
 * as ICMP type ECHO REQUEST to search for active hosts
 * If host is to answer with ECHO REPLY we can insert him
 * into vector of active hosts.
 * All requested hosts are scanned in loop and socket
 * is closed after every host from given network has been scanned.
 * If interface parameter is given socket is bound to interface.
 *
 * @param Void.
 *
 * @return Void.
 */
void icmp_send();

/**
 * @brief Recieve ICMP packets in non-blocking manner
 *
 * Function creates RAW ICMP non-blocking socket,
 * which is trying to receive data in an infinite loop
 * until terminated. If data are received, they are parsed
 * out with pointer arithmetic to see if ICMP ECHO REPLY flag
 * has been received to indicate communicating host and if so
 * host is inserted into vector of active hosts.
 * After given timeout expires socket is closed and active
 * clients are written to the stdout.
 *
 * @param Void.
 *
 * @return Void.
 *
 */
void icmp_recieve();

/**
 * @brief Send ARP packets to all hosts from given interface
 *
 * Function creates RAW ARP non-blocking socket,
 * which is later filled with layer 2 and layer 3 data,
 * to ask who has ip's from network given.
 * All requested hosts are scanned in loop and socket
 * is closed after every host from given network has been scanned.
 * If interface parameter is missing all active interfaces are
 * used for this scan.
 *
 * @param intf - structure if_info
 *
 * @return Void.
 *
 * @bug If sendto sets errno to EAGAIN, there are no available resources. Keeps trying until successful.
 */
void arp_send(if_info intf);

/**
 * @brief Recieve RAW ARP packets from all hosts from given interface
 *
 * Function creates RAW ARP non-blocking socket,
 * which is trying to receive data in an infinite loop
 * until terminated. If data are received, ARP reply
 * has been received and host from this packet is inserted
 * into active hosts vector.
 * After given timeout expires socket is closed and and active
 * clients are written to the stdout.
 *
 * @param Void.
 *
 * @return Void.
 *
 */
void arp_recieve();

/**
 * @brief Generated checksum according to RFC 1071
 *
 * Function creates checksum based on data and their size.
 * Standard checksum computation based on RFC to fill out
 * checksum field in most protocols.
 *
 * @return Void.
 *
 */
unsigned short checksum(unsigned short *buff, int size);

