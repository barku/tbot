/*****************************************************************************/
/*                                  COMPANY                                  */
/*                              www.COMPANY.com                              */
/*                   (C) Copyright 2016. All Rights Reserved.                */
/*                                                                           */
/* File name : tbot.ino                                                      */
/* Author    : author name (author_email@email.com)                          */
/* Purpose   : tracked bot's main entry                                      */
/*                                                                           */
/* Revision :                                                                */
/*    v0.1 : 160312 created by                                               */
/*                                                                           */
/*****************************************************************************/

#include "config.h"

#include "mainBarku.h"

//******************************************************************************

#if 0
void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    int frac1 = frac;
    while( frac1 /= 10 )
        precision /= 10;
    precision /= 10;
    while(  precision /= 10)
        Serial.print("0");

    Serial.print(frac,DEC) ;
}
#endif // #if 0

//******************************************************************************

void setup()
{
    Serial.begin(9600);                          // configure serial port baud rate and start serial port
    Serial.println("");
    Serial.println(".ABC.");

#ifdef MAIN_BARKU
    setupBarku();
#endif // #ifdef MAIN_BARKU

    Serial.println("-----");
}


void loop()
{

#ifdef MAIN_BARKU
    loopBarku();
#endif // #ifdef MAIN_BARKU

}

/******************************** END OF FILE *********************************/

