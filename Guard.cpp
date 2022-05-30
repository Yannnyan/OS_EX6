#include "Guard.hpp"



ex6::Guard::Guard()
{
    if(!initialized)
    {
        initialized = true;
        this->lock = PTHREAD_MUTEX_INITIALIZER;
    }
    pthread_mutex_lock(&this->lock);
}


ex6::Guard::~Guard()
{
    pthread_mutex_unlock(&this->lock);
}





