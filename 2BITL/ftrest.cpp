/** CLIENT */

#include <iostream>
#include <vector>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>

using namespace std;

struct cli_arguments {
    string option;
    string hostname;
    uint16_t port;
    string file;
    string local;
} args;

int client_socket;
struct hostent *server;
struct sockaddr_in server_address;


void parser(int count, vector<string> argument);

void initiate_communication();

void communicate();

string http_header();

int main(int argc, char **argv) {
    vector<string> arguments; // Casting args to vector of strings
    for (int i = 0; i < argc; i++)
        arguments.push_back(string(argv[i]));

    parser(argc, arguments);
    /*cout << "Hostname: " << args.hostname << endl;
    cout << "Port: " << args.port << endl;
    cout << "File: " << args.file << endl;
    cout << "Local: " << args.local << endl;*/

    //http_header();

    initiate_communication();
    communicate();

    return 0;
}


string http_header() {
    string header = "";
    int length = 0;
    if(!args.local.empty())
        length = (int)args.local.length();

    const char *day[] = {"Sun", "Mon", "Tue",
                         "Wed", "Thu", "Fri", "Sat"};

    const char *month[] = {"Jan", "Feb", "Mar", "Apr", "May",
                           "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    char command[100];
    if (args.option == "del" || args.option == "rmd")
        strcpy(command, "DEL");
    if (args.option == "mkd" || args.option == "put")
        strcpy(command, "PUT");
    if (args.option == "lst" || args.option == "get")
        strcpy(command, "GET");

    char type[100];
    if (args.file.find(".") != string::npos)
        strcpy(type, "file");
    else
        strcpy(type, "folder");

    char request[100];
    sprintf(request, "%s %s?type=%s HTTP/1.1\r\n", command, args.file.c_str(), type);
    header += request;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    //Date: Wed, 21 Oct 2015 07:28:00 GMT
    char date[100];
    sprintf(date, "Date: %s %i %s %i %i:%i:%i CET\r\n", day[ltm->tm_wday], ltm->tm_mday,
            month[ltm->tm_mon], ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    header += date;

    char accept[100];
    sprintf(accept, "Accept: text/plain\r\n");
    header += accept;

    char accept_encoding[100];
    sprintf(accept_encoding, "Accept-Encoding: gzip\r\n");
    header += accept_encoding;

    char content_type[100];
    sprintf(content_type, "Content-Type: text/plain\r\n");
    header += content_type;

    char content_length[100];
    sprintf(content_length, "Content-Length: %i\r\n\r\n", length);
    header += content_length;

    if(!args.local.empty()){
       header += args.local;
    }
    return header;

}

void communicate() {
    ssize_t bytesrx = 0;
    ssize_t bytestx = 0;
    char out_buf[1024] = {};
    char in_buf[1024] = {};

    strcpy(out_buf, http_header().c_str());

    /* odeslani zpravy na server */
    bytestx = send(client_socket, out_buf, strlen(out_buf), 0);
    if (bytestx < 0)
        perror("ERROR in sendto");
    // todo vector char pre recv
    // todo odosielanie podla file meni pristup
    /* prijeti odpovedi a jeji vypsani */
    bytesrx = recv(client_socket, in_buf, 1024, 0);
    if (bytesrx < 0)
        perror("ERROR in recvfrom");

    printf("Echo from server:\n%s", in_buf);

    close(client_socket);

}

void initiate_communication() {

    /* 2. ziskani adresy serveru pomoci DNS */
    if ((server = gethostbyname(args.hostname.c_str())) == NULL) {
        fprintf(stderr, "ERROR: no such host as %s\n", args.hostname.c_str());
        exit(EXIT_FAILURE);
    }

    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_address.sin_addr.s_addr, (size_t) server->h_length);
    server_address.sin_port = htons(args.port);

    /* tiskne informace o vzdalenem soketu */
    //printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    /* Vytvoreni soketu */
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }
}


/*
 del smaže soubor určený REMOTE-PATH na serveru
 get zkopíruje soubor z REMOTE-PATH do aktuálního lokálního adresáře či na místo určené pomocí LOCAL-PATH je-li uvedeno
 put zkopíruje soubor z LOCAL-PATH do adresáře REMOTE-PATH
 lst  vypíše obsah vzdáleného adresáře na standardní výstup (formát bude stejný jako výstup z příkazu ls)
 mkd vytvoří adresář specifikovaný v REMOTE-PATH na serveru
 rmd odstraní adresář specifikovaný V REMOTE-PATH ze serveru */

void parser(int count, vector<string> argument) {

    if (count > 4 || count <= 2) {
        cout << "Invalid number of arguments : " << count << endl;
        exit(1);
    }

    if (argument[1] != "del" &&
        argument[1] != "get" &&
        argument[1] != "put" &&
        argument[1] != "lst" &&
        argument[1] != "mkd" &&
        argument[1] != "rmd") {
        cout << "Invalid command : " << argument[1] << endl;
        exit(1);
    }

    args.option = argument[1];

    if (argument[1] == "put" && count != 4) {
        cout << "Put has dependency on 3rd argument - local path.";
        exit(1);
    } else if (count == 4) {
        args.local = argument[3];
    }

    string http("http://");
    unsigned int colon_pos = (int) argument[2].find_first_of(":", http.size());
    unsigned int slash_pos = (int) argument[2].find_first_of("/", colon_pos);

    args.hostname = argument[2].substr(http.size(), colon_pos - http.size());
    //cout << "Host is " << args.hostname << endl;

    string port_string = argument[2].substr(colon_pos + 1, slash_pos);
    //cout << "Port_string:" << port_string << endl;
    args.port = (uint16_t) stoi(port_string);

    args.file = argument[2].substr(slash_pos);

}