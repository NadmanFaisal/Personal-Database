#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define BUFFER_SIZE 16000

struct Server {
    int domain;
    int port;
    int service;
    int protocol;
    int backlog;
    __u_long interface;

    int socket;
    struct sockaddr_in addr;

    void(*launch)(struct Server *server);
};

struct Server serverConstructor(int domain, int port, int service, int protocol, int backlog, unsigned long interface, void(*launch)(struct Server *server));
void launch(struct Server *server);

#endif