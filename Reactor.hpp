
#include <sys/select.h>

#define MAX_FDS 64

#define REACTOR_SIZE sizeof(ex6::Reactor)

namespace ex6
{
    struct Reactor
    {
        void (*handlers[MAX_FDS])(void *);
        fd_set fds;
        int highest_fd;

    };
    /**
     * @brief constructs a new reactor 
     * 
     * @return void* return a pointer to where it's located
     */
    void * newReactor();
    /**
     * @brief Installs a new handle function at the specified fd 
     * 
     * @param reactor the struct of the Reactor
     * @param func the function desired to install
     * @param fd the file descriptor to which func corresponds to
     */
    void InstallHandler(void * reactor, void (*func)(void *), int fd);
    /**
     * @brief Removes a handler at the specific fd 
     * 
     * @param reactor 
     */
    void RemoveHandler(void * reactor, int FD);
    //         void startReactor();

    // };
}







