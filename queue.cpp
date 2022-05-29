#include "queue.hpp"
#include "ActiveObject.hpp"

using namespace ex6;

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
    pthread_cond_init(&this->cond, NULL);
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

queue ex6::queue::createQ()
{
    return queue();
}
void ex6::queue::destroyQ()
{
    this->~queue();
}
void ex6::queue::enQ(void * member)
{
    pthread_mutex_lock(&this->lock);
    if(this->size == 0)
    {
        pthread_cond_signal(&this->cond);
    }
    this->size+=1;
    // if head is null
    if(!this->head)
    {
        this->head = new Node(member);
    }
    // find the tail of the list
    Node * curr = this->head;
    while(curr->next != nullptr)
    {
        curr = curr->next;
    }
    // insert
    curr->next = new Node(member);
    
    pthread_mutex_unlock(&this->lock);
}

void * ex6::queue::deQ()
{
    pthread_mutex_lock(&this->lock);  
    while(this->size == 0)
    {
        pthread_cond_wait(&this->cond, &this->lock);
    }
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






