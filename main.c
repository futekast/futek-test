#include "Config.h"
#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>

#define F_CPU 100000000UL
#define FCY F_CPU / 2
#define TICK_HZ F_CPU / 2
#define MS 1000
#define US 1000000
#define shitthat 1

void DelayMS(unsigned long m)
{
    unsigned long stop = m * (TICK_HZ / MS) + _mfc0(_CP0_COUNT, _CP0_COUNT_SELECT);
    while(_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT) < stop);
}

void DelayUS(unsigned long u)
{
    unsigned long stop = u * (TICK_HZ / US) + _mfc0(_CP0_COUNT, _CP0_COUNT_SELECT);
    while(_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT) < stop);
}

void __ISR_AT_VECTOR(_EXTERNAL_1_VECTOR, IPL7SRS) ExternalInterrupt (void)
{
    IFS0bits.INT1IF = 0;
    LATBbits.LATB11 ^= 1;
    
    LATAbits.LATA3 ^= 1;
}

void InitializeInterrupt()
{
    IPC2bits.INT1IP = 7; // Primary Priority
    IPC2bits.INT1IS = 3; // Secondary Priority
    INTCONbits.INT1EP = 1; // Rising Edge
    IFS0bits.INT1IF = 0; // Reset Interrupt Flag
    INTCONSET = _INTCON_MVEC_MASK;
    IEC0bits.INT1IE = 1; // Enable Interrupt
    
    asm volatile("ei");

    ANSELEbits.ANSE8 = 0; // Digital
    TRISEbits.TRISE8 = 1; // Input

    INT1R = 0x000D; //http://www.mouser.com/ds/2/268/60001320D-967555.pdf Page 252
}

int main(void)
{
    TRISBbits.TRISB11 = 0; // Output for LED
    LATBbits.LATB11 = 0; // LED OFF
    
    InitializeInterrupt();

    TRISAbits.TRISA3 = 0; // Output for testing
    LATAbits.LATA3 = 0;
    
    while (1)
    {
        LATAbits.LATA10 ^= 0; 
    }
    return 0;
}