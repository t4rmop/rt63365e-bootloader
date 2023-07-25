#include "uart_debug.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"
#include "flash.h"
#include "utils.h"
#include "exceptions.h"
#include "regdef.h"
#include "../lib/gdbstub/src/gdb_stub.h"

u8 *buf = (u8 *)"Bienvenido\n\r";

__attribute__ ((section(".text.main")))
void _main() {
	uart_puts(buf);
	config_exceptions();
	init_irq();
	//config_timer();
	//mips_gdb_stub_install();
	while (1) {
		//value1=read_frame(0xbfc00044);
		//uart_printf("El valor es %x\n\r",value1);
		//uart_printf("ra: %x", regs_init->reg32);
		uart_debug_process();
	}
}
