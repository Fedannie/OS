#include "lock.h"
#include "idt_init.h"
#include "allocator.h"
#include "print.h"
#include "desc.h"
#include "ioport.h"

void lock(struct spinlock *excl) {
    while (1) {
        disable_ints();
        if (excl->locked == UNLOCKED) {
            excl->locked = LOCKED;
            enable_ints();
            return;
        }
        enable_ints();
    }
}

void unlock(struct spinlock *excl) {
    excl->locked = UNLOCKED;
}

void init_spinlock(struct spinlock *excl) {
    excl->locked = UNLOCKED;
}