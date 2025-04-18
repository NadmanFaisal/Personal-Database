#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define READ(s, b, l) recv(s, b, l, 0)
#define WRITE(s, b, l) send(s, b, l, 0)
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define READ(s, b, l) read(s, b, l)
#define WRITE(s, b, l) write(s, b, l)
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

#include "Server.h"
#include "Logger.h"
#include "InputBuffer.h"

struct Server serverConstructor(int domain, int port, int service, int protocol, int backlog, __u_long interface, void (*launch)(struct Server *server)) {
    struct Server server;

#ifdef _WIN32
    WSADATA wsaData;
    int wsaInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaInit != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", wsaInit);
        exit(EXIT_FAILURE);
    }
#endif

    server.domain = domain;
    server.service = service;
    server.port = port;
    server.protocol = protocol;
    server.backlog = backlog;

    server.addr.sin_family = domain;
    server.addr.sin_port = htons(port);
    server.addr.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
    if (server.socket < 0) {
        perror("Failed to initialize/connect to socket...\n");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
        perror("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(EXIT_FAILURE);
    }

    server.launch = launch;
    return server;
}

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("=== WAITING FOR CONNECTION === \n");
        int addrlen = sizeof(server->addr);
        int new_socket = accept(server->socket, (struct sockaddr*)&server->addr, (socklen_t*)&addrlen);
        ssize_t bytesRead = READ(new_socket, buffer, BUFFER_SIZE - 1);
        
        if (bytesRead >= 0) {
            buffer[bytesRead] = '\0';
            // puts(buffer);
        } else {
            perror("Error reading buffer...\n");
        }
        
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4;
            logOutput("CommsFiles/command.txt", "w", "%s\n", body);
            sleep_ms(300);
        }

        const char *header = "HTTP/1.1 200 OK\r\n"
                             "Content-Type: text/plain\r\n"
                             "Access-Control-Allow-Origin: *\r\n"
                             "Connection: close\r\n"
                             "\r\n";
        WRITE(new_socket, header, strlen(header));

        INPUTBUFFER *logBuffer = createBuffer();
        readInputFromFile(logBuffer, "CommsFiles/output.txt");
        WRITE(new_socket, logBuffer->buffer, logBuffer->inputLength);
        free(logBuffer->buffer);
        free(logBuffer);

        close(new_socket);
    }
}