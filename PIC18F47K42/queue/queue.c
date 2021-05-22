#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#define LENGTH 1100 
/*Must be declare in main
short int vector[LENGHT];
*/
/*Struct initialization 
queue_param.read_index = &vector[0];
queue_param.write_index = &vector[0];
queue_param.buffer_flag = 0;
queue_param.begin_flag = 0;
*/
void queue_init(QUEUE *queue,short int *vector){ // QUEUE INITIALIZATION
    queue->last_pointer = &vector[LENGTH];
    queue->initial_pointer = &vector[0];
    queue->read_index = &vector[0];
    queue->write_index = &vector[0];
    queue->buffer_flag = 0;
    queue->begin_flag = 0;
    queue->queue_empty = 1;
    queue->quantity = 0;
}
short int push(QUEUE *queue, short int data){
    
    if((queue->write_index == queue->read_index) && (queue->begin_flag == 1)){
              
        queue->buffer_flag = 1;
        return 0;
    }else{
        queue->buffer_flag = 0;
    }
            
    queue->begin_flag = 1;
    if(queue->buffer_flag == 0){
        *(queue->write_index) = data;
        queue->queue_empty = 0;
        queue->quantity++;
        if(queue->write_index == (queue->last_pointer)-1){ 
            queue->write_index = queue->initial_pointer;
        }else{
            (queue->write_index)++;
        }
    }
    
}
short int pop(QUEUE *queue){
    int data = 0;
    
    if(queue->read_index == queue->write_index && queue->buffer_flag == 0){
            
        return 0; // Case where queue is empty
    }
            
    data = *(queue->read_index);
    queue->quantity--;
    *(queue->read_index) = 0;
    
    if(queue->read_index == queue->last_pointer){
        queue->read_index = queue->initial_pointer;
    }else{
        queue->read_index++;
    }

    queue->buffer_flag = 0;
    if(queue->read_index == queue->write_index){
        queue->queue_empty = 1;
        // Case where queue is empty
    }
    return data;
    
}