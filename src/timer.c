#include "utils.h"
#include "exceptions.h"
#include "uart.h"
#include "regdef.h"
#include "timer.h"

extern void (*irq_handlers[32])();

int cuenta = 0;

void config_timer() {
	u32 status, prev_status;
	write32((u32 *) 0xbfbf010c, 0x7000*10);
	//write32((u32 *) 0xbfbf0100, read32((u32 *) 0xbfbf0100) | 0x202);
	write32((u32 *) 0xbfbf0100, (1 << 9) | (1 << 1) | read32((u32 *) 0xbfbf0100));
	mfc0(prev_status, C0_STATUS, 0);
	mtc0(((prev_status | 0x1) ^ 0x1), C0_STATUS, 0); 
	irq_handlers[5] = timer_handler;
	mfc0(status, C0_STATUS, 0);
	mtc0(((prev_status & 0x1) | ((status | 1)  ^ 0x1)), C0_STATUS, 0); 
	write32((u32 *) 0xbfb40004, ((1 << 5) | read32((u32 *) 0xbfb40004)));
	//write32((u32 *) 0xbfb40004, (~(1 << 5) & read32((u32 *) 0xbfb40004)));

}

//INT timer 5
void timer_handler() {
	cuenta++;	
	uart_printf("Hola su cuenta es: %d\n\r",cuenta);
}
