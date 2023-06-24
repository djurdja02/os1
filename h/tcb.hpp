//
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "MemAllocator.h"

// Thread Control Block
class TCB
{
public:
    ~TCB() {

        MemAlloc::getInstance().mem_free(stack);
    }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }


    using Body = void (*)(void*);

    static TCB *createThread(Body body,TCB** handle,void* arg,uint64* stack);

    static TCB *createCPPThread(Body body,TCB** handle,void* arg,uint64* stack);

    static void yield();

    static TCB *running;

private:

    friend class Sem;
    bool blocked;
    bool closed;
    static TCB* createTCB(Body body, void* arg,uint64* stack);
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* fArg;
    uint64 *stack;
    Context context;
    bool finished;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();


};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
