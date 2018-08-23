#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Queue - queue structure
 * @buf: queue buffer
 * @read: position of read
 * @write: position of write
 * @size: buffer size
 * @space: writable buffer size
 */
typedef struct {
    u8 *buf;
    unsigned int read;
    unsigned int write;
    unsigned int size;
    unsigned int space;
} Queue;

#define Avail(q) (q->size - q->space)

#define Queue_len 1000
u8 Queue_re[Queue_len];
Queue q1;
/**
 * Queue_Init - init a queue
 * @q: pointer of queue
 * @size: size of buffer in queue
 *
 * Must be called when started. 
 */
void Queue_Init(Queue *q, int size)
{
    q->read = 0;
    q->write = 0;
    q->size = size;
    q->space = size;
    q->buf = (u8 *)Queue_re;
}

/**
 * Queue_Destroy - destroy a queue
 * @q: pointer of queue
 */
void Queue_Destroy(Queue *q)
{
    free(q->buf);
}

/**
 * Queue_Empty - tests whether a queue is empty
 * @q: the queue to test
 */
bool Queue_Empty(Queue *q) 
{
    return (q->space == q->size);
}

/**
 * Queue_Full - tests whether a queue is full
 * @q: the queue to test
 */
bool Queue_Full(Queue *q)
{
    return (q->space == 0);
}

/**
 * AddQueue - add a byte to queue
 * @q: the queue to add to
 * @val: the char to add
 */
bool AddQueue(Queue *q, char val)
{
    if (!Queue_Full(q)) {
        q->buf[q->write] = val;
        q->write = (q->write + 1) % q->size;
        q->space--;
        return true;
    } 
    return false;
}

/**
 * DelQueue - delete a byte from queue
 * @q: the queue to delete from
 * @val: the char deleted
 */
bool DelQueue(Queue *q, char *val)
{
    if (!Queue_Empty(q)) {
        *val = q->buf[q->read];
        q->read = (q->read + 1) % q->size;
        q->space++;
        return true;
    }
    return false;
}

/**
 * WriteQueue - write buffers to queue
 * @q: the queue to write in
 * @buf: pointer of write buffer
 * @len: length of write buffer
 */
static int WriteQueue(Queue *q, char *buf, unsigned int len)
{
    unsigned int ret = 0;
    unsigned int rest = q->size - q->write;

    if (!Queue_Full(q)) {
        if (q->space >= len) {
            ret = len;
            if (rest >= len) {
				memcpy((q->buf+q->write), buf, len);
				q->write = (q->write + len) % q->size;
                q->space -= len;

            } else {
                memcpy(q->buf + q->write, buf, rest);
                q->write = 0;
                memcpy(q->buf, buf + rest, len - rest);
                q->write = len -rest;
                q->space -= len;
            }
        } else {
            ret = q->space;
            if (rest >= q->space) {
                memcpy((q->buf + q->write), buf, q->space);
                q->write = (q->write + q->space) % q->size;
                q->space = 0;
            } else {
                memcpy(q->buf + q->write, buf, rest);
                q->write = 0;
                memcpy(q->buf, buf + rest, q->space - rest);
                q->write = q->space -rest;
                q->space = 0;
            }
        }   
    } 
    return ret;
}

/**
 * ReadQueue - read buffers from queue
 * @q: the queue to read from
 * @buf: pointer of read buffer
 * @len: read length
 */
static int ReadQueue(Queue *q, char *buf, unsigned int len)
{
    unsigned int rest = q->size - q->read;
    unsigned int ret = 0;

    if (!Queue_Empty(q)) {
        if (Avail(q) >= len) {
            ret = len;
            if (rest >= len) {
                memcpy(buf, q->buf + q->read, len);
                q->read = (q->read + len) % q->size;
                q->space += len;
            } else {
                memcpy(buf, q->buf + q->read, rest);
                q->read = 0;
                memcpy(buf + rest, q->buf, len - rest);
                q->read = len -rest;
                q->space += len;
            }
            return len;
        } else {
            ret = Avail(q);//Çå¿Õ¶ÓÁÐ
            if (rest >= Avail(q)) {
                memcpy(buf, q->buf + q->read, Avail(q));
                q->read = (q->read + Avail(q)) % q->size;
                q->space = q->size;
            } else {
                memcpy(buf, q->buf + q->read, rest);
                q->read = 0;
                memcpy(buf + rest, q->buf, Avail(q) - rest);
                q->read = Avail(q) -rest;
                q->space = q->size;
            }
        }
    } 
    return ret;
}

void queue_test()
{
    Queue_Init(&q1, 1000);
    //Queue_Destroy(&q1);
}
	 int ret = 0;
    char buf[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char buf2[10];
void test()
{
	static u16 i=41;
	if(i++<300)
		buf[0] = i;
	else
		i=1;
	ret = WriteQueue(&q1, buf, 9);
	if(i++<300)
		buf[0] = i;
	else
		i=1;
    ret = WriteQueue(&q1, buf, 9);
    printf("writed %d bytes.\n", ret);

    ret = ReadQueue(&q1, buf2, 9);
    printf("readed %d bytes.\n", ret);


}






