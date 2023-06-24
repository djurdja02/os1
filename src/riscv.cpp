//
// Created by marko on 20.4.22..
//
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/MemAllocator.h"
#include "../h/Sem.h"
void Riscv::popSppSpie() {

        __asm__ volatile("csrw sepc, ra");
        __asm__ volatile("sret");


}

void Riscv::handleSupervisorTrap() {
    uint64 scauseVar;
    uint64 reg3;
    __asm__ volatile ("mv %[rarg], a3" : [rarg]"=r"(reg3));
    __asm__ volatile("csrr %[ime],scause":[ime] "=r"(scauseVar));

    //sistemski poziv
    if (scauseVar == (0x09) || scauseVar == 0x08)
    {
        uint64 volatile  sepcV;
        __asm__ volatile ("csrr %0, sepc" : "=r" (sepcV));
        uint64 x;
        uint64 volatile sstatus = r_sstatus();
        __asm__ volatile("mv %[rx], a0" : [rx]"=r"(x));
        //malloc
        if (x == 0x01) {
            uint64 ar;
            __asm__ volatile("mv %[ry], a1" : [ry]"=r"(ar));
            //zovi funciju
            void *ret = MemAlloc::getInstance().mem_alloc(ar);
            __asm__ volatile ("mv a0, %[addr]" : : [addr]"r"(ret));

        }//free
        else if (x == 0x02) {
            uint64 tmp2;
            __asm__ volatile("mv %[rz], a1" : [rz]"=r"(tmp2));
            void *argT = (void *) tmp2;
            uint ret = MemAlloc::getInstance().mem_free(argT);
            __asm__ volatile ("mv a0, %[addrt]" : : [addrt]"r"(ret));


        }
        //thread_create
        else if(x==0x11){
            uint64 thandle;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(thandle));
            TCB** handle=(TCB**)thandle;
            uint64 startR;
            __asm__ volatile ("mv %[rs], a2" : [rs]"=r"(startR));
            TCB::Body funct=(TCB::Body)startR;
            void* arg=(void*)reg3;
            uint64* stack=(uint64*)MemAlloc::getInstance().mem_alloc((DEFAULT_STACK_SIZE+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
            TCB::createThread(funct,handle,arg,stack);
            uint64 retVal=0;
            if(handle== nullptr)retVal=-1;
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(retVal));

        }
        //exit
        else if(x==0x12){
            TCB::running->setFinished(true);
            TCB::dispatch();
        }
        //dispatch
        else if(x==0x13){
            TCB::dispatch();

        }
        else if(x==0x14){
            uint64 thandle;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(thandle));
            TCB** handle=(TCB**)thandle;
            uint64 startR;
            __asm__ volatile ("mv %[rs], a2" : [rs]"=r"(startR));
            TCB::Body funct=(TCB::Body)startR;
            void* arg=(void*)reg3;
            uint64* stack=(uint64*)MemAlloc::getInstance().mem_alloc((DEFAULT_STACK_SIZE+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
            TCB::createCPPThread(funct,handle,arg,stack);
            uint64 retVal=0;
            if(handle== nullptr)retVal=-1;
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(retVal));

        }
        //sem open
        else if(x==0x21){
            uint64 h;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(h));
            Sem** handle=(Sem**)h;
            uint64 init;
            __asm__ volatile ("mv %[in], a2" : [in]"=r"(init));
            Sem::openSem(handle,init);
            uint64 ret=0;
            if(handle== nullptr)ret=-1;
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(ret));


        }
        //sem close
        else if(x==0x22){
            uint64 h;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(h));
            Sem* handle=(Sem*)h;
            int ret=Sem::closeSem(handle);
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(ret));
        }
        //sem wait
        else if(x==0x23){
            uint64 h;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(h));
            Sem* handle=(Sem*)h;
            int ret=Sem::semWait(handle);
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(ret));

        }
        //sem signal
        else if(x==0x24){
            uint64 h;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(h));
            Sem* handle=(Sem*)h;
            int ret=Sem::semSignal(handle);
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(ret));
        }
    //change to user mode
        else if(x==0x25){
            w_sstatus(sstatus);
            mc_sstatus(1<<8);
            __asm__ volatile ("csrw sepc, %0" : : "r" (sepcV + 4));
            mc_sip(SIP_SSIP);
            return;
        }//getc
        else if(x==0x41){
            char c=__getc();
            __asm__ volatile ("mv a0, %[rVal]" : : [rVal]"r"(c));
        }
        //putc
        else if(x==0x42){
            uint64 ch;
            __asm__ volatile ("mv %[handle], a1" : [handle]"=r"(ch));
            __putc(ch);
        }
        w_sstatus(sstatus);
        __asm__ volatile ("csrw sepc, %0" : : "r" (sepcV + 4));
        mc_sip(SIP_SSIP);
    }
    else if (scauseVar == 0x8000000000000001UL)
    {

        mc_sip(SIP_SSIP);

    }
    else if (scauseVar== 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }

}
