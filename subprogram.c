#include "subprogram.h"

unsigned char marche = 0; //type char pour optimiser l'utilisation de l'espace mémoire
unsigned char marcheMoteurs = 0;
float Ubat;
unsigned int Distance1;
unsigned int Distance2;
unsigned char bouton[3];
volatile unsigned char FlagSurvBat = 0;
unsigned char FlagArret = 0;
unsigned char FlagTel = 0;
unsigned char FlagIR = 0;
unsigned char FlagArretMoteurs = 0;
unsigned char compteur = 0;
unsigned int Distance;

#pragma code HighVector=0x08

void IntHighVector(void) {
    _asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR

void HighISR(void) {

    if (INTCONbits.INT0IF) {
        INTCONbits.INT0IF = 0;
        FlagTel = 1;
    }

    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        TMR0L = 0xDF;
        TMR0H = 0xB1;
        FlagIR = 1;
        compteur++;


    }

    if(compteur>100){
        FlagSurvBat=1;
        compteur=0;
    }


}

void surveillanceBatterie(void) {
    //calcul la tension de la batterie et modifie le flag d'arret
    int i = 0;

    FlagSurvBat = 0; //remise du Flag à 0
    Ubat = 0;

    // Selection du port ADC AN2 correspondant à SURVBAT
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 1;
    ADCON0bits.CHS0 = 0;

    for (i = 0; i < 4; i++) {
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO_DONE) {
            //on attend que la recuperation des données soit terminé
        }
        Ubat += ADRESH;
    }

    Ubat = Ubat / 4;

    if (Ubat < 160 && marche)
    {
        FlagArret = 1;
        printf("Tension trop basse \r");
    }

    Ubat = (Ubat*5)/255;

    printf("Ubatt: %d.%d Etat : %hd\r\n",(int)Ubat,(int)((Ubat - (int)Ubat)*10),marche);
    //print via RS232
}

void arret(void) {
    //modifie l'état des moteurs et des capteurs (switch arret/marche)
    FlagArret = 0; //remise du Flag à 0
    //printf("marche avant %hd\n",marche);
    marche = !marche;
    marcheMoteurs = marche;
    //printf("marche apres %hd\n",marche);
    if (marche) {
        printf("Demarrage du robot \r");
        CCPR1L = 30;
        CCPR2L = 30;
        PORTBbits.RB1 = 0;
    } else {
        printf("Arret du robot \r");
        CCPR1L = 0;
        CCPR2L = 0;
        PORTBbits.RB1 = 1;
    }
     //printf("ARRET TOTAL\n");
    //print via RS232
}

void lectureIR(void) {
    //lit les tensions retournées par les capteurs IR et modifie le flag arretMoteur
    int i;
    int j = 0;
    Distance1 = 0;
    Distance2 = 0;
    Distance=0;

    FlagIR = 0; //remise du Flag à 0

    // Selection du port ADC AN0 correspondant à CAPTD


    while(j<10){
        // Selection du port ADC AN0 correspondant à CAPTD
        ADCON0bits.CHS3 = 0;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 0;

        ADCON0bits.GO = 1;
        while (ADCON0bits.GO_DONE) {
        //on attend que la recuperation des données soit terminé
         }

        Distance1 = ADRESH;

        ADCON0bits.CHS3 = 0;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 1;

        ADCON0bits.GO = 1;
        while (ADCON0bits.GO_DONE) {
        //on attend que la recuperation des données soit terminé
        }
        Distance2 = ADRESH;

        if((Distance2-Distance1)<3){
            Distance+=(Distance2 + Distance1)/2;
            j++;
        }
    }

    Distance = Distance/j;

    printf("moyenne : %d  \r\n", Distance);



    if ((Distance < 20 || Distance > 61 ) && marcheMoteurs ){
        printf("Mauvaise distance \r\n");
        FlagArretMoteurs = 1;
    }
    if ((Distance > 20 && Distance < 61 ) && !marcheMoteurs ){
        printf("Bonne distance \r\n");
        FlagArretMoteurs = 1;
    }

    Write_PCF8574(0x40, ~ADRESH);
    //printf("marche moteur : %d", marcheMoteurs);

}

void telecomande(void) {
    //lit la trame I²C recue par la telecomande et modifie le flag arret

    FlagTel = 0; //remise du Flag à 0

    Lire_i2c_Telecom(0xA2,(void*) bouton);

    if (bouton[1] == 0x31){
        printf("Interruption telecomande \r");
        arret();

    }
}

void arretMoteurs(void) {
    //modifie l'état des moteur (switch arret/marche
    FlagArretMoteurs = 0; //remise du Flag à 0
    marcheMoteurs = !marcheMoteurs;
    if (marcheMoteurs) {
        CCPR1L = 30;
        CCPR2L = 30;
        printf("Demarrage moteurs\r");
    }
    else {
        CCPR1L = 0;
        CCPR2L = 0;
        printf("Arret moteurs\r");
    }
    //print via RS232
}