#include <pubq/queue.h>
#include <memory.h>
#include <stdio.h>
#include <Windows.h>
#include <cthreads.h>

typedef struct {
	int task_no;
	queue_t* queue;
} reader_arg;

void* task_reader(void* arg) {
	reader_arg* task_data = (reader_arg*)arg;
	queue_t* queue = task_data->queue;
	while (1) {
		int data = *((int*)queue_dequeue(queue));
		printf("INSIDE TASK (%d): %d\n", task_data->task_no,data);
	}
}

void* task_writer(void* arg) {
	queue_t* queue = (queue_t*)arg;
	for (int i = 0; i < 128; i++) {
		queue_enqueue(queue, &i);
		Sleep(500);
	}
}

void* task_writer_second(void* arg) {
	queue_t* queue = (queue_t*)arg;
	for (int i = 128; i < 1024; i++) {
		queue_enqueue(queue, &i);
		Sleep(100);
	}
}

int main() {
	// tons of memory leaks for testing!
	queue_t* queue = malloc(sizeof(queue_t));
	queue_init(queue);

	int i = 1;
	queue_enqueue(queue, &i);

	struct cthreads_thread twriter;
	struct cthreads_args* argsw = calloc(1, sizeof(struct cthreads_args));
	argsw->data = queue;
	argsw->func = &task_writer;
	cthreads_thread_create(&twriter, NULL, &task_writer, queue, argsw);

	struct cthreads_thread twriter2;
	struct cthreads_args* argsw2 = calloc(1, sizeof(struct cthreads_args));
	argsw->data = queue;
	argsw->func = &task_writer_second;
	cthreads_thread_create(&twriter2, NULL, &task_writer_second, queue, argsw2);

	for (int i = 0; i < 1024; i++) {
		struct cthreads_thread reader;
		struct cthreads_args* args = calloc(1, sizeof(struct cthreads_args));
		reader_arg* reader_data = malloc(sizeof(reader_arg));
		reader_data->queue = queue;
		reader_data->task_no = i;
		args->data = reader_data;
		args->func = &task_reader;
		cthreads_thread_create(&reader, NULL, &task_reader, reader_data, args);
	}

	cthreads_thread_join(twriter, NULL);
	cthreads_thread_join(twriter2, NULL);

	goto CLEANUP;

CLEANUP:
	if (queue != NULL) {
		queue_deinit(queue);
	}
}