#include "Reactor.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define PORT 8081


using namespace ex6;
typedef struct 
{
    Reactor * reactor;
    int sockfd;
    char * buffer;
    int buffersize;
} send_struct;

typedef struct
{
    Reactor * reactor;
    int sockfd;
    char * buffer;
    int buffersize;
} read_struct;
// this fucntion just receives messages from stdin and sends it to the server
void *handle_user(void * send_st)
{
    send_struct * sd_st = (send_struct *) send_st;
    char buffer[1024];
    while(true)
    {
        memset(buffer, 0, sizeof buffer);
        scanf("%1024[^\n]", buffer);
        if(send(sd_st->sockfd, buffer, sizeof buffer, 0) == -1)
        {
            perror("send");
            exit(1);
        }
    }
    return nullptr;
}
// this function just reads a message from the server if it can
void * read_message(void * read_st)
{
    read_struct * rd_st = (read_struct *) read_st;
    fd_set master;
    fd_set current;
    FD_ZERO(&master);
    FD_SET(rd_st->sockfd, &master);
    int highestfd = rd_st->sockfd;
    while(true)
    {
        current = master;
        if(select(highestfd + 1, &current, NULL, NULL, NULL))
        {
            perror("select failed");
            exit(1);
        }
        // this is the socket that receives information from the server
        if(FD_ISSET(rd_st->sockfd, &current))
        {
            memset(rd_st->buffer, 0, rd_st->buffersize);
            if(read(rd_st->sockfd, rd_st->buffer, rd_st->buffersize))
            {
                perror("read");
                exit(1);
            }
            printf("[CLIENT] Recieved a new message: %s\n", rd_st->buffer);
        }
        
    }
    return nullptr; 
}


int main()
{
    // set up the socket.
    int fd;
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_address;
    int opt = 1;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    // set the server addr attribute to localhost
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        printf("cannot assign to localhost.\n");
        exit(1);
    }
    int client_listenerfd;
    if((client_listenerfd = connect(fd, (struct sockaddr *)&server_address, sizeof (server_address)) ) == -1)
    {
        printf("cannot connect client\n");
        exit(1);
    }
    read_struct rd_st;
    send_struct sd_st;
    char buffer[1024];
    rd_st.buffer = buffer;
    rd_st.buffersize = sizeof(buffer);
    rd_st.sockfd = client_listenerfd;
    
    sd_st.buffer = buffer;
    sd_st.buffersize = 1024;
    sd_st.sockfd = client_listenerfd;
    pthread_t thread1;
    pthread_create(&thread1, NULL, &read_message, &rd_st);
    handle_user(&sd_st);
    return 0;
}





