#ifndef QUEUE_H
#define QUEUE_H
#define LENGTH 1100 //Amount of position in the queue
typedef struct QUEUE{
    short int begin_flag;
    short int buffer_flag;
    short int *read_index;
    short int *write_index;
    short int *last_pointer;
    short int *initial_pointer;
}QUEUE;
void queue_init(QUEUE *queue,short int *vector);
short int push(QUEUE *queue, int data);
short int pop(QUEUE *queue);
#endif