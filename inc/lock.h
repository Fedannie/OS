#ifndef __LOCK_H__
#define __LOCK_H__

#include <stdint.h>

#define LOCKED 1
#define UNLOCKED 0

struct spinlock {
	volatile int locked;
};

void lock(struct spinlock*);

void unlock(struct spinlock*);

void init_spinlock(struct spinlock*);

#endif /* __LOCK_H__ */