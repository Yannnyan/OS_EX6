#include "ActiveObject.hpp"
#include "queue.hpp"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace ex6;


void * ex6::AO::ao_func(void * arg)
{
    arg_functions_queue * funq = (arg_functions_queue *) arg;
    printf("[SERVER] Active object is ready to operate!\n");
    while(true)
    {
        funq->after_func(funq->before_func(funq->q->deQ()));
    }
    return nullptr;
}
args ex6::AO::new_AO(queue * q, void *(*before_func)(void * value), void *(*after_func)(void * value))
{
    active_obj * obj = new active_obj;
    arg_functions_queue * funq = new arg_functions_queue;
    funq->after_func = after_func;
    funq->before_func = before_func;
    funq->q = q;
    pthread_create(&obj->thread, NULL, &ex6::AO::ao_func, funq);
    args arg;
    arg.a_f_q = funq;
    arg.a_o = obj;
    return arg;
}
void ex6::AO::destroy_AO(args arg)
{
    pthread_cancel(arg.a_o->thread);
    arg.a_o->q->destroyQ();
    delete(arg.a_f_q);
    delete(arg.a_o);
}
void * ex6::AO::insert_to_queue(void * caps)
{
    capsula * capsul = (capsula * ) caps;
    capsul->insert_to_queue(caps);
    return nullptr;
}

void * ex6::AO::listenAO(void * caps)
{
    capsula * param = (capsula *) caps;
    queue * q = param->getq();
    int sockfd = param->getsockfd();
    char * buffer;
    while(true)
    {
        buffer = new(char[1024]);
        // receive message
        printf("[SERVER] waiting to read message from %d fd client.\n", sockfd);
        if(recv(sockfd, buffer, 1024, 0) == -1)
        {
            perror("cannot receiver.");
            break;
        }
        // encapsulate
        capsula * capsul = new capsula(q, buffer, sockfd);
        // send it!!
        param->getsq()->enQ(capsul);
    }
    return nullptr; 
}
args ex6::AO::new_listenAO(int sockfd, queue * q)
{
    capsula * caps = new capsula(q, nullptr, sockfd,this->q);
    active_obj * a_o = new active_obj;
    args arg;
    arg.a_o = a_o;
    arg.a_f_q = nullptr;
    pthread_create(&(a_o->thread) ,NULL, &(ex6::AO::listenAO), caps);
    return arg;
}
void * ex6::convert_opposite_case_letters(void * s)
{
    capsula * caps = (capsula * ) s;
    char * str = caps->getbuffer();
    size_t n = strlen(str);
    for(size_t i=0; i< n; i++)
    {
        // small letters
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] -= 32;
        }

        // big letters
        else if(str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] += 32;
        }
    }
    return caps;
}
void * ex6::caesar_cipher_1(void * v)
{
    capsula * caps = (capsula * ) v;
    char * str = caps->getbuffer();
    size_t n = strlen(str);
    for(size_t i=0; i< n; i++)
    {
        // small letters
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            int temp = str[i];
            if(temp == 'z')
            {
                str[i] = 'a';
                continue;
            }
            temp +=1;
            str[i] = (char) temp;
        }
        
        // big letters
        if(str[i] >= 'A' && str[i] <= 'Z')
        {
            int temp = str[i];
            if(temp == 'Z')
            {
                str[i] = 'A';
                continue;
            }
            temp += 1;
            str[i] = (char) temp;
        }
    }
    return caps;
}


/**
 * @brief Construct a new ex6::capsula::capsula object
 *  only copies the pointer to the buffer in to the buffer of the capsula
 * @param q 
 * @param buffer 
 */
ex6::capsula::capsula(queue * q, char * buffer, int sock, queue * q_s) : q(q),  buffer(buffer) , socket(sock), q_send(q_s)
{
}
char * ex6::capsula::getbuffer()
{
    return this->buffer;
}
queue * ex6::capsula::getq()
{
    return this->q;
}
queue * ex6::capsula::getsq()
{
    return this->q_send;
}
void ex6::capsula::setbuffer(char *  buffer)
{
    this->buffer = buffer;
}
void ex6::capsula::insert_to_queue(void * capsula)
{
    this->q->enQ(capsula);
}
int ex6::capsula::getsockfd()
{
    return this->socket;
}


