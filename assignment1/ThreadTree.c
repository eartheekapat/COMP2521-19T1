// ThreadTree.c ... implementation of Tree-of-Mail-Threads ADT
// Written by John Shepherd, Feb 2019
// Edited by Kanadech Jirapongtanavech z5176970 Mar 2019

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "MMList.h"
#include "MMTree.h"
#include "MailMessage.h"
#include "ThreadTree.h"

// Representation of ThreadTree's

typedef struct ThreadTreeNode *Link;

typedef struct ThreadTreeNode {
    MailMessage mesg;
    Link next, replies;
} ThreadTreeNode;

typedef struct ThreadTreeRep {
    Link messages;
    Link ptr;	// temporary pointer to the reply node (for inserting replies)
} ThreadTreeRep;

// Auxiliary data structures and functions

// Add any new data structures and functions here ...

static void doDropThreadTree (Link t);
static void doShowThreadTree (Link t, int level);
// create new empty ThreadTreeNode
static Link newThreadTreeNode (MailMessage mesg);

// Insert the message to top level list
static void ThreadTreeInsertTopLevel (ThreadTree t, MailMessage m);

// Insert the message at the end of the passed in ThreadTree node
static void ThreadTreeInsertReply (Link t, MailMessage m);

// Trasverse the ThreadTree and find the replyTo node
static void ThreadTreeTraverse (ThreadTree tree, Link t, MailMessage m, int found);
// END auxiliary data structures and functions

// create a new empty ThreadTree
ThreadTree newThreadTree (void)
{
    ThreadTreeRep *new = malloc (sizeof *new);
    if (new == NULL) err (EX_OSERR, "couldn't allocate ThreadTree");
    *new = (ThreadTreeRep) { };
    return new;
}

void dropThreadTree (ThreadTree tt)
{
    assert (tt != NULL);
    doDropThreadTree (tt->messages);
}

// free up memory associated with list
static void doDropThreadTree (Link t)
{
    if (t == NULL)
        return;

    for (Link curr = t, next; curr != NULL; curr = next) {
        next = curr->next;
        doDropThreadTree (curr->replies);
        // don't drop curr->mesg, in case referenced elsehwere
        free (curr);
    }
}

void showThreadTree (ThreadTree tt)
{
    assert (tt != NULL);
    doShowThreadTree (tt->messages, 0);
}

// display thread tree as hiearchical list
static void doShowThreadTree (Link t, int level)
{
    if (t == NULL)
        return;
    for (Link curr = t; curr != NULL; curr = curr->next) {
        showMailMessage (curr->mesg, level);
        doShowThreadTree (curr->replies, level + 1);
    }
}

// insert mail message into ThreadTree
// if a reply, insert in appropriate replies list
// whichever list inserted, must be in timestamp-order
ThreadTree ThreadTreeBuild (MMList mesgs, MMTree msgids)
{
    ThreadTree tree = newThreadTree();
    MMListStart(mesgs);
    // Iterates the MMList
    while(!MMListEnd(mesgs)) {
        MailMessage m = MMListNext(mesgs);
        // If the message does not reply to any message,
        // insert at top level else, find the replyTo node
        // and insert the message there
        if (MailMessageRepliesTo(m) == NULL) {
            // Insert top level list
            ThreadTreeInsertTopLevel(tree,m);
        } else {
            // Traverse through the tree to insert
            // found flag = 0 initially
            ThreadTreeTraverse(tree,tree->messages,m,0);
            // Insert the message
            ThreadTreeInsertReply(tree->ptr,m);
        }
    }
    return tree;
}

// create new empty ThreadTreeNode
static Link newThreadTreeNode (MailMessage mesg) 
{
    Link new = malloc (sizeof (ThreadTreeNode));
    if (new == NULL) err (EX_OSERR, "couldn't allocate ThreadTree node");
    new->mesg = mesg;
    new->next = new->replies = NULL;
    return new;
}

// Insert the message to top level list
static void ThreadTreeInsertTopLevel (ThreadTree t, MailMessage m) {
    Link new = newThreadTreeNode(m);
    if (t->messages == NULL) {
        t->messages = new;
    } else {
        Link curr = t->messages;
        while(curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new;
    }
}

// Insert the message at the end of the passed in ThreadTree node
static void ThreadTreeInsertReply (Link t, MailMessage m) {
    Link new = newThreadTreeNode(m);
    if(t->replies == NULL){
        t->replies = new;
    } else {
        Link curr = t->replies;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new;
    }
}


// Trasverse the ThreadTree and find the replyTo node
static void ThreadTreeTraverse (ThreadTree tree, Link t, MailMessage m, int found) {
    // If found matching detail, set tree->ptr to that node else keep searching
    // set found flag to minimize traversal
    if (found) return;
    if (t==NULL) return;
    if (strcmp(MailMessageID(t->mesg),MailMessageRepliesTo(m)) == 0) {
        tree->ptr = t;
        found = 1;
    }
    ThreadTreeTraverse(tree,t->next,m,found);
    ThreadTreeTraverse(tree,t->replies,m,found);
}