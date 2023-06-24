//
// Created by os on 8/13/22.
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"
Thread::Thread(void (*body)(void *), void *arg){
    thread_createCPP(&myHandle,body,arg);
}

Thread::~Thread() {
    delete myHandle;
}

void Thread::dispatch() {

    thread_dispatch();
}

int Thread::start() {
   Scheduler::put(myHandle);
   return 0;
}

void Thread::threadwrap(void *thr) {
    Thread* thrd=(Thread*)thr;
    thrd->run();

}

Thread::Thread() {


    thread_createCPP(&myHandle, threadwrap, this);
}

char Console::getc() {
    return __getc();
}

void Console::putc(char cr) {

    __putc(cr);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {

    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}
//

void *operator new(size_t sz) {
    return mem_alloc(sz);
}

void operator delete(void * adr) noexcept{

    mem_free(adr);
}
