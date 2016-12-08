/*
 * user.c
 *
 *  Created on: Dec 2, 2016
 *      Author: ogruber
 */


#include <stddef.h>
#include <stdint.h>

static int errcode = 0;
void kprintf(const char *fmt, ...);
void _arm_usr_mode(uint32_t userno, void* entry);
void _arm_halt(void);

void exit(int code) {
  errcode = code;
  __asm volatile ("swi #0x00" ::: );
  _arm_halt(); // should never execute!
}

void sleep(uint32_t delay) {
  __asm volatile ("swi #0x01" ::: );
}

/*
 * WARNING: do not change this signature without changing the assembly code
 * in gic.s -> see _arm_usr_mode
 */
void main(uint32_t pid) {
  kprintf("USER[%d]: Hello!\n",pid);
  /*
   * Attempt to change to SYS mode, by changing the CPSR
   * That can only be done in privileged mode.
   * Notice that the code does not crash, but has no effect.
   * So this is an example of a sensitive instruction that does not trap
   */
  unsigned long old, niu;
  __asm__ __volatile__(
      "mrs %0, cpsr\n"
      "orr %1, %0, #0x1F \n"
      "msr cpsr, %1\n"
      "mrs %1, cpsr"
      : "=r" (old), "=r" (niu)
      :
      : "memory");
  if (old!=niu) {
    kprintf("USR mode succeeded to change to SYS mode");
    kprintf(" cpsr=0x%x -> cpsr=0x%x \n",old,niu);
  } else
    kprintf(" -- in USR mode!\n");

  /*
   * Now try one syscall (not implemented, will do nothing.
   */
  sleep(0x1234);

  /*
   * Terminate this "process"...
   */
  exit(0);
}

void umain(uint32_t pid) {
  kprintf("--> launching user pid=%d...\n",pid);
  _arm_usr_mode(pid,main);
  kprintf("--> user pid=%d exited, errcode=%d \n",pid, errcode);
}
