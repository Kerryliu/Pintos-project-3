#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <stdbool.h>
#include <stdint.h>
#include <list.h>
#include "threads/thread.h"
#include "threads/synch.h"

struct lock frameTableLock;
struct list frameTable;

struct frameEntry {
	void *frame;
	tid_t tid;
	uint32_t *pte;
	struct list_elem elem;
};

void initFrameTable(void);
void* allocateFrame(enum palloc_flags flags);
void addFrameToTable(void *frame);
void freeFrame(void *frame);
bool evictFrame(void *frame);

#endif
