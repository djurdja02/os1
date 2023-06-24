//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/MemAllocator.h"
#include "../h/syscall_c.h"
TCB* TCB::running=nullptr;
TCB* TCB::createThread(TCB::Body body, TCB **handle, void *arg,uint64* stack) {
    TCB* ptr = createTCB(body,arg,stack);
    *handle= ptr;
    if (body != nullptr) { Scheduler::put(*handle);}
    return ptr;
}

TCB *TCB::createCPPThread(TCB::Body body, TCB **handle, void *arg, uint64 *stack) {
    TCB* ptr = createTCB(body,arg,stack);
    *handle= ptr;
    return ptr;
}


TCB *TCB::createTCB(TCB::Body body, void *arg, uint64 *stack) {
    TCB* newT=(TCB*)MemAlloc::getInstance().mem_alloc((sizeof(TCB)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    newT->body=body;
    if(body!= nullptr)newT->stack=stack;
    else stack= nullptr;
    newT->context={(uint64) &threadWrapper,stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0};
    newT->fArg=arg;
    newT->finished=false;
    newT->blocked=false;
    newT->closed=false;

    return newT;
}

void TCB::yield()
{
    __asm__ volatile("li a0,0x13");
    __asm__ volatile ("ecall");
    return;
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->blocked) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}


void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->fArg);
    running->setFinished(true);
    thread_dispatch();
}
