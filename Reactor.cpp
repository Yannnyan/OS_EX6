#include "Reactor.hpp"
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void * ex6::newReactor()
{
    Reactor * reac = (Reactor *) malloc(REACTOR_SIZE);
    FD_ZERO(&reac->fds);
    memset(reac->handlers, 0, sizeof(reac->handlers));
    return reac;
}
void ex6::InstallHandler(void * reactor, void (*func)(void *), int fd)
{
    Reactor * reac = (Reactor *) reactor;
    reac->handlers[fd]= func;
    reac->highest_fd = reac->highest_fd < fd ? fd : reac->highest_fd;
    FD_SET(fd, &(reac->fds));
}
void ex6::RemoveHandler(void * reactor, int FD)
{
    Reactor * reac = (Reactor *) reactor;
    reac->handlers[FD] = nullptr;
}








