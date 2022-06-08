#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Reactor.hpp"

#define PORT "3490" // port we're listening on
// get sockaddr, IPv4 or IPv6:
using namespace ex6;

typedef struct 
{
    Reactor * reactor;
    struct sockaddr_storage * remoteaddr; // client address
    socklen_t * addrlen;
    int listener_fd;
    char remoteIP[INET6_ADDRSTRLEN];

}accept_struct;
typedef struct
{
    Reactor * reactor;
    int fd;
    int listener_fd;
    char * buffer;
    int buffersize;

} read_struct;

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) 
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * @brief handles each client sending messge to appear in other client's socket fds
 * 
 * @param read_st 
 */
void handle_client_data(void * read_st)
{

    read_struct * rd_st = (read_struct *) read_st;
    int nbytes;
    // handle data from a client
    printf("[SERVER] reading message from fd %d\n" ,rd_st->fd);
    if ((nbytes = recv(rd_st->fd, rd_st->buffer, rd_st->buffersize, 0)) <= 0) 
    {
        // got error or connection closed by client
        if (nbytes == 0) 
        {
            // connection closed
            printf("selectserver: socket %d hung up\n", rd_st->fd);
        } 
        else 
        {
            perror("recv");
        }
        close(rd_st->fd); // bye!
        FD_CLR(rd_st->fd, &rd_st->reactor->fds); // remove from master set
    } 
    else 
    {
        printf("[SERVER] message : %s\n", rd_st->buffer);
        printf("[SERVER] sending message to all users connected. highest fd is : %d\n", rd_st->reactor->highest_fd);
        // we got some data from a client
        for(int j = 0; j <= rd_st->reactor->highest_fd; j++) {
        // send to everyone!
            if (FD_ISSET(j, &rd_st->reactor->fds)) {
            // except the listener and ourselves
                if (j != rd_st->listener_fd ) {
                    if (send(j, rd_st->buffer, nbytes, 0) == -1) 
                    {
                        perror("send");
                    }
                }
            }
        }
    }
}
/**
 * @brief receiving an address to accept_struct
 * 
 * @param acc_st 
 */
void accept_connection(void * acc_st)
{
    accept_struct * ac_st = (accept_struct *) acc_st;
     // handle new connections
    *ac_st->addrlen = sizeof ac_st->remoteaddr;
    printf("[SERVER] Accepting new connections . . .\n");
    int newfd = accept(ac_st->listener_fd, (struct sockaddr *)ac_st->remoteaddr, ac_st->addrlen);
    if (newfd == -1)
    {
        perror("accept");
    } 
    else 
    {
        FD_SET(newfd, &ac_st->reactor->fds); // add to master set
        ex6::InstallHandler(ac_st->reactor, &handle_client_data, newfd);
        if (newfd > ac_st->reactor->highest_fd) 
        { // keep track of the max
            ac_st->reactor->highest_fd = newfd;
        }
        printf("selectserver: new connection from %s on "
        "socket %d\n",
        inet_ntop(ac_st->remoteaddr->ss_family,
        get_in_addr((struct sockaddr*)ac_st->remoteaddr),ac_st->remoteIP, INET6_ADDRSTRLEN),newfd);
    }
}


int main()
{   
    Reactor * reactor = (Reactor *) ex6::newReactor();
    fd_set read_fds; // temp file descriptor list for select()

    int listener; // listening socket descriptor
    int newfd; // newly accept()ed socket descriptor

    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    char buf[256]; // buffer for client data
    int nbytes;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes=1; // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;
    struct addrinfo hints, *ai, *p;

    // assigning the struct to pass the  function for accepting connections
    accept_struct ac_st;
    ac_st.addrlen = &addrlen;
    ac_st.reactor = reactor;
    ac_st.remoteaddr = &remoteaddr;
    strncpy(ac_st.remoteIP, remoteIP, INET6_ADDRSTRLEN);

    read_struct rd_st;
    rd_st.buffer = buf;
    rd_st.reactor = reactor;
    rd_st.buffersize = sizeof buf;
    

    FD_ZERO(&read_fds);
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    for(p = ai; p != NULL; p = p->ai_next) 
    {
        printf("[SERVER] Creating socket.\n");
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        rd_st.listener_fd = listener;
        ac_st.listener_fd = listener;
        if (listener < 0) 
        {
            continue;
        }
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) 
        {
            close(listener);
            continue;
            return 0;
        }
        break;
    }
    // if we got here, it means we didn't get bound
    if (p == NULL) 
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    freeaddrinfo(ai); // all done with this
    // listen
    if (listen(listener, 10) == -1) 
    {
        perror("listen");
        exit(3);
    }
    // add the listener to the master set
    FD_SET(listener, &reactor->fds);
    ex6::InstallHandler(reactor, &accept_connection, listener);
    // keep track of the biggest file descriptor
    reactor->highest_fd = listener; // so far, it's this one
    // main loop
    for(;;) {
        read_fds = reactor->fds; // copy it
        printf("[SERVER] Waiting for users to be active.\n");
        if (select(reactor->highest_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
        perror("select");
        exit(4);
        }
        // run through the existing connections looking for data to read
        for(i = 0; i <= reactor->highest_fd; i++) {
            if (FD_ISSET(i, &read_fds)) 
            { // we got one!!
                if (i == listener) 
                {
                    printf("[SERVER] Got a new connection from user.\n");
                    // call accept_connection
                   reactor->handlers[i](&ac_st);
                } 
                else 
                {
                    printf("[SERVER] Got message in fd %d.\n", i);
                    rd_st.fd = i;
                    reactor->handlers[i](&rd_st);
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    return 0;
}






