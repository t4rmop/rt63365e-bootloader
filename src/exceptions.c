#include "utils.h"
#include "regdef.h"
#include "uart.h"
#include "cache.h"

extern u32 _irq_size;
extern void* trampoline_irq;
extern void* trampoline_exception;
extern void* exception_dispatcher;
u32 exception_handlers[32];

void exception_handler(struct reg_struct *reg_context) {
	u32 status,cause,epc,badaddr;
	mfc0(status, C0_STATUS, 0);
	mfc0(cause, C0_CAUSE, 0);
	mfc0(epc, C0_EPC, 0);
	mfc0(badaddr, C0_BADVADDR, 0);
	uart_printf("excepcion\n\r");
	uart_printf("pc es 0x%x \n\r", reg_context->reg31);
	uart_printf("status es 0x%x \n\r", status);
	uart_printf("cause es 0x%x \n\r", cause);
	uart_printf("epc es 0x%x \n\r", epc);
	uart_printf("badaddr es 0x%x \n\r", badaddr);
	while(1);
}

void irq_handler(struct reg_struct *reg_context) {
	u32 status, cause, epc, badaddr;
	mfc0(status, C0_STATUS, 0);
	mfc0(cause, C0_CAUSE, 0);
	mfc0(epc, C0_EPC, 0);
	mfc0(badaddr, C0_BADVADDR, 0);
	uart_printf("interrupcion\n\r");
	uart_printf("pc es 0x%x \n\r", reg_context->reg31);
	uart_printf("status es 0x%x \n\r", status);
	uart_printf("cause es 0x%x \n\r", cause);
	uart_printf("epc es 0x%x \n\r", epc);
	uart_printf("badaddr es 0x%x \n\r", badaddr);
	return;
}


void set_except_vector(int n, void *addr) {
	u32 handler = (u32) addr;
	exception_handlers[n] = handler;
}


void config_exceptions() {
	u32 cause;
	u32 status;
	mtc0(0x80000000, C0_EBASE, 1);		
	mfc0(cause, C0_CAUSE, 0);		
	mtc0(cause & (~(1<<18)), C0_CAUSE, 0); 
	mtc0(cause & 0xfffffc1f, C0_INTCTL, 1); //intctl

	mfc0(status, C0_STATUS, 0);		//status
	mtc0(status & ~0x400000, C0_STATUS, 0x0);

	memcpy32((u32 *)0x80000180, &trampoline_exception, 64);
	memcpy32((u32 *)0x80000200, &trampoline_irq, 64);

	//set_except_vector(0,(void *)0x80020200);

	for(int i=1;i<32;i++){
		set_except_vector(i, &exception_dispatcher);
	}

	memcpy32((u32 *)0x80000000, exception_handlers, 32*4);
	flush_icache_range(0x80000000, 0x200);
}

