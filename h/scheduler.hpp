//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_SCHEDULER_HPP


class TCB;
struct Elem
{
    TCB *data;
    Elem *next;

};

class Scheduler
{
private:
    static Elem* head;
    static Elem* tail;
public:
    static TCB *get();

    static void put(TCB *ccb);

};


#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_SCHEDULER_HPP
