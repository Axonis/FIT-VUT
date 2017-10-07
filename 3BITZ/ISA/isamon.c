#include "isamon.h"


int main(int argc, char *argv[]) {


    cli_parser(argc, argv);

}

int validated_number(char *input, bool type) {
    char *ptr = NULL;
    long tmp;

    tmp = strtol(input, &ptr, 10);
    if (*ptr != NULL) {
        if (type == true) {
            if (tmp > 65535 && tmp < 0)
                print_error(ERR_PORT);
        } else
            print_error(ERR_WAIT);

        print_error(ERR_HELP);
    }

    return (int) tmp;
}


void print_error(int error) {
    switch (error) {
        case 0:
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

        case 1:
            fprintf(stderr, "Invalid port number !\n");
            break;

        case 2:
            fprintf(stderr, "Invalid max RTT !\n");
            break;

        case 3:
            fprintf(stderr, "Duplicite declaration of an argument\n");
            break;

        default:
            fprintf(stderr, "Unknown error");
            break;

    }
    exit(error);
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
                args.port = validated_number(optarg, true);
                printf("option p with value '%d'\n", args.port);
                break;

            case 'w':
                if (flags.w)
                    print_error(ERR_DUPL);
                args.wait = validated_number(optarg, false);
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


}

