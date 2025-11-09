#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = NULL;
    fp = fopen(filename, "r");
    board = malloc(sizeof(int*) * 9);
    for(int i = 0; i < 9; i++){
        board[i] = malloc(sizeof(int) * 9);
    }
    for(int i = 0;i < 9;i++){
        for(int j = 0;j<8;j++){
            fscanf(fp,"%d,", &board[i][j]);
        }
        fscanf(fp,"%d", &board[i][8]);
    }
    fclose(fp);
    return board;
}

void* checkBox(void* param){
    param_struct* data = (param_struct*)param;
    int used[10] = {0};
    for(int i = data->starting_row; i< data->ending_row;i++){
        for(int j = data->starting_col; j < data->ending_col;j++){
            int val = sudoku_board[i][j];
            if (val < 1 || val > 9 || used[val]){
                worker_validation[data->id] = 0;
                return NULL;
            }
            used[val] = 1;
        }
    }
    worker_validation[data->id] = 1;
    return NULL;
}

int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;

    tid = (pthread_t*)malloc(sizeof(pthread_t) *27);
    parameter = (param_struct*)malloc(sizeof(param_struct) * 27);
    worker_validation = (int*)malloc(sizeof(int) * 27);
    
    pthread_attr_init(&attr);

    for(int i = 0; i < 27;i++){
        worker_validation[i]=0;
    }
    int id = 0;
    for(int i = 0;i<9;i++){
        parameter[id].id = id;
        parameter[id].starting_row = i;
        parameter[id].starting_col = 0;
        parameter[id].ending_row = i+1;
        parameter[id].ending_col = 9;
        pthread_create(&tid[id], &attr, checkBox, &parameter[id]);
        id++;

    }
    for(int i = 0;i < 9;i++){
        parameter[id].id = id;
        parameter[id].starting_row = 0;
        parameter[id].starting_col = i;
        parameter[id].ending_row = 9;
        parameter[id].ending_col = i + 1;
        pthread_create(&tid[id], &attr, checkBox, &parameter[id]);
        id++;
    }
    for(int i = 0;i < 9; i++){
        int a = (i/3) * 3;
        int b = (i%3) * 3;
        parameter[id].id = id;
        parameter[id].starting_row = a;
        parameter[id].starting_col = b;
        parameter[id].ending_row = a + 3;
        parameter[id].ending_col = b + 3;
        pthread_create(&tid[id], &attr, checkBox, &parameter[id]);
        id++;
    }
    for(int i = 0;i < 27;i++){
        pthread_join(tid[i], NULL);
    }
    for(int i = 0;i<27;i++){
        if(worker_validation[i] != 1){
            free(tid);
            free(parameter);
            free(worker_validation);
            return 0;
        }
        }
    free(tid);
    free(parameter);
    free(worker_validation);
    return 1;
    }
    
    

