#include "Guard.hpp"
#include "doctest.h"
#include <sys/time.h>
#include <time.h>

using namespace ex6;

int glob_var = 0;

void * myfunc(void *t_num)
{
    Guard g = Guard();
    int thread_num = *(int *) t_num;
    if(!glob_var)
    {
        CHECK(thread_num == 1);
        timeval start,end;
        gettimeofday(&start, NULL);
        gettimeofday(&end, NULL);
        while(end.tv_sec - start.tv_sec < 5)
        {
            gettimeofday(&end, NULL);
        }
        glob_var = 1;
    }
    else
    {
        CHECK(thread_num == 2);
        
    }
    return nullptr;
}
/**
 * @brief this test case takes two threads, and creates the first one with a pointer to the myfunc function,
 * there this thread should use guard and lock the function the same thread should be stuck in a loop for 5 seconds,
 * the main thread meanwhile waits two seconds before duplicating into another thread that goes into the same
 * function and locks itself using guard mechanism. The argument sent to the function is the thread number.
 * so we could check if the thread entered is really the thread that should enter.
 * if this test case wouldn't have worked then our guard functionallity is bad. This test provides enough information
 * about the guard mechanism.
 */
TEST_CASE("Guard functionallity")
{
    pthread_t thread1, thread2;
    int * ptr1 = new int, *ptr2 = new int;
    *ptr1 = 1;
    *ptr2 = 2;
    pthread_create(&thread1, NULL, &myfunc, ptr1);
    timeval start, end;
    gettimeofday(&start, NULL);
    end = start;
    while(end.tv_sec - start.tv_sec < 2)
    {
        gettimeofday(&end, NULL);
    }
    pthread_create(&thread2, NULL, &myfunc, ptr2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);


}


