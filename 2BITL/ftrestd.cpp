/** SERVER */

#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <algorithm>
#include <fstream>

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
    string type;
    int content_length;
    string local;
} header_args;

struct sockaddr_in6 sa;
struct sockaddr_in6 sa_client;
int welcome_socket;
socklen_t sa_client_len = sizeof(sa_client);


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

    return 0;
}

string file_type(const char *path) {
    struct stat s;
    int err = stat(path, &s);
    if (err == -1) {
        if (ENOENT == errno)
            return "no_dir";
        else {
            perror("stat");
            exit(1);
        }
    } else if (err == 0) {
        if (S_ISDIR(s.st_mode))
            return "dir";
        else if (S_ISREG(s.st_mode))
            return "file";
        else
            return "exists"; // exists but no dir
    }
}


void operation() {

    string whole_path = args.root + header_args.path;

    size_t user_end = header_args.path.find_first_of("/", 1);
    string user = header_args.path.substr(0, user_end);
    cout << "MENO UZIVATELA: " << user << endl;
    cout << "KONIEC MENA UZIVATELA: " << user_end << endl;
    cout << "DIRECTORY FUNKCIA: " << file_type((whole_path).c_str()) << endl;

    // USER ACCOUNT CHECK
    if (!(file_type((args.root + user).c_str()) == "dir")) {
        http.code = 404;
        http.message = "Not Found";
        header_args.msg = "User Account Not Found.";
    }


    // MKD
    if (header_args.command == "PUT" && header_args.type == "folder") {
        // DIR EXISTS
        if (file_type((whole_path).c_str()) == "dir") {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Already exists.";
            // FILE OR SOMETHING ELSE
        } else if (file_type((whole_path).c_str()) == "exists" ||
                   file_type((whole_path).c_str()) == "file") {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Not a directory";
            // ABLE TO CREATE IT
        } else if (file_type((whole_path).c_str()) == "no_dir") {
            mkdir((whole_path).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            http.code = 200;
            http.message = "OK";
            header_args.msg = "Directory was created.";
        }
    }

    // LST
    if (header_args.command == "GET" && header_args.type == "folder") {
        // if path is directory
        if (file_type((whole_path).c_str()) == "dir") {
            char tmp[100];
            sprintf(tmp, "/bin/ls %s", (whole_path).c_str());
            FILE *proc = popen(tmp, "r");
            char buf[1024];
            while (!feof(proc) && fgets(buf, sizeof(buf), proc))
                header_args.msg += buf;
            http.code = 200;
            http.message = "OK";
            // if not directory
        } else {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Not a directory";
        }
    }

    // GET
    if (header_args.command == "GET" && header_args.type == "file") {
        if (file_type((whole_path).c_str()) == "file") {
            // KOPIROVANIE SUBORU
        }
    }

    // PUT
    if (header_args.command == "PUT" && header_args.type == "file") {
        // TODO
    }


    // DEL
    if (header_args.command == "DEL" && header_args.type == "file") {
        if (file_type((whole_path).c_str()) == "file") {
            remove((whole_path).c_str());
            http.code = 200;
            http.message = "OK";
            header_args.msg = "File was deleted";
        } else if (file_type((whole_path).c_str()) == "dir") {
            http.code = 400;
            http.message = "Bad request";
            header_args.msg = "Not a file.";
        } else if (file_type((whole_path).c_str()) == "dir" ||
                   file_type((whole_path).c_str()) == "exists") {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "File not found.";
        } else {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Unknown error.";
        }
    }

    // RMD
    if (header_args.command == "DEL" && header_args.type == "folder") {
        if (file_type((whole_path).c_str()) == "dir") {
            if (rmdir((whole_path).c_str()) < 0) {
                http.code = 400;
                http.message = "Bad request";
                header_args.msg = "Directory not empty.";
            } else {
                http.code = 200;
                http.message = "OK";
                header_args.msg = "Directory was deleted";
            }
        } else if (file_type((whole_path).c_str()) == "file") {
            http.code = 400;
            http.message = "Bad request";
            header_args.msg = "Not a directory.";
        } else if (file_type((whole_path).c_str()) == "no_dir") {
            http.code = 404;
            http.message = "Not found";
            header_args.msg = "Directory not found.";
        } else {
            http.code = 404;
            http.message = "Not Found";
            header_args.msg = "Unknown error.";
        }
    }

}


void header_parser(char *header) {
    string head = header;

    header_args.command = head.substr(0, 3);

    size_t path_pos = head.find("?");
    header_args.path = head.substr(4, path_pos - 4);

    size_t file_pos_start = head.find("=");
    size_t file_pos_end = head.find(" HTTP/1.1");
    header_args.type = head.substr(file_pos_start + 1, file_pos_end - file_pos_start - 1);

    size_t content_pos_start = head.find("Content-Length: ");
    size_t content_pos_end = head.find("\r\n", content_pos_start);
    header_args.content_length = stoi(
            head.substr(content_pos_start + strlen("Content-Length: "), content_pos_end - content_pos_start));
    size_t end = 4+head.find("\r\n\r\n");
    header_args.local = head.substr(end);

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
    sprintf(content_length, "Content-Length: %i\r\n", (int)header_args.msg.length());
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

            vector<char> file_chunks;
            char buff[1024] = "";
            ssize_t res = 0;

            for (;;) {
                if (header_args.command == "GET" || header_args.type == "file") {

                    const char *crlf2 = "\r\n\r\n";
                    auto itr = search(file_chunks.begin(), file_chunks.end(), crlf2, crlf2 + strlen(crlf2));
                    // KONIEC HLAVICKY
                    long end_of_header = itr - file_chunks.begin() + strlen(crlf2);


                    res = recv(comm_socket, buff, sizeof(buff), 0);
                    if (res <= 0)
                        break;
                    // do file chunk idu data a potom sa ulozia do suboru ako binarne data
                    file_chunks.assign(buff, buff+1024);
                    vector<char> out(file_chunks.begin()+end_of_header, file_chunks.end());

                    cout << "DATA NA ZAPIS: "<< out.data() << endl;
                    // ZAPIS DO SUBORU
                    // local path je cesta kam zapisujem
                    // requestInfo.cBody je subor v binarke
                    ofstream FILE(header_args.local, ios::out | ofstream::binary);
                    copy(out.begin(), out.end(), ostreambuf_iterator<char>(FILE));
                }





                res = recv(comm_socket, buff, sizeof(buff), 0);
                if (res <= 0)
                    break;
                header_parser(buff);
                operation();

                printf("\nMessage from client:\n%s", buff);
                msg = http_header();
                cout << endl << "Message to client" << endl;
                cout << msg.c_str() << endl;


                send(comm_socket, msg.c_str(), strlen(msg.c_str()), 0);
            }
        } else {
            printf(".");
        }
        close(comm_socket);

    }
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

    if (count == 3) {
        if (argument[1] == "-r")
            args.root = argument[2];
        else if (argument[1] == "-p") {
            args.port = (uint16_t) stoi(argument[2]);
        } else {
            cout << "Wrong arguments." << endl;
            exit(1);
        }
    }

    if (count == 5) {
        if (argument[1] == "-r")
            args.root = argument[2];
        else if (argument[1] == "-p") {
            args.port = (uint16_t) stoi(argument[2]);
        } else {
            cout << "Wrong arguments." << endl;
            exit(1);
        }

        if (argument[3] == "-r")
            args.root = argument[4];
        else if (argument[3] == "-p") {
            args.port = (uint16_t) stoi(argument[4]);
        } else {
            cout << "Wrong arguments." << endl;
            exit(1);
        }
    }

    if (args.root.empty()) {
        args.root = get_current_dir_name();
    }

}
