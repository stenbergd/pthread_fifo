/**
 * @brief	pthread_fifo Example 1
 */

/* Includes -------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <pthread_fifo.h>
#include <unistd.h>

/* Private defines ------------------------------------------*/
#define QUEUE_LEN	10

void *producer(void *arg)
{
	int status = 0;
	uint8_t nbr = 0;
	pthread_fifo_t *queue = NULL;

	queue = (pthread_fifo_t *) arg;

	while (1) {
		printf("[Producer] Sending: %u\n", nbr);

		status = pthread_fifo_enqueue(queue, &nbr);
		if (0 != status) {
			fprintf(stderr, "[Producer] Failed to send to queue\n");
		}

		nbr ++;

		sleep(1);
	}
}

void *consumer(void *arg)
{
	int status = 0;
	uint8_t nbr;
	pthread_fifo_t *queue = NULL;

	queue = (pthread_fifo_t *) arg;

	while (1) {
		status = pthread_fifo_dequeue(queue, &nbr, NULL);
		if (0 != status) {
			fprintf(stderr, "[Consumer] Failed to receive from queue\n");
		} else {
			printf("[Consumer] Received: %u\n", nbr);
		}
	}
}

int main(int argc, char *argv[])
{
	int status = 0;

	pthread_t producer_thread;
	pthread_t consumer_thread;
	pthread_attr_t thread_attr;

	pthread_fifo_t queue;

	status = pthread_attr_init(&thread_attr);
	if (0 != status) {
		fprintf(stderr, "Failed to init thread attributes\n");
		goto out;
	}

	status = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
	if (0 != status) {
		fprintf(stderr, "Failed to set thread detach statte attributes\n");
		goto out;
	}

	status = pthread_fifo_create(&queue, QUEUE_LEN, sizeof(uint8_t));
	if (0 != status) {
		fprintf(stderr, "Failed to create queue\n");
		goto out;
	}

	status = pthread_create(&producer_thread, &thread_attr, producer, (void *) &queue);
	if (0 != status) {
		fprintf(stderr, "Failed to create producer thread\n");
		goto out;
	}

	status = pthread_create(&consumer_thread, &thread_attr, consumer, (void *) &queue);
	if (0 != status) {
		fprintf(stderr, "Failed to create consumer thread\n");
		goto out;
	}

	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);

out:
	status = pthread_fifo_destroy(&queue);
	if (0 != status) {
		fprintf(stderr, "Failed to free queue\n");
	}

	status = pthread_attr_destroy(&thread_attr);
	if (0 != status) {
		fprintf(stderr, "Failed to destroy thread attributes\n");
	}

	return status;
}
