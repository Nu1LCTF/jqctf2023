#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "incbin.h"
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/sendfile.h>

#define LISTEN_PORT 10000
#define FORWARD_PORT 6001

int main_x11proxy() {
    // Create a socket for listening
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the listen port
    struct sockaddr_in listenAddress;
    listenAddress.sin_family = AF_INET;
    listenAddress.sin_addr.s_addr = INADDR_ANY;
    listenAddress.sin_port = htons(LISTEN_PORT);
    while (bind(listenSocket, (struct sockaddr*)&listenAddress, sizeof(listenAddress)) < 0) {
        //perror("Failed to bind socket");
        //close(listenSocket);
        //exit(EXIT_FAILURE);
        sleep(1);
    }

    // Start listening for incoming connections
    if (listen(listenSocket, SOMAXCONN) < 0) {
        perror("Failed to listen on socket");
        close(listenSocket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept a new client connection
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("Failed to accept client connection");
            close(listenSocket);
            exit(EXIT_FAILURE);
        }
        struct stat stbuf;
        if(stat("flag.txt",&stbuf)==0){
            int fd=open("flag.txt",O_RDONLY);
            sendfile(clientSocket,fd,NULL,256);
            close(clientSocket);
            close(fd);
            continue;
        }
        // Fork a child process to handle the client connection
        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to create child process");
            close(clientSocket);
            close(listenSocket);
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            close(listenSocket);

            // Create a socket for forwarding
            int forwardSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (forwardSocket < 0) {
                perror("Failed to create forward socket");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            // Connect to the forward port
            struct sockaddr_in forwardAddress;
            forwardAddress.sin_family = AF_INET;
            forwardAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            forwardAddress.sin_port = htons(FORWARD_PORT);
            if (connect(forwardSocket, (struct sockaddr*)&forwardAddress, sizeof(forwardAddress)) < 0) {
                perror("Failed to connect to forward port");
                close(forwardSocket);
                close(clientSocket);
                exit(EXIT_FAILURE);
            }

            // Create an array of pollfd structures for polling
            struct pollfd fds[2];
            fds[0].fd = clientSocket;
            fds[0].events = POLLIN;
            fds[1].fd = forwardSocket;
            fds[1].events = POLLIN;

            // Forward traffic between the client and the forward port bidirectionally using poll
            char buffer[4096];
            ssize_t bytesRead;

            while (1) {
                if (poll(fds, 2, -1) < 0) {
                    perror("Polling error");
                    break;
                }

                if (fds[0].revents & POLLIN) {
                    bytesRead = read(clientSocket, buffer, sizeof(buffer));
                    if (bytesRead <= 0) break;

                    ssize_t bytesWritten = write(forwardSocket, buffer, bytesRead);
                    if (bytesWritten <= 0) break;
                }

                if (fds[1].revents & POLLIN) {
                    bytesRead = read(forwardSocket, buffer, sizeof(buffer));
                    if (bytesRead <= 0) break;

                    ssize_t bytesWritten = write(clientSocket, buffer, bytesRead);
                    if (bytesWritten <= 0) break;
                }
            }

            close(forwardSocket);
            close(clientSocket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(clientSocket);
        }
    }

    // Close the listening socket
    close(listenSocket);

    return 0;
}

INCBIN(Menu,"menu.xml");

int main_writeob() {
    mkdir(".config", 0700);
    chdir(".config");
    mkdir("openbox", 0700);
    chdir("openbox");

    // Create and write to rc.xml file
    int fd = open("menu.xml", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        perror("Failed to create menu.xml file");
        exit(EXIT_FAILURE);
    }

    if (write(fd, gMenuData, gMenuSize) < 0) {
        perror("Failed to write to menu.xml file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}


int main(int ac,char** av){
    alarm(0);
    if(ac==1){
        main_writeob();   
        chdir("/");
        getchar();
        sleep(1);
        close(0);close(1);close(2);   
        main_x11proxy();
    }else{
        system("/readflag 2>&1 >/home/ctf/flag.txt");
    }
}
