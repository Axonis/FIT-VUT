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
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>

#define ERR_ARG 1


void cli_parser(int count, char *argument[]);
void print_usage();
int validated_number(char* input, bool type);

struct cli_arguments {
    bool help;
    char* interface;
    bool tcp;
    bool udp;
    int port;
    int wait;
    char* network;
} args;