#include "isamon.h"


int main(int argc, char *argv[]) {


    cli_parser(argc, argv);

    exit (0);
}

int validated_number(char* input, bool type) {
    char *ptr = NULL;
    long tmp;

    tmp = strtol(input, &ptr, 10);
    if (*ptr != NULL) {
        if (type == true) {
            if (tmp > 65535 && tmp < 0)
                perror("Invalid port number !\n");
        }
        else
            perror("Invalid max RTT !\n");

        print_usage();
        exit(ERR_ARG);
    }

    return (int)tmp;
}


void print_usage() {
    fprintf(stderr, "Usage:\n"
            "isamon [-h] [-i <interface>] [-t] [-u] [-p <port>] [-w <ms>] -n <net_address/mask> \n"
            "   -h --help -- zobrazí nápovědu \n"
            "   -i --interface <interface> -- rozhraní na kterém bude nástroj scanovat \n"
            "   -n --network <net_address/mask> -- ip adresa síťe s maskou definující rozsah pro scanování \n"
            "   -t --tcp -- použije TCP \n"
            "   -u --udp -- použije UDP \n"
            "   -p --port <port> -- specifikace scanovaného portu, pokud není zadaný, scanujte celý rozsah \n"
            "   -w --wait <ms> -- dodatečná informace pro Váš nástroj jaké je maximální přípustné RTT ");
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

        c = getopt_long(count, argument, "htu:i:n:p:w:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {

            case 'h':
                printf("option h \n");
                print_usage();
                exit(0);
                break;

            case 'i':
                args.interface = optarg;
                printf("option i with value '%s'\n", args.interface);
                break;

            case 'n':
                args.network = optarg;
                printf("option n with value '%s'\n", args.network );
                break;

            case 't':
                args.tcp = true;
                printf("option t '%d'\n", args.tcp);
                break;

            case 'u':
                args.udp = true;
                printf("option u '%d'\n", args.udp);
                break;

            case 'p':
                args.port = validated_number(optarg, true);
                printf("option p with value '%d'\n", args.port);
                break;

            case 'w':
                args.wait = validated_number(optarg, false);
                printf("option w with value '%d'\n", args.wait);
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

        print_usage();
        exit(ERR_ARG);
    }


}

