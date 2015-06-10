#include subroutinen.h

void kreisverkehr()
{
	while (barc_read == false)
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