#include "subroutinen.h"
#include <Arduino.h>

void treppe_rampe()
{
  while (barc_read == 0)
  {
    drive_straight(0, RIGHT_SIDE, 120);
  }
}

void kreisverkehr()
{
  while (barc_read == 0)
  {
    drive_straight(0, RIGHT_SIDE, 120);
    struct side_info right = get_side_info(RIGHT_SIDE);		/*Seiteninfo auslesen evtl. zur manuellen korrektur*/
    /*
    Alternative 1:
    */
    /*
    if (right.angle > KREISVERKEHR_TOLERANZ)
    {
    	drehen(1, 45);
    }
    */

    /*
    Alternative 2
    Aenderungsvorschlag:

    wert des vorderen Sensors in side_info mit uebergeben, damit es hier beruecksichtigt werden kann.
    damit kann man den Roboter schon vorzeitig gegensteuern lassen, falls es schon zu nahe an die Wand kommt.
    */
    /*
    if (right.front < irgendein_minimalwert)
    {
    	drehen(1, 45);
    }
    */
  }
}

void barriere()
{
  while (barc_read == 0)
  {
    int d1, d2;

    d1 = digitalRead(IR_FRONT_LEFT);
    d2 = digitalRead(IR_FRONT_RIGHT);

    if (d1 == 1 || d2 == 1)
      stop_motors();
    else
      drive_straight(0, RIGHT_SIDE, 120);
  }
}

/*Interrupt zur Barcodeauslese*/

void ir_interrupt()
{
  /*TC_Write_RA (TC1, 0, 0); //Setzt den Timer zurueck
  TC_Write_RB (TC1, 0, 0); //Ich wusste leider nicht auf welches Regiser (a, b oder c) geschrieben wird und hab daher zur Sicherheit einfach alle abgedeckt
  TC_Write_RC (TC1, 0, 0);
  
  TC_GetStatus(TC2, 1);

  TC_Start(TC2, 0); // Startet den Timer
  */
  
  Timer3.stop();
  BC_TIMER * 100;

  bc_counter++;
}
