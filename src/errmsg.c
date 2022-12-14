/*********************/
/* errmsg.c          */
/* for Par 3.20      */
/* Copyright 1993 by */
/* Adam M. Costello  */
/*********************/

/* This is ANSI C code. */
#include "errmsg.h"  /* Makes sure we're consistent with the declarations. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*

char errmsg[163];

*/

/**
 * @brief  Set an error indication, with a specified error message.
 * @param msg Pointer to the error message.  The string passed by the caller
 * will be copied.
 */
const char * const outofmem = "Out of memory.\n";

static char *errmsg;

void set_error(char *msg){
    
    clear_error();
    errmsg = strdup(msg); //copies the string called 
    return;
}

/**
 * @brief  Test whether there is currently an error indication.
 * @return 1 if an error indication currently exists, 0 otherwise.
 */

int is_error(){
    if(errmsg == NULL){
        return 0;
    }else{
    return 1;
    }
}

/**
 * @brief  Issue any existing error message to the specified output stream.
 * @param file  Stream to which the error message is to be issued.  
 * @return 0 if either there was no existing error message, or else there
 * was an existing error message and it was successfully output.
 * Return non-zero if the attempt to output an existing error message
 * failed.
 */

int report_error(FILE *file){

    if(is_error()){
        if(file != NULL){
            fputs(errmsg,file);
            return 0;
        }else{
            return 1;
        }

    }else{
        return 0;
    }
}

/**
 * Clear any existing error indication and free storage occupied by
 * any existing error message.
 */

void clear_error(){

    free(errmsg); //frees the error msg
    errmsg = NULL; //so the memory could not point to something else later
    return;

}