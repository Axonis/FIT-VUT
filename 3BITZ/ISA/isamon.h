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

#define PORT_CHECK 100
#define MASK_CHECK 101
#define IP_CHECK 102
#define TIME_CHECK 103


void cli_parser(int count, char *argument[]);

void print_error(int error);

int validated_number(char *input, int type);

void network_separator();

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