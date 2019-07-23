#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msgpack.hpp>
#include <ostream>
#include <iostream>

#define PORT    20001
#define MAXLINE 1024

struct ngps_t{
    bool st_status;
    uint edge_index;
    double edge_offset;
    double velocity;
    MSGPACK_DEFINE(st_status, edge_index, edge_offset, velocity)

    friend std::ostream &operator<<(std::ostream &os, const ngps_t &ngps) {
        os << "st_status: " << ngps.st_status << " edge_index: " << ngps.edge_index << " edge_offset: "
           << ngps.edge_offset << " velocity: " << ngps.velocity;
        return os;
    }
};

int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char * hello = "Hello from client";
    struct sockaddr_in     servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    ngps_t ngps{
        false,
        1,
        10.4,
        -2.8
    };

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, ngps);

    sendto(sockfd, sbuf.data(), sbuf.size(),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,
                 (socklen_t*)&len);
    msgpack::object_handle oh =
            msgpack::unpack(buffer, n);

    msgpack::object obj = oh.get();

    ngps_t rt;
    obj.convert(rt);

    std::cout << rt << std::endl;

    close(sockfd);
    return 0;
}