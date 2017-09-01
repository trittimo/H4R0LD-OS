#ifndef _INTERRUPT_H
#define _INTERRUPT_H

void handleInterrupt21(int ax, int bx, int cx, int dx);
void registerInterruptHandlers();

#endif