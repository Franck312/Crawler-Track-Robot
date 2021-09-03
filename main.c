//#include<p18f2520.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include "MI2C.h"
#ifndef INIT_H_INCLUDED
#include "init.h"
#endif

#ifndef SUBPROGRAM_H_INCLUDED
#include "subprogram.h"
#endif


#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON


//#pragma config OSC = INTIO67
//#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON



//void init(void);
//void surveillanceBatterie(void);
//void telecomande(void);
//void arret(void);
//void lectureIR(void);
//void arretMoteurs(void);
//void HighISR(void);

//Déclaration des Flags
//unsigned char marche = 0; //type char pour optimiser l'utilisation de l'espace mémoire
//unsigned char marcheMoteurs = 0;
//float Ubat;
//unsigned int Distance1;
//unsigned int Distance2;
//unsigned char bouton[3];
//volatile unsigned char FlagSurvBat = 0;
//unsigned char FlagArret = 0;
//unsigned char FlagTel = 0;
//unsigned char FlagIR = 0;
//unsigned char FlagArretMoteurs = 0;
//unsigned char compteur = 0;
//unsigned int Distance;



//#pragma code HighVector=0x08
//
//void IntHighVector(void) {
//    _asm goto HighISR _endasm
//}
//#pragma code
//
//#pragma interrupt HighISR
//
//void HighISR(void) {
//
//    if (INTCONbits.INT0IF) {
//        INTCONbits.INT0IF = 0;
//        FlagTel = 1;
//    }
//
//    if (INTCONbits.TMR0IF) {
//        INTCONbits.TMR0IF = 0;
//        TMR0L = 0xDF;
//        TMR0H = 0xB1;
//        FlagIR = 1;
//        compteur++;
//
//
//    }
//
//    if(compteur>100){
//        FlagSurvBat=1;
//        compteur=0;
//    }
//
//
//}

void main(void) {
    init();

    while (1) {
        if (FlagSurvBat) {
            surveillanceBatterie();
        }
        if (FlagTel) {
            telecomande();
        }
        if (FlagArret) {
            arret();
        }
        if (marche) {
            if (FlagIR) {
                lectureIR();
            }
            if (FlagArretMoteurs) {
                arretMoteurs();
            }
        }
    }
}

//void init(void) {
//
//
//
////horloge
//OSCCONbits.IRCF=0b111; //8Mhz
//
//
//
////ADC
//ADCON1bits.VCFG = 0; // réglage Vref
//ADCON1bits.PCFG= 0b1100;// config AN0 AN1 AN2
//ADCON2bits.ADCS= 0b001;// Fosc/8
//ADCON2bits.ACQT= 0b010;// 4xT 010
//ADCON2bits.ADFM=0; // justification à gauche
//ADCON0bits.ADON= 1; // allumage
//
//
////Timer 0
//T0CONbits.TMR0ON=1;
//T0CONbits.T08BIT=0;//16 bits
//T0CONbits.T0CS=0;//Clock interne
//T0CONbits.PSA=1;//prescaler
//T0CONbits.T0PS=000;//Prescalar 4
//INTCONbits.TMR0IE=1;//enable
//TMR0L = 0xDF;
//TMR0H = 0xB1;
////overflow at (0,03s)
//
////Timer2 PWM
//
////T2CONbits.T2CKPS=16;//POST;;
//T2CONbits.T2CKPS = 0b10; //PRE CLK /4
//PR2 = 124; // Reglage periode FPWM = Fosc/(4*(PR2+1)*PRE) 1ms
//// Reglage rapport cyclique
//CCP1CONbits.DC1B0 = 0;
//CCP1CONbits.DC1B1 = 0;
//CCP2CONbits.DC2B0 = 0;
//CCP2CONbits.DC2B1 = 0;
////Selection Mode PWM
//CCP1CONbits.CCP1M3 = 1;
//CCP1CONbits.CCP1M2 = 1;
//CCP2CONbits.CCP2M3 = 1;
//CCP2CONbits.CCP2M2 = 1;
//CCPR1L = 0;
//CCPR2L = 0;
//
//
//
////Initialisation I2C
//MI2CInit();
//
//
////uart
//TXSTAbits.SYNC=0;//Asynchronous mode
//TXSTAbits.BRGH=1;//High speed
//BAUDCONbits.BRG16=0;//Baud rate 8 bits
//SPBRG=51; //9600
//RCSTAbits.SPEN=1;
//TXSTAbits.TXEN=1;//transmit enable
//PIR1bits.TXIF=0;
//PIE1bits.TXIE=0;
//RCSTAbits.CREN=1;//enable receiver
//
////ENTRE/SORTIE
//
//TRISCbits.RC6=0;//TX
//TRISCbits.RC7=1;//RX
//
//TRISAbits.RA6=0;//DIRD
//TRISAbits.RA7=0;//DIRG
//
//TRISBbits.RB0=1; //interruption télécommande
//TRISBbits.RB5=0;//LED
//
//TRISCbits.RC1=0;//PWM Gauche
//TRISCbits.RC2=0;//PWM droit
//
//PORTAbits.RA6=0; //direc
//PORTAbits.RA7=0;
//PORTBbits.RB5=1;//LED
//TRISAbits.RA2=1;//batt
//TRISBbits.RB1 = 0;
//
//// interrupt
//PIE1bits.TMR2IE=0;
//T2CONbits.TMR2ON = 1;
//INTCONbits.TMR0IE=1;
//INTCONbits.PEIE = 1;
//INTCONbits.INT0IE = 1;
//PIE1bits.ADIE=0;
//INTCONbits.GIE = 1;
//
//
//printf("Fin d'initialisation ! \r");
//}

