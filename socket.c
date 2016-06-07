
/*
 * socket.c -- socket description in C
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    struct addrinfo hints, *res, *p;
    int status,sockfd;
    char ipstr[INET6_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 2;
        }

    //socket description
sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    //socket bind method example
bind(sockfd,res->ai_addr,res->ai_addrlen);
