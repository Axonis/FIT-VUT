/** SERVER */

#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>


using namespace std;

struct cli_arguments {
    string root;
    uint16_t port = 6677;
} args;

struct http_args {
    int code;
    string message;
} http;

struct header_info {
    string path;
    string command;
    string msg;
} header_args;

struct sockaddr_in6 sa;
struct sockaddr_in6 sa_client;
int welcome_socket;
socklen_t sa_client_len = sizeof(sa_client);
string type;


void parser(int count, vector<string> argument);

void initiate_communication();

void communicate();

void header_parser(char *header);

string http_header();

string file_type(const char *path);


int main(int argc, char **argv) {
    vector<string> arguments; // Casting args to vector of strings
    for (int i = 0; i < argc; i++)
        arguments.push_back(string(argv[i]));

    parser(argc, arguments);
    cout << "Root: " << args.root << endl;
    cout << "Port: " << args.port << endl;

    initiate_communication();
    communicate();

    cout << endl << file_type(header_args.path.c_str()) << endl;

    return 0;
}

string file_type(const char *path) {
    struct stat buf;
    stat(path, &buf);
    if(buf.st_mode & S_IFDIR != 0)
        return "dir_exists";
    else if(S_ISREG(buf.st_mode))
        return "is_file";
    else
        return "none";
}

void operation() {
    // TODO BUGFIXING

    // MKD
    if(header_args.command == "PUT"){
        if (file_type(header_args.path.c_str()) == "dir_exists"){
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Already exists.";
        } else if (file_type(header_args.path.c_str()) == "is_file"){
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Not a directory";
        } else if (file_type(header_args.path.c_str()) == "none"){
            mkdir(header_args.path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            http.code = 200;
            http.message = "OK";
            header_args.msg = "Directory was created.";
        }
    }

    if(header_args.command == "GET"){
        // LST
        if (file_type(header_args.path.c_str()) == "dir_exists") {
            char tmp[100];
            sprintf(tmp, "/bin/ls %s", header_args.path.c_str());
            FILE *proc = popen(tmp, "r");
            char buf[2048];
            while (!feof(proc) && fgets(buf, sizeof(buf), proc))
                header_args.msg += buf;
            http.code = 200;
            http.message = "OK";
        } else {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Not a directory";
        }

        // GET
        if (file_type(header_args.path.c_str()) == "is_file") {
            // KOPIROVANIE SUBORU
        }

    }

    if(header_args.command == "DEL"){
        if(file_type(header_args.path.c_str()) == "is_file"){
            remove(header_args.path.c_str());
            http.code = 200;
            http.message = "OK";
            header_args.msg = "File was deleted";
        }
        else if (file_type(header_args.path.c_str()) == "dir_exists"){
            if(rmdir(header_args.path.c_str()) < 0){
                http.code = 400;
                http.message = "Bad request";
                header_args.msg = "Directory not empty.";
            } else {
                http.code = 200;
                http.message = "OK";
                header_args.msg = "Directory was deleted";
            }

        }
        else {
            http.code = 400;
            http.message = "Bad request";
            header_args.msg = "Directory not found";
        }
    }

}

// TODO root default - miesto spustenia
// TODO root + path od clienta

void header_parser(char *header) {
    string head = header;

    header_args.command = head.substr(0, 3);

    size_t path_pos = head.find("?");
    header_args.path = head.substr(4, path_pos-4);

    size_t file_pos_start = head.find("=");
    size_t file_pos_end = head.find(" HTTP/1.1");
    type = head.substr(file_pos_start+1, file_pos_end-file_pos_start-1);
    // todo content length

}

string http_header() {
    string header = "";

    const char *day[] = {"Sun", "Mon", "Tue",
                         "Wed", "Thu", "Fri", "Sat"};

    const char *month[] = {"Jan", "Feb", "Mar", "Apr", "May",
                           "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    char request[100];
    sprintf(request, "HTTP/1.1 %i %s\r\n", http.code, http.message.c_str());
    header += request;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    //Date: Wed, 21 Oct 2015 07:28:00 GMT
    char date[100];
    sprintf(date, "Date: %s %i %s %i %i:%i:%i GMT\r\n", day[ltm->tm_wday], ltm->tm_mday,
            month[ltm->tm_mon], ltm->tm_year + 1900, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    header += date;

    char content_type[100];
    sprintf(content_type, "Content-Type: text/plain\r\n");
    header += content_type;

    char content_length[100];
    sprintf(content_length, "Content-Length: 1024\r\n");
    header += content_length;

    char content_encoding[100];
    sprintf(content_encoding, "Content-Encoding: identity\r\n\r\n");
    header += content_encoding;

    header += header_args.msg;
    return header;
}

void communicate() {

    char str[INET6_ADDRSTRLEN];
    string msg = "";


        int comm_socket = accept(welcome_socket, (struct sockaddr *) &sa_client, &sa_client_len);
        if (comm_socket > 0) {
            if (inet_ntop(AF_INET6, &sa_client.sin6_addr, str, sizeof(str))) {
                /*printf("INFO: New connection:\n");
                printf("INFO: Client address is %s\n", str);
                printf("INFO: Client port is %d\n", ntohs(sa_client.sin6_port));*/
            }

            char buff[2048] = "";
            ssize_t res = 0;
            for (;;) {
                res = recv(comm_socket, buff, sizeof(buff), 0);
                if (res <= 0)
                    break;

                header_parser(buff);
                operation();

                printf("\nMessage from client:\n%s", buff);
                msg = http_header();
                cout << "Message to client" << endl;
                cout << msg.c_str() << endl;

                send(comm_socket, msg.c_str(), strlen(msg.c_str()), 0);
            }
        } else {
            printf(".");
        }
        printf("Connection to %s closed\n", str);
        close(comm_socket);

}


void initiate_communication() {
    int rc;

    if ((welcome_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin6_family = AF_INET6;
    sa.sin6_addr = in6addr_any;
    sa.sin6_port = htons(args.port);


    if ((rc = bind(welcome_socket, (struct sockaddr *) &sa, sizeof(sa))) < 0) {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }
    if ((listen(welcome_socket, 1)) < 0) {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }
}


/*
 -r ROOT-FOLDER specifikuje kořenový adrssář, kde budou ukládány soubory pro jednotlivé uživatele, defaultní hodnota je aktuální
 -p PORT specifikuje port, na kterém bude server naslouchat, implicitně 6677*/

void parser(int count, vector<string> argument) {

    if (count > 5 || count == 4) {
        cout << "Invalid number of arguments : " << count << endl;
        exit(1);
    }

    if (count == 3){
        if(argument[1] == "-r")
            args.root = argument[2];
        else if (argument[1] == "-p"){
            args.port = (uint16_t)stoi(argument[2]);
        }
        else {
            cout << "Wrong arguments." << endl;
            exit(1);
        }
    }

    if (count == 5){
        if(argument[1] == "-r")
            args.root = argument[2];
        else if (argument[1] == "-p"){
            args.port = (uint16_t)stoi(argument[2]);
        }
        else {
            cout << "Wrong arguments." << endl;
            exit(1);
        }

        if(argument[3] == "-r")
            args.root = argument[4];
        else if (argument[3] == "-p"){
            args.port = (uint16_t)stoi(argument[4]);
        }
        else {
            cout << "Wrong arguments." << endl;
            exit(1);
        }
    }

    if (args.root.empty()){
       args.root = get_current_dir_name();
    }

}