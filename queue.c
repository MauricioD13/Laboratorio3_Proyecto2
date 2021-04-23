#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5
short int vector[5];
int main(){
    short int* index_escribir = &vector[0];
    short int* index_leer = &vector[0];
    short int* index_anterior_leer = &vector[0];
    short int* index_anterior_escribir = &vector[0];
    short int flag_inicio = 0; // 1 -> Buffer lleno , 2 -> Dejo de estar lleno reset 
    short int flag_buffer = 0;
    while(1){
        int option = 0;
        int dato = 0;
        printf("\n[1]Ingresar dato\n[2]Sacar y leer dato\n[3]Todo el vector\n[4]Index\n[5]Salir\n");
        scanf("%d",&option);
        if(option == 1){
            printf("Index leer: %p \t index escribir: %p\n",index_leer,index_escribir);
            if((index_escribir == index_leer) && (flag_inicio == 1)){
                    printf("Buffer lleno\n");
                    flag_buffer = 1;
                    continue;
            }else{
                flag_buffer = 0;
            }
            
            flag_inicio = 1;
            if(flag_buffer == 0){
                printf("Dato a ingresar: ");
                scanf("%d",&dato);
                *index_escribir = dato;
                
                if(index_escribir == &vector[LENGTH-1]){
                    index_escribir = &vector[0];
                }else{
                    index_escribir++;
                }
            }
            
           
        }
        else if(option == 2){
            if(index_leer == index_escribir && flag_buffer == 0){
                printf("No hay mas datos en la cola\n");
                continue;
            }
            
            dato = *index_leer;
            printf("Dato leido: %d\n",dato);
            *index_leer = 0;
            
            if(index_leer == &vector[LENGTH]){
                index_leer = &vector[0];
            }else{
                index_leer++;
            }
            flag_buffer = 0;
            if(index_leer == index_escribir){
                printf("No hay mas datos en la cola\n");
            }
        }
        else if(option == 3){
            int i = 0;
            while(vector[i] != 0){
                printf("vector: %d\n",vector[i]);
                i++;
            }
        }
        else if(option == 4){
            printf("Index leer: %p \t index esccribir: %p\n",index_leer,index_escribir);
        }
        else if(option == 5){
            printf("Adios\n");
            break;
        }else{
            break;
        }
    }
}