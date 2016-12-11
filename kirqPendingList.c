#include "kirqPendingList.h"





/**
 * Strcture to store the pending IRQ requests
 */
kIrqPendingList irqPendingList;
int		nbPendingIrqRequest;



/**
 * Allocate and return a pointer to an empty list of pending IRQ requests (that have not been handeled yet).
 */
void initIrqPendingList()
{
	int i;

	nbPendingIrqRequest = 0;
	for (i=0; i<MAX_NBR_PENDING_IRQ; i++)
	{
		irqPendingList[i].irqId = 0;
	}
}


/**
 * Add the IRQ of given type to the list of pending IRQ (that have not been handeled yet).
 * Print and error (and halt) if the list is already full.
 */
void addPendingIrq (kIrqPendingEntry entry)
{
	if (nbPendingIrqRequest >= MAX_NBR_PENDING_IRQ)
		panic(666, "IRQ pending list overflow\n\r");

	irqPendingList[nbPendingIrqRequest] = entry;
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

	*pendingEntry = irqPendingList[nbPendingIrqRequest-1];
	nbPendingIrqRequest--;

	return 1;
}

