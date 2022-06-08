/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <fcntl.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 1024 // max number of bytes we can get at once 
#define BUFFERSIZE 1024

char buffer[BUFFERSIZE];
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// return allocated char array that the user has inputted into the system.
void get_input()
{
    char c;
    memset(buffer, 0, BUFFERSIZE);
    printf(">: ");
    for(int i=0; i< BUFFERSIZE; i++)
    {
        c = getc(stdin);
        if( c== '\n')
        {
            buffer[i] = '\0';
            break;
        }
        buffer[i] = c;
    }
    printf("[CLIENT]: got message from prompt.\n");
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[BUFFERSIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("[CLIENT] socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("[CLIENT] connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "[CLIENT] failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("[CLIENT] connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure
    while(1)
    {
        get_input();
        if(send(sockfd, buffer, strlen(buffer) + 1, 0) == -1)
        {
            perror("ERROR: cannot send the message");
            break;
        }
        printf("[CLIENT] sent message.\n");
        if ((numbytes = recv(sockfd, buf, BUFFERSIZE-1, 0)) == -1) {
            perror("recv");
            break;
        }

        buf[numbytes] = '\0';

        printf("[CLIENT] received '%s', number of bytes: %d\n",buf, numbytes);
    }
    
    
    close(sockfd);

    return 0;
}