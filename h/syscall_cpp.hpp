//
// Created by os on 8/13/22.
//

#ifndef VASKOZABATA_SYSCALL_CPP_HPP
#define VASKOZABATA_SYSCALL_CPP_HPP
#include "syscall_c.h"
void* operator new (size_t);
void operator delete (void*)noexcept;
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    //static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:

    thread_t myHandle;
    static void threadwrap(void* thr);
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class Console {
public:
    static char getc ();
    static void putc (char cr);
};

#endif //VASKOZABATA_SYSCALL_CPP_HPP
