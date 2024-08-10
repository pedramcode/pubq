# PubQ

Thread-safe, simple, fast and cross-platform queue data structure

## Third-party

* CThreads: a cross-platform thread library [view page on github](https://github.com/PerformanC/CThreads/)

## Usage

```c
#include <pubq/queue.h>
#include <memory.h>

int main(){
	// make queue_t instance
	queue_t* queue = malloc(sizeof(queue_t));
	queue_init(queue); // initialize

	for(int i = 0 ; i < 10 ; i++){
		// enqueue data (void*) into queue
		// it has built-in thread-safety features (condition variable and mutex)
		queue_enqueue(queue, &i);
	}

	for(int i = 0 ; i < 10 ; i++){
		// dequeue data (void*) from queue
		// if queue being empty, the process will wait unitl a new item get pushed into queue
		void* data = *queue_dequeue(queue);
	}

	// memory deallocation
	queue_deinit(queue) // free queue
	return 0;
}
```
