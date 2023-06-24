//
// Created by os on 8/11/22.
//

#include "../h/Sem.h"
#include "../h/MemAllocator.h"
#include "../h/syscall_c.h"

Sem *Sem::createSem(unsigned int init) {
    Sem* sem=(Sem*)MemAlloc::getInstance().mem_alloc((sizeof(Sem)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    sem->init=init;
    sem->head= nullptr;
    sem->tail=nullptr;
    return sem;
}

void Sem::openSem(Sem **handle, unsigned int init) {
    Sem* ptr = createSem(init);
    *handle= ptr;
}

TCB *Sem::getThread() {
    if(head== nullptr)return nullptr;
    TCB* ret=head->thread;
    Blocked* old=head;
    head=head->next;
    MemAlloc::getInstance().mem_free(old);
    return ret;

}

int Sem::closeSem(Sem *handle) {
    if(!handle)return -1;
    TCB* thread=handle->getThread();
    while(thread!= nullptr){
        thread->blocked=false;
        thread->closed=true;
        Scheduler::put(thread);
        thread=handle->getThread();
    }
 //   int ret=0;
    MemAlloc::getInstance().mem_free(handle);
    handle= nullptr;
    return 0;
}

void Sem::puThread(TCB *thread) {
    Blocked* newEl=(Blocked*)MemAlloc::getInstance().mem_alloc((sizeof(Blocked)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    newEl->thread=thread;
    newEl->next= nullptr;
    if(head==nullptr){
        head=tail=newEl;
    }
    else {
        tail->next=newEl;
        tail=tail->next;
    }

}

int Sem::semWait(Sem *handle) {
    if(handle== nullptr)return -1;
    int in=handle->init;
    handle->init=--in;
    if (in<0) {
        if(TCB::running->closed)return -1;
        handle->puThread(TCB::running);
        TCB::running->blocked=true;
        thread_dispatch();
    }
    return 0;
}

int Sem::semSignal(Sem *handle) {
    if(!handle)return -1;
    handle->init+=1;
    if ((handle->init)<=0) {
        TCB* thread=handle->getThread();
        thread->blocked=false;
        Scheduler::put(thread);
    }return 0;
}
