#ifndef QUEUE_H
#define QUEUE_H
#define LENGTH 50 //Amount of position in the queue
typedef struct QUEUE{
    short int begin_flag;
    short int buffer_flag;
    short int *read_index;
    short int *write_index;
    short int *last_pointer;
    short int *initial_pointer;
    short int queue_empty;
    int quantity;
}QUEUE;
void queue_init(QUEUE *queue,short int *vector);
short int push(QUEUE *queue, short int data);
short int pop(QUEUE *queue);
#endif