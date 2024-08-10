#ifndef ZQUEUE_QUEUE_H
#define ZQUEUE_QUEUE_H

#include <cthreads.h>

typedef struct node_t {
	void* data;
	struct node_t* next;
} node_t;

typedef struct {
	node_t* head;
	node_t* tail;
	struct cthreads_mutex mutex;
	struct cthreads_cond cond;
} queue_t;

void queue_init(queue_t* queue);
void queue_deinit(queue_t* queue);
void queue_enqueue(queue_t* queue, void* data);
void* queue_dequeue(queue_t* queue);

#endif // ZQUEUE_QUEUE_H