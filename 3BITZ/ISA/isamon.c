#include "isamon.h"


int main(int argc, char *argv[]) {


    cli_parser(argc, argv);
    //Set up a socket to use.
    int socket = get_socket();

    //start scanning
    start_scan(socket,args.ip,0,65535);


}








int get_socket(){
    if(socket(AF_INET, SOCK_STREAM, 6)==-1) //6=tcp
        return -1;
}


void start_scan(int socketfd, char *host, int start_port, int end_port){

    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 6; //tcp
    hints.ai_flags = 0;

    //for converting port int to string
    char port_s[6];
    sprintf(port_s,"%d",start_port);
    int address_info = getaddrinfo(host, port_s, &hints, &result);

    if(address_info != 0){
        puts("Failed to find host exiting...");
        exit(0);
    }

    struct sockaddr_in * port_number = (struct sockaddr_in *) result->ai_addr;

    for(int port = start_port; port<=end_port; port++){
        port_number->sin_port = htons(port);

        int connection = connect(socketfd,result->ai_addr, result->ai_addrlen);

        if(connection == 0){
            printf("Port %d is open\n",port);
            close(socketfd);
            socketfd = get_socket();
        }else{
            printf("Port %d is closed\n",port);
        }

    }
    freeaddrinfo(result);

}

void cli_parser(int count, char *argument[]) {


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
                print_error(0);
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

    while (token != NULL)
    {
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

int validated_number(char *input, int type) {
    char *ptr = NULL;
    long tmp;

    if (type != IP_CHECK) {
        tmp = strtol(input, &ptr, 10);
        if (*ptr != NULL)
            print_error(ERR_HELP);

        if (type == PORT_CHECK && (tmp > 65535 || tmp < 0))
            print_error(ERR_PORT);

        if (type == TIME_CHECK && (tmp > 5000 || tmp < 0))
            print_error(ERR_WAIT);
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

        default:
            fprintf(stderr, "Unknown error");
            break;

    }
    exit(error);
}

