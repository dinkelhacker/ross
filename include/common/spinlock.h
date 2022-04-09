#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdint.h>

typedef struct spinlock {
	volatile uint32_t lock;
} spinlock_t;

extern void spin_lock(spinlock_t *lock);
extern void spin_unlock(spinlock_t *lock);

#endif
