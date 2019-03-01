// testList.c - testing DLList data type
// Written by John Shepherd, March 2013
// Modified by Kanadech z5176970

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "DLList.h"

void test1 (DLList myList);
void printList (DLList myList);

int main (void)
{
	DLList myList = getDLList (stdin);
	test1(myList);
	return EXIT_SUCCESS;
}

void test1 (DLList myList){
	
	size_t initialLength = DLListLength(myList);
	assert(validDLList (myList));
	char *a = "AAA";	// 1sr string
	char *b = "BBB";	// 2nd string
	char *c = "CCC";	// 3rd string
	char temp[100];		// temp string for comparison
	memset(temp, '\0', sizeof(temp));
	printf("Current-> %s\n",DLListCurrent(myList));
	printf("Moving Current halfway (%d)\n",(int)(initialLength/2));
	DLListMove(myList, (int)(initialLength/2)); // Moves curr mid-list
	printf("Current-> %s\n",DLListCurrent(myList));
	printf("Insert a string %s after\n",a);
	DLListAfter(myList, a); // Inserts first string
    assert(DLListLength(myList) == initialLength + 1);
	assert(validDLList(myList));
	printList(myList);
	printf("Current-> %s\n",DLListCurrent(myList));

	// printf("Try moving current forward and backward 1 node\n");
	// if (DLListMove(myList,1)) {
	// 	printf("Current-> %s\n",DLListCurrent(myList));
	// 	DLListMove(myList,-1);
	// 	printf("Current-> %s\n",DLListCurrent(myList));
	// 	assert(validDLList(myList));
	// } else {
	// 	printf("Can't move forward (End of list)\n");
	// 	printf("Current-> %s\n",DLListCurrent(myList));
	// 	assert(validDLList(myList));
	// }
	
	printf("Moving current forward 1 node\n");
	DLListMove(myList,1);
	strcpy(temp,DLListCurrent(myList));
	printf("Temp is %s and Current->%s\n",temp,DLListCurrent(myList));
	DLListAfter(myList, "test1");
	printf("Current-> %s\n",DLListCurrent(myList));
	DLListAfter(myList, "test2");
	printf("Current-> %s\n",DLListCurrent(myList));
	DLListAfter(myList, "test3");
	printf("Current-> %s\n",DLListCurrent(myList));
	printList(myList);
	assert(strcmp(DLListCurrent(myList),"test3")==0);
	printf("Delete last 3 nodes\n");
	printf("B1Current-> %s\n",DLListCurrent(myList));
	DLListDelete(myList);
	printf("B2Current-> %s\n",DLListCurrent(myList));
	DLListDelete(myList);
	printf("B3Current-> %s\n",DLListCurrent(myList));
	DLListDelete(myList);
	printf("A3Current-> %s\n",DLListCurrent(myList));
	printList(myList);
	assert(strcmp(DLListCurrent(myList),temp)==0);
	printList(myList);
	printf("Move back to first and insert %s at the beginning\n",b);
	DLListMoveTo(myList,1);
	DLListBefore(myList,b);
	assert(DLListLength(myList)==initialLength+2);
	assert(strcmp(DLListCurrent(myList),b)==0);
	printf("Current-> %s\n",DLListCurrent(myList));
	assert(validDLList(myList));
	printList(myList);
	
	printf("\nTest 1 Completed\n");
	freeDLList (myList);
}

void printList (DLList myList) {
	printf("\n------\n");
	putDLList (stdout, myList);
	printf("------\n");
}
