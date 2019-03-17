// ThreadTree.c ... implementation of Tree-of-Mail-Threads ADT
// Written by John Shepherd, Feb 2019

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
} ThreadTreeRep;

// Auxiliary data structures and functions

// Add any new data structures and functions here ...

static void doDropThreadTree (Link t);
static void doShowThreadTree (Link t, int level);
static Link newThreadTreeNode (MailMessage mesg);
static void ThreadTreeInsertTopLevel (ThreadTree t, MailMessage m);
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
	// You need to implement this
	ThreadTree tree = newThreadTree();
	MMListStart(mesgs);
	while(!MMListEnd(mesgs)) {
		MailMessage m = MMListNext(mesgs);
		if (MailMessageRepliesTo(m) == NULL) {
			// Insert top level list
			ThreadTreeInsertTopLevel(tree,m);
		} else {
			// Traverse through the tree to insert
		}
	}
	return tree; // change this line
}

static Link newThreadTreeNode (MailMessage mesg) 
{
	Link new = malloc (sizeof (ThreadTreeNode));
	if (new == NULL) err (EX_OSERR, "couldn't allocate ThreadTree node");
	//*new = (ThreadTreeNode) {.mesg = mesg};
	new->mesg = mesg;
	new->next = new->replies = NULL;
	return new;
}

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