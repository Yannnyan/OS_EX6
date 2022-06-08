#include "queue.hpp"
#include "doctest.h"
#include <stdio.h>

using namespace ex6;


/**
 * @brief Testing the funtionality of the queue
 * 
 */
TEST_CASE("INSERT_INTO_QUEUE")
{
    queue * q = queue::createQ();
    int mem1 = 5;
    int mem2 = 10;
    int mem3 = 15;
    int mem4 = 20;
    q->enQ(&mem1);
    CHECK(q->top() == &mem1);
    q->enQ(&mem2);
    CHECK(q->top() == &mem1);
    q->enQ(&mem3);
    CHECK(q->top() == &mem1);
    q->enQ(&mem4);
    CHECK(q->top() == &mem1);
    CHECK(q->Size() == 4);
}

TEST_CASE("DEQUEUE_FROM_QUEUE")
{
    queue * q = queue::createQ();
    int mem1 = 5;
    int mem2 = 10;
    int mem3 = 15;
    int mem4 = 20;
    q->enQ(&mem1);
    q->enQ(&mem2);
    q->enQ(&mem3);
    q->enQ(&mem4);
    // printf("%d ", *(int *)q->deQ());
    // printf("%d ", *(int *)q->deQ());
    // printf("%d ", *(int *)q->deQ());
    // printf("%d ", *(int *)q->deQ());
    CHECK(&mem1 ==  q->deQ());
    CHECK(&mem2 ==  q->deQ());
    CHECK(&mem3 ==  q->deQ());
    CHECK(&mem4 ==  q->deQ());



}

TEST_CASE("BOTH_CASES")
{
    queue * q = queue::createQ();
    int mem1 = 5;
    int mem2 = 10;
    int mem3 = 15;
    q->enQ(&mem3);
    CHECK(&mem3 == q->deQ());
    q->enQ(&mem2);
    q->enQ(&mem1);
    CHECK(&mem2 == q->deQ());
    CHECK(&mem1 == q->deQ());
}

void * threadfunc1(void * q)
{
    queue * myq = (queue *) q;
    int * a = new int;
    *a = 10;
    int * b = new int;
    *b = 11;
    int * c = new int;
    *c = 12;
    int * d = new int;
    *d = 13;
    myq->enQ(a);
    myq->enQ(b);
    myq->enQ(c);
    myq->enQ(d); 
    return nullptr;
}

void * threadfunc2(void * q)
{
    queue * myq = (queue *) q;
    int * a, *b, *c, *d;
    a = (int *)myq->deQ();
    b = (int *)myq->deQ();
    c = (int *)myq->deQ();
    d = (int *)myq->deQ();
    CHECK(*a == 10);
    delete(a);
    CHECK(*b == 11);
    delete(b);
    CHECK(*c == 12);
    delete(c);
    CHECK(*d == 13);
    delete(d);
    return nullptr;
}
TEST_CASE("Multi_Threading")
{
    queue * q = queue::createQ();
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread2, NULL, &threadfunc2, q);
    pthread_create(&thread1, NULL, &threadfunc1, q);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}


