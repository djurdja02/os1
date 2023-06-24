//
// Created by os on 8/6/22.
//

#ifndef PROJECT_BASE_V1_1_MEMALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMALLOCATOR_H
#include "../lib/hw.h"
struct FreeMem{
    FreeMem* next;
    FreeMem* prev;
    size_t size;
};
class MemAlloc {
    FreeMem *head=nullptr;
    void tryToJoin(FreeMem* cur);
public:
    static MemAlloc& getInstance();
    void *mem_alloc(size_t sz);
    int mem_free(void* adr);

};
#endif //PROJECT_BASE_V1_1_MEMALLOCATOR_H
