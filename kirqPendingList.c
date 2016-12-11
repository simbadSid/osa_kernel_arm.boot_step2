#include "kirqPendingList.h"





/**
 * Strcture to store the pending IRQ requests
 */
kIrqPendingList *irqPendingList;
int		nbPendingIrqRequest;



/**
 * Initialize the structures that contain the pending IRQ requests
 */
void initIrqPendingList()
{
	nbPendingIrqRequest	= 0;
	irqPendingList		= NULL;
}


/**
 * Add the IRQ of given type to the list of pending IRQ (that have not been handeled yet).
 * Print and error (and halt) if the list is already full.
 */
void addPendingIrq (kIrqPendingEntry entry)
{
	if (nbPendingIrqRequest >= MAX_NBR_PENDING_IRQ)
		panic(666, "IRQ pending list overflow\n\r");

	kIrqPendingList *newIrqPendingList = kmalloc(sizeof(kIrqPendingList));

	newIrqPendingList->entry	= entry;
	newIrqPendingList->next		= irqPendingList;
	irqPendingList			= newIrqPendingList;
	nbPendingIrqRequest ++;


/*
int j=0;
for (j=0; j<nbArg; j++)
{
	int arg = va_arg(argList, int);
	if (arg==0) continue;
}
*/
}


char isFullPendingIrqList()
{
	return (nbPendingIrqRequest >= MAX_NBR_PENDING_IRQ);
}


/**
 * Put a pending IRQ into the parameter pendingEntry.   Remove the returned entry from the local list.
 * Return 0 if the local list contains no pending IRQ, and 1 otherwise.
 */
unsigned int getAndRemovePendingIrq(kIrqPendingEntry *pendingEntry)
{
	if (nbPendingIrqRequest <= 0)
		return 0;

	kIrqPendingList *tmpIrqPendingList	= irqPendingList;
	kIrqPendingList *prevIrqPendingList	= NULL;

	while(tmpIrqPendingList->next != NULL)
	{
		prevIrqPendingList	= tmpIrqPendingList;
		tmpIrqPendingList	= tmpIrqPendingList->next;
	}

	*pendingEntry = tmpIrqPendingList->entry;
	kfree(tmpIrqPendingList);
	nbPendingIrqRequest --;
	if (prevIrqPendingList != NULL)
		prevIrqPendingList->next = NULL;
	else
		irqPendingList = NULL;

	return 1;
}

