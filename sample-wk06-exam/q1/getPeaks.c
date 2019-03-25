// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/

DLList getPeaks(DLList L){

	DLList peaksL = newDLList();
    if (DLListLength(L) >= 3) {
        L->curr = L->first->next;
        while (L->curr->next != NULL){
            if (L->curr->prev->value < L->curr->value && L->curr->next->value < L->curr->value){
                DLListNodeP new = newDLListNode(L->curr->value);
                if(DLListIsEmpty(peaksL)){
                    peaksL->first = peaksL->last = peaksL->curr = new;
                    peaksL->nitems = 1;
                } else {
                    peaksL->curr->next = new;
                    peaksL->last = new;
                    peaksL->curr = peaksL->curr->next;
                    peaksL->nitems++;
                }
            }
            L->curr = L->curr->next;
        }
    }

	return peaksL;

}



