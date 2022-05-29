#ifndef ACTIVEOBJECT_HPP_
#define ACTIVEOBJECT_HPP_

#include "queue.hpp"
#include <pthread.h>

namespace ex6
{
    class AO
    {
        public:
            enum active_objects{
                active_obj1,
                active_obj2,
                active_obj3
            };
            typedef struct
            {
                /**
                 * @brief each queue holds queue members, these queue members contain all the queues available
                 * and the socket file descriptor for communicating with the specific client
                 * and the message that is constantly being changed as wanted in the exercise.
                 * 
                 */
                // this is the queue of the first active object
                queue * q1;
                // this is the queue of the second active object
                queue * q2;
                // this is the queue of the third active object
                queue * q3;
                // this is the socket file descriptor of the client that the active object is reffering to
                int sockfd;
                // this is the string that is being manipulated
                char * str;
                /**
                 * @brief this int specifies which active object is now in role so we can differentiate in a single function between
                 * each active object.
                 *  0 is the first active object
                 *  1 is the second active object
                 *  2 is the third active object.
                 * 
                 */
                int num_active_object;
                // this is a pointer to the first active object thread
                pthread_t * thread1;
                // this is a pointer to the second active object thread
                pthread_t * thread2;
                // this is a pointer to the third active object thread
                pthread_t * thread3;

            }queue_member;
        private:
            pthread_t * thread;
            queue * q;
            void *(*before_func)(void * value);
            void *(*after_func)(void * value);
        public:
            AO(queue * q, void *(*before_func)(void * value), void *(*after_func)(void * value));
            ~AO();
            /**
             * @brief activates the active object,
             * The active object loops infinitely and enques the output from activating before_func on output form queue and 
             * after_func on the output from before_func.
             * 
             * 
             * @param q queue of the latest active object
             * @param before_func 
             * @param after_func 
             */
            void new_AO(queue * q, void *(*before_func)(void * value), void *(*after_func)(void * value));
            void destroy_AO();
            void init_thread(pthread_t * thread);
    };
    void * convert_opposite_case_letters(void * str);
    void * caesar_cipher_1(void * v);
    void* insert_to_queue1(void * member);
    void* insert_to_queue2(void * member);
    void* insert_to_queue3(void * member);
}




#endif

