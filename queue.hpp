#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <pthread.h>


namespace ex6
{
    class queue
    {
        private:
            class Node
            {
                public:
                    /**
                     * @brief Construct a new Node object
                     * 
                     * @param node_value  is the value of the node
                     * next will be set to nullptr
                     */
                    Node(void * node_value);
                    void * current;
                    Node * next;
                    
            };
            Node * head;
            pthread_mutex_t lock;
            pthread_cond_t cond;
            size_t size;

            void * private_deQ();
        public:
            /**
             * @brief Construct a new queue object
             * 
             */
            queue();
            /**
             * @brief Destroy the queue object
             * 
             */
            ~queue();
            /**
             * @brief construct and return a new queue
             * 
             * @return queue 
             */
            static queue createQ();
            /**
             * @brief destroy the current queue
             * 
             */
            void destroyQ();
            /**
             * @brief add a member to the queue
             * 
             * @param member 
             */
            void enQ(void * member);
            /**
             * @brief remove a member from the top of the queue
             * 
             * @return void* 
             */
            void * deQ();

    };
}








#endif