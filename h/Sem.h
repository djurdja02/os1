//
// Created by os on 8/11/22.
//

#ifndef VASKOZABATA_SEM_H
#define VASKOZABATA_SEM_H
#include "tcb.hpp"
struct Blocked{
    TCB* thread;
    Blocked* next=nullptr;
};

class Sem {
    int init;

    friend class Riscv;

    Blocked* head;
    Blocked* tail;

    TCB* getThread();

    void puThread(TCB* thread);

    static Sem* createSem(unsigned init);
public:
    static void openSem(Sem** handle,unsigned init);

    static int closeSem(Sem* handle);

    static int semWait(Sem* handle);

    static int semSignal(Sem* handle);



};


#endif //VASKOZABATA_SEM_H
