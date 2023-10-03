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
 
#define STARTING_SIZE       50

int num = 0;

typedef struct Node{
    char * item;
    int count;
} Node;

// typedef struct ArrayD {
//     struct Node items[STARTING_SIZE];
// } ArrayD;

// You may find this Useful
char * delim = "\"\'.“”‘’?:;-,—*($%)! \t\n\x0A\r";

int insert(char * element, Node array[]){

    printf("num = %d\n", num);

    if(num == 0){
        struct Node node;
        node.item = element;
        node.count = 1;
        array[num] = node;
        num++;
        for(int j = 0; j < num+1; j++){
            printf("%d: %s  ", j, array[j].item);
            printf("%d\n", array[j].count);
        }
        return 0;
    }


    for(int i = 0; i < num; i++){
        if(strcmp(array[i].item, element) == 0){
            array[i].count++;
            return 0;
        }
    }

    struct Node node;
    node.item = element;
    node.count = 1;
    array[num] = node;
    num++;


    for(int j = 0; j < num; j++){
        printf("%d: %s  ", j, array[j].item);
        printf("%d\n", array[j].count);
    }

    
    
    return 0;

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
    //atoi()
    int threadCount = argv[2];

    // for(int i = 0; i < threadCount; i++){
    //     pthread_create();
    // }
    // for(int i = 0; i < threadCount; i++){
    //     pthread_join();
    // }
    
    int fd = open(path, O_RDONLY);

    int fileSize = lseek(fd, 0, SEEK_END);
    printf("%d\n", fileSize);

    if(fileSize == -1){
        perror("errno");
        exit(1);
    }

    char * buffer = malloc(fileSize);

    ssize_t bytesRead = pread(fd, buffer, fileSize, 0);
    if(bytesRead == -1){
        perror("Error: ");
    }    

    close(fd);

    struct Node words[STARTING_SIZE];
    insert("hello", words);
    insert("hello", words);
    // insert("hello", words);

    insert("world", words);
    insert("yes", words);
    insert("haha", words);
    insert("lmao", words);
    insert("yes", words);
    insert("bitch", words);

    //printf("Bytes Read: %ld\n Buffer:\n%s\n",bytesRead, buffer);





    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish


    // ***TO DO *** Process TOP 10 and display

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

    return 0;
    }
