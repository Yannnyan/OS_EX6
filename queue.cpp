#include "queue.hpp"
#include "ActiveObject.hpp"
#include <stdio.h>

using namespace ex6;

// pthread_cond_t ex6::queue::cond;

ex6::queue::Node::Node(void * node_value)
{
    this->current = node_value;
    this->next = nullptr;
}

ex6::queue::queue()
{
    this->head = nullptr;
    this->lock = PTHREAD_MUTEX_INITIALIZER;
    this->size = 0;
    pthread_cond_init(&cond, NULL);
}
ex6::queue::~queue()
{
    Node * cur = this->head;
    Node * prev = this->head;
    while(cur != nullptr)
    {
        cur = cur->next;
        delete(prev);
        prev = cur;
    }
}

queue * ex6::queue::createQ()
{
    return new queue();
}
void ex6::queue::destroyQ()
{
    this->~queue();
}
void ex6::queue::enQ(void * member)
{
    pthread_mutex_lock(&this->lock);
    // if head is null
    this->size+=1;
    if(this->head==nullptr)
    {
        this->head = new Node(member);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&this->lock);
        return;
    }
    // find the tail of the list
    Node * curr = this->head;
    while(curr->next != nullptr)
    {
        curr = curr->next;
    }
    // insert
    curr->next = new Node(member);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&this->lock);
}

void * ex6::queue::deQ()
{
    pthread_mutex_lock(&this->lock);
    while(this->size == 0 || !this->head)
    {
        pthread_cond_wait(&cond, &this->lock);
    }
    // pthread_mutex_lock(&this->lock);
    void * ret = this->private_deQ();
    pthread_mutex_unlock(&this->lock);
    return ret;
}
void * ex6::queue::private_deQ()
{
    Node * ret = this->head;
    this->head = this->head->next;
    return ret->current;   
}


void * ex6::queue::top()
{
    return this->head->current;
}


size_t ex6::queue::Size()
{
    return this->size;
}

