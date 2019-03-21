// MMList.c ... implementation of List-of-Mail-Messages ADT
// Written by John Shepherd, Feb 2019
// Edited by Kanadech Jirapongtanavech z5176970 Mar 2019

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "MMList.h"
#include "MailMessage.h"

// data structures representing MMList

typedef struct MMListNode *Link;

typedef struct MMListNode {
    MailMessage data; // message associated with this list item
    Link next;		  // pointer to node containing next element
} MMListNode;		  // declared as Link

typedef struct MMListRep {
    Link first; // node containing first value
    Link last;  // node containing last value
    Link curr;  // current node (for iteration)
} MMListRep;	// declared as MMList

static Link newMMListNode (MailMessage mesg);

// create a new empty MMList
MMList newMMList (void)
{
    MMListRep *new = malloc (sizeof *new);
    if (new == NULL) err (EX_OSERR, "couldn't allocate MMList");
    *new = (MMListRep) { };
    return new;
}

// free up memory associated with list
// note: does not remove Mail Messages
void dropMMList (MMList L)
{
    assert (L != NULL);
    Link curr = L->first;
    while (curr != NULL) {
        Link next = curr->next;
        free (curr);
        curr = next;
    }
    free (L);
}

// display list as one integer per line on stdout
void showMMList (MMList L)
{
    assert (L != NULL);
    for (Link curr = L->first; curr != NULL; curr = curr->next)
        showMailMessage (curr->data, 0);
}

// insert mail message in order
// ordering based on MailMessageDateTime
void MMListInsert (MMList L, MailMessage mesg)
{
    assert (L != NULL);
    assert (MMListIsOrdered (L));

    assert (mesg != NULL);

    // create new node
    Link new = newMMListNode(mesg);
    
    // If Empty list ,else 
    if ((L->first == NULL) && (L->last == NULL)) {
        L->first = L->last = L->curr = new;
    // Check begining
    } else if (DateTimeBefore(MailMessageDateTime(new->data),MailMessageDateTime(L->first->data))) {
            new->next = L->first;
            L->first = new;
    // Check ending
    } else if (DateTimeAfter(MailMessageDateTime(new->data),MailMessageDateTime(L->last->data))) {
            L->last->next = new;
            L->last = new;
    } else {
        // There are probably at least 2 nodes in the list
        // Insertion must be done in between
        MMListStart(L);

        // Create new insert node, previous node & date for checking because insertion must be in between
        DateTime insertDate = MailMessageDateTime(mesg);
        DateTime prevDate = NULL;
        Link prevNode = NULL;
        
        while(!MMListEnd(L)){
            DateTime currDate = MailMessageDateTime(L->curr->data);
            if (prevDate != NULL) {
                // Checking current date with the new date node and prev with the new date note
                if (DateTimeBefore(insertDate,currDate) && DateTimeAfter(insertDate,prevDate)) {
                    prevNode->next = new;
                    new->next = L->curr;
                    break;
                }
            }
            prevDate = currDate;
            prevNode = L->curr;
            L->curr = L->curr->next;
        }
        
    }
}

// create a new MMListNode for mail message
// (this function is used only within the ADT)
static Link newMMListNode (MailMessage mesg)
{
    Link new = malloc (sizeof *new);
    if (new == NULL) err (EX_OSERR, "couldn't allocate MMList node");
    *new = (MMListNode) { .data = mesg };
    return new;
}

// check whether a list is ordered (by MailMessageDate)
bool MMListIsOrdered (MMList L)
{
    DateTime prevDate = NULL;
    for (Link n = L->first; n != NULL; n = n->next) {
        DateTime currDate = MailMessageDateTime (n->data);
        if (prevDate != NULL && DateTimeAfter (prevDate, currDate))
            return false;
        prevDate = currDate;
    }
    return true;
}

// start scan of an MMList
void MMListStart (MMList L)
{
    assert (L != NULL);
    L->curr = L->first;
}

// get next item during scan of an MMList
MailMessage MMListNext (MMList L)
{
    assert (L != NULL);
    if (L->curr == NULL)
        // this is probably an error
        return NULL;

    MailMessage mesg = L->curr->data;
    L->curr = L->curr->next;
    return mesg;
}

// check whether MMList scan is complete
bool MMListEnd (MMList L)
{
    assert (L != NULL);
    return (L->curr == NULL);
}
