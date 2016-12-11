/*
 * Copyright (C) SID LAKHDAR Riyane.
 *
 * This code is a patch to the code implemented by Pr Olivier 
 * Gruber during the Advanced Operating System cours.
 * It is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Classpath; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 *
 */





#ifndef K_IRQ__PENDING_LIST_H
#define K_IRQ__PENDING_LIST_H

#include "kmem.h"
#include "board.h"

#include <stdarg.h>




#define MAX_NBR_PENDING_IRQ	3


typedef struct __attribute ((packed)) K_IRQ_PENDING_ENTRY
{
	uint32_t irqId;
	union __attribute ((packed))
	{
		struct __attribute ((packed))
		{
			char	receivedChar;
			void	(*handler)(char);
		}uart0;
	};
} kIrqPendingEntry;


// TODO: this struct will change to a list when the kmem (dynamic memory allocation) will be available
typedef kIrqPendingEntry kIrqPendingList[MAX_NBR_PENDING_IRQ];





void		initIrqPendingList		();
void		addPendingIrq			(kIrqPendingEntry entry);
char		isFullPendingIrqList		();
unsigned int	getAndRemovePendingIrq		(kIrqPendingEntry *pendingEntry);



#endif
