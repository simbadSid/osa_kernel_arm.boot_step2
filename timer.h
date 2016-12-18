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

#ifndef TIMER_H
#define TIMER_H

#include "board.h"



/**
 * Offset of the watchdog private registers (relative to 
 * the base address of the watchdog private memory region)
 */
#define TIMER_OFF_WATCHDOG_REGISTER_LOAD			0x20
#define TIMER_OFF_WATCHDOG_REGISTER_COUNTER			0x24
#define TIMER_OFF_WATCHDOG_REGISTER_CONTROL			0x28


/**
 * Index in the watchdog control register where to find the given informations
 */
#define TIMER_BIT_WATCHDOG_REGISTER_CONTROL_WATCHDOG_MODE	3
#define TIMER_BIT_WATCHDOG_REGISTER_CONTROL_INTERUPT_ENABLE	2
#define TIMER_BIT_WATCHDOG_REGISTER_CONTROL_AUTO_RELOAD		1
#define TIMER_BIT_WATCHDOG_REGISTER_CONTROL_WATCHDOG_ENABLE	0




void	setTimmer	(uint32_t time, char multipleShot);
void	unsetTimmer	();




#endif
