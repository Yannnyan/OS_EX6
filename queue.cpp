#include "queue.hpp"

using namespace ex6;

ex6::queue::Node::Node(void * node_value)
{
    this->current = node_value;
    this->next = nullptr;
}

ex6::queue::queue()
{
    this->head = nullptr;
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
}
void * ex6::queue::deQ()
{
    Node * ret = this->head;
    this->head = this->head->next;
    return ret->current;
}