//void surveillanceBatterie(void) {
//    //calcul la tension de la batterie et modifie le flag d'arret
//    int i = 0;
//
//    FlagSurvBat = 0; //remise du Flag à 0
//    Ubat = 0;
//
//    // Selection du port ADC AN2 correspondant à SURVBAT
//    ADCON0bits.CHS3 = 0;
//    ADCON0bits.CHS2 = 0;
//    ADCON0bits.CHS1 = 1;
//    ADCON0bits.CHS0 = 0;
//
//    for (i = 0; i < 4; i++) {
//        ADCON0bits.GO = 1;
//        while (ADCON0bits.GO_DONE) {
//            //on attend que la recuperation des données soit terminé
//        }
//        Ubat += ADRESH;
//    }
//
//    Ubat = Ubat / 4;
//
//    if (Ubat < 160 && marche)
//    {
//        FlagArret = 1;
//        printf("Tension trop basse \r");
//    }
//
//    Ubat = (Ubat*5)/255;
//
//    printf("Ubatt: %d.%d Etat : %hd\r\n",(int)Ubat,(int)((Ubat - (int)Ubat)*10),marche);
//    //print via RS232
//}
//
//void arret(void) {
//    //modifie l'état des moteurs et des capteurs (switch arret/marche)
//    FlagArret = 0; //remise du Flag à 0
//    //printf("marche avant %hd\n",marche);
//    marche = !marche;
//    marcheMoteurs = marche;
//    //printf("marche apres %hd\n",marche);
//    if (marche) {
//        printf("Demarrage du robot \r");
//        CCPR1L = 30;
//        CCPR2L = 30;
//        PORTBbits.RB1 = 0;
//    } else {
//        printf("Arret du robot \r");
//        CCPR1L = 0;
//        CCPR2L = 0;
//        PORTBbits.RB1 = 1;
//    }
//     //printf("ARRET TOTAL\n");
//    //print via RS232
//}
//
//void lectureIR(void) {
//    //lit les tensions retournées par les capteurs IR et modifie le flag arretMoteur
//    int i;
//    int j = 0;
//    Distance1 = 0;
//    Distance2 = 0;
//    Distance=0;
//
//    FlagIR = 0; //remise du Flag à 0
//
//    // Selection du port ADC AN0 correspondant à CAPTD
//
//
//    while(j<10){
//        // Selection du port ADC AN0 correspondant à CAPTD
//        ADCON0bits.CHS3 = 0;
//        ADCON0bits.CHS2 = 0;
//        ADCON0bits.CHS1 = 0;
//        ADCON0bits.CHS0 = 0;
//
//        ADCON0bits.GO = 1;
//        while (ADCON0bits.GO_DONE) {
//        //on attend que la recuperation des données soit terminé
//         }
//
//        Distance1 = ADRESH;
//
//        ADCON0bits.CHS3 = 0;
//        ADCON0bits.CHS2 = 0;
//        ADCON0bits.CHS1 = 0;
//        ADCON0bits.CHS0 = 1;
//
//        ADCON0bits.GO = 1;
//        while (ADCON0bits.GO_DONE) {
//        //on attend que la recuperation des données soit terminé
//        }
//        Distance2 = ADRESH;
//
//        if((Distance2-Distance1)<3){
//            Distance+=(Distance2 + Distance1)/2;
//            j++;
//        }
//    }
//
//    Distance = Distance/j;
//
//    printf("moyenne : %d  \r\n", Distance);
//
//
//
//    if ((Distance < 20 || Distance > 61 ) && marcheMoteurs ){
//        printf("Mauvaise distance \r\n");
//        FlagArretMoteurs = 1;
//    }
//    if ((Distance > 20 && Distance < 61 ) && !marcheMoteurs ){
//        printf("Bonne distance \r\n");
//        FlagArretMoteurs = 1;
//    }
//
//    Write_PCF8574(0x40, ~ADRESH);
//    //printf("marche moteur : %d", marcheMoteurs);
//
//}
//
//void telecomande(void) {
//    //lit la trame I²C recue par la telecomande et modifie le flag arret
//
//    FlagTel = 0; //remise du Flag à 0
//
//    Lire_i2c_Telecom(0xA2,(void*) bouton);
//
//    if (bouton[1] == 0x31){
//        printf("Interruption telecomande \r");
//        arret();
//
//    }
//}
//
//void arretMoteurs(void) {
//    //modifie l'état des moteur (switch arret/marche
//    FlagArretMoteurs = 0; //remise du Flag à 0
//    marcheMoteurs = !marcheMoteurs;
//    if (marcheMoteurs) {
//        CCPR1L = 30;
//        CCPR2L = 30;
//        printf("Demarrage moteurs\r");
//    }
//    else {
//        CCPR1L = 0;
//        CCPR2L = 0;
//        printf("Arret moteurs\r");
//    }
//    //print via RS232
//}
