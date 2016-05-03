#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "vm/frame.h"

void initFrameTable(void) {
	list_init(&frameTable);
	lock_init(&frameTableLock);
}

void* allocateFrame(enum palloc_flags flags) {
	if((flags & PAL_USER) == 0) {
		return NULL;
	}
	void *frame = palloc_get_page(flags);
	if(frame) {
		addFrameToTable(frame);
	} else if(!evictFrame(frame)) {
		PANIC ("Shit man, the swap is full.");
	}
	return frame;
}

void addFrameToTable(void *frame) {
	struct frameEntry *fte = malloc(sizeof(struct frameEntry));
	fte->frame = frame;
	fte->tid = thread_tid();
	lock_acquire(&frameTableLock);
	list_push_back(&frameTable, &fte->elem);
	lock_release(&frameTableLock);
}

void freeFrame(void *frame) {
	struct list_elem *e;
	lock_acquire(&frameTableLock);
	for(e = list_begin(&frameTable); e != list_end(&frameTable); e = list_next(e)) {
		struct frameEntry *fte = list_entry(e, struct frameEntry, elem);
		if(fte->frame == frame) {
			list_remove(e);
			free(fte);
			break;
		}
	}
	lock_release(&frameTableLock);
	palloc_free_page(frame);
}

bool evictFrame(void *frame) {
	return false;
}
