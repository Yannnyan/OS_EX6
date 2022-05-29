#include "ActiveObject.hpp"
#include "queue.hpp"
#include <pthread.h>
#include <string.h>

using namespace ex6;

ex6::AO::AO(queue * q, void *(*before_func)(void * value), void *(*after_func)(void * value))
{
    this->q = q;
    this->before_func = before_func;
    this->after_func = after_func;

}
ex6::AO::~AO()
{
    // insert pthread cancel for threads
    // pthread_cancel()
    this->q->~queue();

}

void* insert_to_queue1(void * member)
{
    ex6::AO::queue_member * mem = (ex6::AO::queue_member *) member;
    mem->q1->enQ(mem);
    return nullptr;

}
void* insert_to_queue2(void * member)
{
    ex6::AO::queue_member * mem = (ex6::AO::queue_member *) member;
    mem->q2->enQ(mem);
    return nullptr;
}
void* insert_to_queue3(void * member)
{
    ex6::AO::queue_member * mem = (ex6::AO::queue_member *) member;
    mem->q3->enQ(mem);
    return nullptr;

}


void ex6::AO::new_AO(queue * q, void *(before_func)(void * value), void *(after_func)(void * value))
{
    char * buffer;
    void * value = q->deQ();
    queue_member * mem = (queue_member *) value;
    switch(mem->num_active_object)
    {
        case 0:
            // this should receive a message from sockfd
            buffer = (char *)before_func((void *)&mem->sockfd);
            mem->str = buffer;
            mem->num_active_object = active_objects::active_obj2;
            // this should enQ mem into q2
            after_func(mem);
            break;
        case 1:
            // this should ciper the message as wanted
            buffer = (char *)before_func((void*)mem->str);
            mem->str = buffer;
            mem->num_active_object = active_objects::active_obj3;
            // this should enQ mem into q3
            after_func(mem);
            break;
        case 2:
            // this should convert letter case
            buffer = (char *)before_func(mem->str);
            mem->str = buffer;
            // this should send the result mem->str back to the client.
            after_func(mem);
            delete(mem);
    }
    
}
void ex6::AO::destroy_AO()
{
    this->~AO();
}

void ex6::AO::init_thread(pthread_t * thread)
{
    this->thread = thread;
}
void * convert_opposite_case_letters(char * str)
{
    size_t n = strlen(str);
    for(int i=0; i< n; i++)
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
    return str;
}
void * caesar_cipher_1(void * v)
{
    char * str = (char *) v;
    size_t n = strlen(str);
    for(int i=0; i< n; i++)
    {
        // small letters
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] -= 98;
            str[i] = str[i] % 26;
            str[i] += 97;
        }
        
        // big letters
        else if(str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] -= 66;
            str[i] = str[i] % 26;
            str[i] += 65;
        }
    }
    return str;
}



