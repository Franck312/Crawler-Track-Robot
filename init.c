#include "init.h"


void init(void) {



//horloge
OSCCONbits.IRCF=0b111; //8Mhz



//ADC
ADCON1bits.VCFG = 0; // réglage Vref
ADCON1bits.PCFG= 0b1100;// config AN0 AN1 AN2
ADCON2bits.ADCS= 0b001;// Fosc/8
ADCON2bits.ACQT= 0b010;// 4xT 010
ADCON2bits.ADFM=0; // justification à gauche
ADCON0bits.ADON= 1; // allumage


//Timer 0
T0CONbits.TMR0ON=1;
T0CONbits.T08BIT=0;//16 bits
T0CONbits.T0CS=0;//Clock interne
T0CONbits.PSA=1;//prescaler
T0CONbits.T0PS=000;//Prescalar 4
INTCONbits.TMR0IE=1;//enable
TMR0L = 0xDF;
TMR0H = 0xB1;
//overflow at (0,03s)

//Timer2 PWM

//T2CONbits.T2CKPS=16;//POST;;
T2CONbits.T2CKPS = 0b10; //PRE CLK /4
PR2 = 124; // Reglage periode FPWM = Fosc/(4*(PR2+1)*PRE) 1ms
// Reglage rapport cyclique
CCP1CONbits.DC1B0 = 0;
CCP1CONbits.DC1B1 = 0;
CCP2CONbits.DC2B0 = 0;
CCP2CONbits.DC2B1 = 0;
//Selection Mode PWM
CCP1CONbits.CCP1M3 = 1;
CCP1CONbits.CCP1M2 = 1;
CCP2CONbits.CCP2M3 = 1;
CCP2CONbits.CCP2M2 = 1;
CCPR1L = 0;
CCPR2L = 0;



//Initialisation I2C
MI2CInit();


//uart
TXSTAbits.SYNC=0;//Asynchronous mode
TXSTAbits.BRGH=1;//High speed
BAUDCONbits.BRG16=0;//Baud rate 8 bits
SPBRG=51; //9600
RCSTAbits.SPEN=1;
TXSTAbits.TXEN=1;//transmit enable
PIR1bits.TXIF=0;
PIE1bits.TXIE=0;
RCSTAbits.CREN=1;//enable receiver

//ENTRE/SORTIE

TRISCbits.RC6=0;//TX
TRISCbits.RC7=1;//RX

TRISAbits.RA6=0;//DIRD
TRISAbits.RA7=0;//DIRG

TRISBbits.RB0=1; //interruption télécommande
TRISBbits.RB5=0;//LED

TRISCbits.RC1=0;//PWM Gauche
TRISCbits.RC2=0;//PWM droit

PORTAbits.RA6=0; //direc
PORTAbits.RA7=0;
PORTBbits.RB5=1;//LED
TRISAbits.RA2=1;//batt
TRISBbits.RB1 = 0;

// interrupt
PIE1bits.TMR2IE=0;
T2CONbits.TMR2ON = 1;
INTCONbits.TMR0IE=1;
INTCONbits.PEIE = 1;
INTCONbits.INT0IE = 1;
PIE1bits.ADIE=0;
INTCONbits.GIE = 1;


//printf("Fin d'initialisation ! \r\n");
}