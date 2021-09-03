
#ifndef SUBPROGRAM_H_INCLUDED
#define SUBPROGRAM_H_INCLUDED

//#ifndef MI2C_H_INCLUDED
#include "MI2C.h"
//#endif

#include<p18f2520.h>
#include <stdlib.h>
#include <stdio.h>

void surveillanceBatterie(void);
void telecomande(void);
void arret(void);
void lectureIR(void);
void arretMoteurs(void);
void HighISR(void);

//Déclaration des Flags
extern unsigned char marche; //type char pour optimiser l'utilisation de l'espace mémoire
extern unsigned char marcheMoteurs;
extern float Ubat;
extern unsigned int Distance1;
extern unsigned int Distance2;
extern unsigned char bouton[3];
extern volatile unsigned char FlagSurvBat ;
extern unsigned char FlagArret ;
extern unsigned char FlagTel ;
extern unsigned char FlagIR ;
extern unsigned char FlagArretMoteurs ;
extern unsigned char compteur ;
extern unsigned int Distance;

#endif