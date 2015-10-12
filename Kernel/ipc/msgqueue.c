#include <stdint.h>
#include "msgqueue.h"
#include "string.h"
#include "mem.h"

typedef struct message message_t;

struct message {
	message_t *next;
	void *msg;
	int size;
};

typedef struct message message_t;

struct msgqueue {
	message_t *first;
	message_t *last;
	uint32_t size;
	uint32_t maxsize;
};

static void message_delete(message_t * message) {
	free(message->msg);
	free(message);
}

static void message_deep_delete(message_t * message) {
	if (message->next != NULL) {
		message_deep_delete(message->next);
	}

	message_delete(message);
}

msgqueue_t* msgqueue_create(uint32_t maxsize) {
	msgqueue_t *msgqueue = malloc(sizeof(msgqueue_t));

	msgqueue->first = NULL;
	msgqueue->last = NULL;
	msgqueue->size = 0;
	msgqueue->maxsize = maxsize;

	return msgqueue;
}

void msgqueue_add(msgqueue_t *msgqueue, void* msg, int size) {

	if(msgqueue->size == msgqueue->maxsize){
		return;
	}

	message_t *message = malloc(sizeof(message_t));

	message->size = size;
	message->msg = malloc(size);
	memcpy(message->msg, msg, size);

	message->next = NULL;

	if (msgqueue->first == NULL) {
		msgqueue->first = msgqueue->last = message;
	} else {
		msgqueue->last->next = message;
		msgqueue->last = message;
	}

	msgqueue->size++;
}

void* msgqueue_deq(msgqueue_t *msgqueue) {

	void* msg;
	message_t *message;

	while(msgqueue->first == NULL);

	message = msgqueue->first;

	msg = malloc(message->size);
	memcpy(msg, message->msg, message->size);

	msgqueue->first = message->next;

	message_delete(message);

	msgqueue->size--;

	return msg;
}

void* msgqueue_peek(msgqueue_t *msgqueue) {

	void* msg;
	message_t *message;

	if(msgqueue->first==NULL){
		return NULL;
	}

	message = msgqueue->first;

	msg = malloc(message->size);
	memcpy(msg, message->msg, message->size);

	return msg;
}

bool msgqueue_isempty(msgqueue_t *msgqueue) {
	return (msgqueue->first == NULL);
}

int msgqueue_size(msgqueue_t msgqueue){
	return msgqueue->size;
}

void msgqueue_delete(msgqueue_t *msgqueue) {

	if (msgqueue->first != NULL) {
		message_deep_delete(msgqueue->first);
	}

	free(msgqueue);
}

