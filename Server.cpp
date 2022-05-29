/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include "queue.hpp"
#include "ActiveObject.hpp"


#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold

#define BUFFERSIZE 1024



void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void * receive_message(void * sockfd)
{
    char * buffer = (char *)malloc(BUFFERSIZE);
    if (recv((*(int *)sockfd), buffer, BUFFERSIZE, 0) == -1)
        perror("recv");
    return buffer;
}
void * send_message(void * arg)
{
    ex6::AO::queue_member * args = (ex6::AO::queue_member *) arg;
    if(send(args->sockfd, args->str, strlen(args->str) + 1, 0))
    {
        perror("send");
    }
    return nullptr;
}

void * active_object1(void * arg)
{
    ex6::AO::queue_member * args = (ex6::AO::queue_member *) arg;
    char * buffer;
    ex6::AO ao1(args->q1, &receive_message, &ex6::insert_to_queue2);
    ao1.init_thread(args->thread1);
    // should be constantly receiving messages and transfering them to q2
    while(true)
    {
        args->num_active_object = ex6::AO::active_objects::active_obj1;
        args->str = nullptr;
        args->q1->enQ(args);
        ao1.new_AO(args->q1, &receive_message, &ex6::insert_to_queue2);
    }
    return nullptr;
}

void * active_object2(void * arg)
{
    ex6::AO::queue_member * args = (ex6::AO::queue_member *) arg;
    char * buffer;
    ex6::AO ao2(args->q2, &ex6::caesar_cipher_1, &ex6::insert_to_queue3);
    ao2.init_thread(args->thread2);
    while(true)
    {
        ao2.new_AO(args->q2, &ex6::caesar_cipher_1, &ex6::insert_to_queue3);
    }
}

void * active_object3(void * arg)
{
    ex6::AO::queue_member * args = (ex6::AO::queue_member *) arg;
    char * buffer;
    ex6::AO ao3(args->q2, &ex6::convert_opposite_case_letters, &send_message);
    ao3.init_thread(args->thread2);
    while(true)
    {
        ao3.new_AO(args->q2, &ex6::convert_opposite_case_letters, &send_message);
    }
    return nullptr;
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    pthread_t * buffer_ao1;
    pthread_t * buffer_ao2;
    pthread_t * buffer_ao3;
    pthread_t thread_pool[BACKLOG]; // TID array by index, 10 members
    bool active_threads[BACKLOG]; // array of booleans
    //int thread_cursor = 0;
    memset(thread_pool, 0, sizeof(thread_pool)); // zero out all cells
    memset(active_threads, 0, sizeof(active_threads));
    ex6::queue q1 = ex6::queue::createQ();
    ex6::queue q2 = ex6::queue::createQ();
    ex6::queue q3 = ex6::queue::createQ();

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        // this struct is freed later by the third active object
        ex6::AO::queue_member * args = (ex6::AO::queue_member *) new ex6::AO::queue_member();
        args->q1 = &q1;
        args->q2 = &q2;
        args->q3 = &q3;
        args->sockfd = new_fd;
        pthread_create(args->thread1, NULL, &active_object1, &args);
        pthread_create(args->thread2, NULL, &active_object2, &args);
        pthread_create(args->thread3, NULL, &active_object3, &args);
    }

    return 0;
}