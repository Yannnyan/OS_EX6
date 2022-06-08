#ifndef ACTIVEOBJECT_HPP_
#define ACTIVEOBJECT_HPP_

#include "queue.hpp"
#include <pthread.h>

namespace ex6
{
    typedef struct 
    {
    pthread_t thread;
    queue * q;
    }active_obj;
    typedef struct
    {
        queue * q;
        void *(*before_func)(void * value);
        void *(*after_func)(void * value);
    } arg_functions_queue;
    typedef struct
    {
        active_obj * a_o;
        arg_functions_queue * a_f_q;
    }args;
    void * convert_opposite_case_letters(void * str);
    void * caesar_cipher_1(void * v);
    class AO
    {
        private:
            queue * const q;
        public:
            AO(queue * que) : q(que){}
            args new_AO(queue * q, void *(*before_func)(void * value), void *(*after_func)(void * value));
            args new_listenAO(int sockfd, queue * q);
            static void * listenAO(void * caps);
            void destroy_AO(args arg);
            static void * insert_to_queue(void * member);
            static void * ao_func(void * arg);
    };
    class capsula
    {
        private:
            queue * q;
            char * buffer;
            int socket;
            queue * q_send;
        
        public:
            capsula(queue * q, char * buffer = nullptr, int sock = -1, queue * q_s = nullptr);
            char * getbuffer();
            int getsockfd();
            queue * getq();
            queue * getsq();
            void setbuffer(char *  buffer);
            void insert_to_queue(void * capsul);

    };
    
    
}




#endif

