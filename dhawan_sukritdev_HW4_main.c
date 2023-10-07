/**************************************************************
* Class:  CSC-415-01 Spring 2023
* Name: Sukrit Dev Dhawan
* Student ID: 922432027
* GitHub ID: SukritD27
* Project: Assignment 4 – Word Blast
*
* File: dhawan_sukritdev_HW4_main.c
*
* Description:
*
**************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#define wordLength 6
 
int size = 20000;
int x = 0;
int numberOfEntries = 0;
int filePerThread;
int threadCount;
char * buffer;
pthread_mutex_t mutex;

int startPoint = 0;

typedef struct Node{
    char * item;
    int count;
} Node;

// You may find this Useful
char * delim = "\"\'.“”‘’?:;-,—*($%)! \t\n\x0A\r";


int pivotFinder(Node * array, int start, int end){
    int pivot = array[end].count;
    int  i = start-1;

    for(int k = start; k < end; k++){ 
        if(array[k].count >= pivot){
            i++;
            Node temp = array[i];
            array[i] = array[k]; 
            array[k] = temp; 
        }
    }

    Node temp1 = array[i+1];
    array[i+1] = array[end];
    array[end] = temp1;

    return (i+1);
}

void quickSort(Node * array, int start, int end){
    if(start < end){
        int pivot = pivotFinder(array, start, end);
        quickSort(array, start, pivot-1);
        quickSort(array, pivot+1, end);
    }
}


void * fileParser(void * array){

    Node * words = (Node *)array;  

    // filePerThread = fileSize/threadCount;

    char *startingPoint = buffer;

    char * buffer1 = calloc(filePerThread, sizeof(char));


    if(pthread_mutex_lock(&mutex)){
        perror("Mutex lock!");
    }

    startingPoint = startingPoint + x*filePerThread;
    memcpy(buffer1, startingPoint, filePerThread);
    x++;

    if(pthread_mutex_unlock(&mutex)){
        perror("Mutex unlock!");
    }

    char * ptr = NULL;

    char * token = strtok_r(buffer1, delim, &ptr);
    
    bool result = 0;
   
    while(token != NULL){

        result = 0;

        if(strlen(token) < wordLength){
            token = strtok_r(NULL, delim, &ptr);
            continue;
        }

        if(numberOfEntries == size){ 
            if(pthread_mutex_lock(&mutex)){
                perror("Mutex lock!");
            }
            words = (Node *) realloc(words, sizeof(Node)*2*size);
            size = size*2;
            if(pthread_mutex_unlock(&mutex)){
                perror("Mutex unlock!");
            }
        }

        for(int i = 0; i < numberOfEntries; i++){ 
            if(words[i].item != NULL && strcasecmp(words[i].item, token) == 0){
                if(pthread_mutex_lock(&mutex)){
                    perror("Mutex lock!");
                }
                words[i].count += 1; 
                if(pthread_mutex_unlock(&mutex)){
                    perror("Mutex unlock!");
                }
                result = 1;      
                break;
            }
            
        }

        if(result == 1){
            token = strtok_r(NULL, delim, &ptr);
            continue;
        }
        
        
        if(pthread_mutex_lock(&mutex)){
            perror("Mutex lock!");
        }
        struct Node node;
        node.item = token;
        node.count = 1;
        words[numberOfEntries] = node;    
        numberOfEntries++;                

        if(pthread_mutex_unlock(&mutex)){
            perror("Mutex unlock!");
        }
        
        token = strtok_r(NULL, delim, &ptr);
    } 
}

int main (int argc, char *argv[])
    {
    //***TO DO***  Look at arguments, open file, divide by threads
    //             Allocate and Initialize and storage structures

    if(argc < 3){
        printf("Not enough arguments provided!\n");
        return 0;
    }

    char * path = argv[1];

    //Converts a string to an integer
    threadCount = atoi(argv[2]);

    int fd = open(path, O_RDONLY);

    int fileSize = lseek(fd, 0, SEEK_END);

    if(fileSize == -1){
        perror("errno");
        exit(1);
    }

    buffer = calloc(fileSize, sizeof(char));


    ssize_t bytesRead = pread(fd, buffer, fileSize, 0);
    if(bytesRead == -1){
        perror("Error: ");
    }    

    filePerThread = fileSize/threadCount;

    close(fd);
    void * res;

    struct Node * words = calloc(size,sizeof(Node));

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish

    pthread_t tid[threadCount];

    for(int i = 0; i < threadCount; i++){
        pthread_create(&tid[i], NULL, fileParser, words);
        
    }

    for(int i = 0; i < threadCount; i++){
        pthread_join(tid[i], &res);
    }


    // ***TO DO *** Process TOP 10 and display
    quickSort(words, 0, numberOfEntries-1); 

    printf("\n\nWord Frequency Count on %s with %d threads\n", path, threadCount);
    printf("Printing top 10 words %d characters or more.\n", wordLength);
    for(int i = 0; i < 10; i++){     
        printf("Number %d is %s ", i+1 ,words[i].item);   
        printf("with a count of %d\n", words[i].count);
    }

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Clock output
    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
        {
        --sec;
        n_sec = n_sec + 1000000000L;
        }

    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    //**************************************************************


    // ***TO DO *** cleanup
    free(buffer);
    free(words);
    return 0;
    }
