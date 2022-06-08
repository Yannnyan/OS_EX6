#ifndef GUARD_HPP_
#define GUARD_HPP_

#include <pthread.h>


namespace ex6
{
        /**
     * @brief This guard can be used to protect strtok.
     * The conplexity that I found when addressing this question is that strtok returns
     * a value and therefore we cannot use a simple lock mechanism to free just before the return statement
     * so rather if we use a Guard design pattern we can actually unlock the mutex after the return statement
     * because the destructor which unlocks the mutex is being called only after we exit the scope of the function
     * and we exit the scope of the function only after the return statement.
     * 
     * 
     */
    class Guard
    {
        private:
            // lock for the guard
            static pthread_mutex_t lock;
        public:
                /**
         * @brief Construct a new ex6::Guard::Guard object
         * 
         */
            Guard();
            /**
             * @brief Destroy the Guard object
             * 
             */
            ~Guard();
    };
}


#endif





