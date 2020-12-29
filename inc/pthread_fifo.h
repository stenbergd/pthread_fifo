/**
 * @brief	Header file of thread-safe FIFO queue using POSIX pthread and semaphore
 */

#ifndef _PTHREAD_FIFO_H_
#define _PTHREAD_FIFO_H_

/* Includes -------------------------------------------------*/
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

/* Exported types -------------------------------------------*/

typedef struct pthread_fifo
{
	pthread_mutex_t mutex; // Internal mutex
	sem_t sem; // Internal semaphore
	uint8_t *buffer; // Internal pointer to allocated queue item data
	unsigned int front; // Index to first inserted item in data
	unsigned int back; // Index to last inserted item in data
	size_t buf_item_len; // Buffer length in number of items
	size_t buf_max_item_len; // Maximum buffer length in number of items
	size_t item_size; // Size of each item in queue
} pthread_fifo_t;

/* Exported function prototypes -----------------------------*/
int pthread_fifo_create(pthread_fifo_t *queue, size_t queue_size, size_t msg_size);
int pthread_fifo_enqueue(pthread_fifo_t *queue, uint8_t *data);
int pthread_fifo_dequeue(pthread_fifo_t *queue, uint8_t *data, const struct timespec *timeout);
int pthread_fifo_destroy(pthread_fifo_t *queue);

#endif // _PTHREAD_FIFO_H_
