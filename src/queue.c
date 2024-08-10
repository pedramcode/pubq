#include <pubq/queue.h>

void queue_init(queue_t* queue) {
	queue->head = NULL;
	queue->tail = NULL;
	cthreads_mutex_init(&queue->mutex, NULL);
	cthreads_cond_init(&queue->cond, NULL);
}

void queue_deinit(queue_t* queue) {
	cthreads_mutex_destroy(&queue->mutex);
	cthreads_cond_destroy(&queue->cond);
}

void queue_enqueue(queue_t* queue, void* data) {
	if (queue == NULL) {
		// TODO panic
		return;
	}
	cthreads_mutex_lock(&queue->mutex);
	node_t* node = calloc(1, sizeof(node_t));
	if (node == NULL) {
		// TODO panic
		cthreads_mutex_unlock(&queue->mutex);
		return;
	}
	node->data = data;
	if (queue->tail == NULL) {
		queue->head = node;
	}
	else {
		queue->tail->next = node;
	}
	queue->tail = node;
	cthreads_cond_broadcast(&queue->cond);
	cthreads_mutex_unlock(&queue->mutex);
}

void* queue_dequeue(queue_t* queue) {
	cthreads_mutex_lock(&queue->mutex);
	if (queue == NULL) {
		// TODO panic
		return;
	}
	while (queue->head == NULL) {
		cthreads_cond_wait(&queue->cond, &queue->mutex);
	}
	node_t* head = queue->head;
	queue->head = head->next;
	if (queue->head == NULL) {
		queue->tail = NULL;
	}
	void* data = head->data;
	cthreads_mutex_unlock(&queue->mutex);
	free(head);
	return data;
}