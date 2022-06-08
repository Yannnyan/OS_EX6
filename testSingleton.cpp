
#include "Singleton.hpp"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>

using namespace ex6;


int main()
{
    // create the map of the file named file.txt to 10 bytes
    int fd;
    if((fd = open("file.txt", O_CREAT)) == -1)
    {
        perror("cannot open file");
        exit(1);
    }
    void * pointer;
    if((pointer = mmap(NULL, 10, PROT_NONE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }
    Singleton<void *> * singleton = Singleton<void *>::Instance(pointer);
    // some random not equal pointers
    char * pointer1 = (char *) pointer;
    pointer1 += 100;
    int * pointer2 = (int *)pointer;
    pointer2 += 1000;    
    Singleton<void *> * singleton1 = Singleton<void *>::Instance(pointer1);
    Singleton<void *> * singleton2 = Singleton<void *>::Instance(pointer2);
    // crashes if one doesn't work
    assert(singleton == singleton1);
    assert(singleton == singleton2);
    // if both work all the singletons are equal

    // if they all point to the same location then certainly the value of the mapping is the same
    // but we will check it anyways

    assert(singleton->get_object() == singleton1->get_object());
    assert(singleton->get_object() == singleton2->get_object());
    printf("all works, now unlinking and closing file, and unmapping memory.\n");
    munmap(pointer, 10);
    close(fd);
    unlink("file.txt");
}






