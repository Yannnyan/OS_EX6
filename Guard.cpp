#include "Guard.hpp"
#include <stdio.h>

pthread_mutex_t ex6::Guard::lock = PTHREAD_MUTEX_INITIALIZER;

ex6::Guard::Guard()
{
    pthread_mutex_lock(&this->lock);
    // printf("in ");
}


ex6::Guard::~Guard()
{
    // printf("out ");
    pthread_mutex_unlock(&this->lock);
}





