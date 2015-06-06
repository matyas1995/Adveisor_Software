#include "hardware.h"

/*
Diese Funktion muss im Setup() teil des Arduino Programms aufgerufen werden, um die Hardware zu initialisieren
*/
void hardware_setup(void)
{
  pinMode(MOTOR_A_BRAKE, OUTPUT);
  pinMode(MOTOR_B_BRAKE, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);
  pinMode(IR_FRONT_LEFT, INPUT);
  pinMode(IR_FRONT_RIGHT, INPUT);
  pinMode(IR_BACK_LEFT, INPUT);
  pinMode(IR_BACK_RIGHT, INPUT);
}

/*
Diese Funktion ist aequivalent zur geradeaus_fahren Funktion, nur mit der Unterschied, dass ich hier versuche, keine Mischung von Englischen und Deutschen woertern zu verwenden
distance_from_middle bezeichnet die Abweichung nach LINKS vom Mittellinie
use_side beschreibt, auf welche Seite die Sensoren verwendet werden sollen
	0 ist links, 1 ist rechts (LEFT_SIDE und RIGHT_SIDE benutzen)
*/
void drive_straight(int distance_from_middle, char use_side)
{
  char motorA_speed = 255;
  char motorB_speed = 255;

  int tolerance_left;
  int tolerance_right;

  int target_distance;
  struct side_info side = get_side_info(use_side);		/*Abhaengig, welche Seite beruecksichtigt werden soll, lese die entsprechende Seite aus*/

  if (use_side)	/*falls  die Sensoren auf der rechten Seite beruecksichtigt werden sollen*/
  {
    target_distance = (TRACK_WIDTH / 2) - (ROBOT_WIDTH) + distance_from_middle;		/*bestimme soll-entfernung vom Wand, basierend auf den Roboter- sowie Fahrbahnbreite und der gewÃ¼nschten Abweichung vom Mittellinie*/

    /*Toleranz beruecksichtigen*/
    tolerance_left = target_distance + TOLERANCE;
    tolerance_right = target_distance - TOLERANCE;

    /*pruefen, ob roboter ausserhalb der Toleranzen ist und Motorengeschiwndigkeiten ggf. anpassen*/
    if (tolerance_right > side.average_distance)
    {
      motorA_speed = regulate(motorA_speed, target_distance, side.average_distance);		/*Regelalgorithmus fuer Motorengeschwindigkeit ist in ein separates Funktion damit man verschiedene Regelalgorithmen testen kann*/
    }
    else if (tolerance_left < side.average_distance)
    {
      motorB_speed = regulate(motorB_speed, target_distance, side.average_distance);
    }

    /*Winker zur Wand Pruefen und ggf. gegensteuern

    }
    else			/*falls die Sensoren auf der linken Seite beruecksichtigt werden sollen*/
    {
      target_distance = (TRACK_WIDTH / 2) - (ROBOT_WIDTH) - distance_from_middle;		/*bestimme soll-entfernung vom Wand, basierend auf den Roboter- sowie Fahrbahnbreite und der gewÃ¼nschten Abweichung vom Mittellinie*/

      /*Toleranz beruecksichtigen*/
      tolerance_left = target_distance - TOLERANCE;
      tolerance_right = target_distance + TOLERANCE;

      /*pruefen, ob roboter ausserhalb der Toleranzen ist und Motorengeschiwndigkeiten ggf. anpassen*/
      if (tolerance_right < side.average_distance)
      {
        motorB_speed = regulate(motorB_speed, target_distance, side.average_distance);
      }
      else if (tolerance_left > side.average_distance)
      {
        motorA_speed = regulate(motorA_speed, target_distance, side.average_distance);
      }
    }

    drive(1, motorA_speed, motorB_speed);
  }
}

/*
diese Funktion bestimmt die Motorgeschwindigkeit anhand der Abweichung vom vorgegebenen Mittelwert
hier koennen verschiedene Regelalgorithmen implementiert werden
*/
int regulate(char motor_speed, int target_distance, int distance)
{
  motor_speed -= (distance - target_distance) * HARDNESS;
  return motor_speed;
}

/*
Diese Funktion ist da, um das Roboter fahren lassen zu koennen
direction steht fuer Richtung, 1 ist vorwaerts, 0 fuer rueckwaerts
*/
void drive(char direction, char motorA_speed, char motorB_speed)
{
  digitalWrite(MOTOR_A_BRAKE, LOW);	/*Bremsen loesen*/
  digitalWrite(MOTOR_B_BRAKE, LOW);

  if (direction)						/*Richtung setzen*/
  {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, HIGH);
  }
  else
  {
    digitalWrite(MOTOR_A_DIR, LOW);
    digitalWrite(MOTOR_B_DIR, LOW);
  }

  analogWrite(MOTOR_A_PWM, motorA_speed);
  analogWrite(MOTOR_B_PWM, motorB_speed);
}

/*
Diese Funktion dreht den Motor in die angegebene Richtung (1 fÃ¼r Rechtsdreh) um ungefaehr den angegebenen winkel in Grad
*/
void turn(char direction, short degree)
{
  int duration = (int)(MILLIS_PER_DEGREE * (float)degree);

  digitalWrite(MOTOR_A_BRAKE, LOW);	/*Bremsen loesen*/
  digitalWrite(MOTOR_B_BRAKE, LOW);

  if (direction)
  {
    digitalWrite(MOTOR_A_DIR, LOW);
    digitalWrite(MOTOR_B_DIR, HIGH);
  }
  else
  {
    digitalWrite(MOTOR_A_DIR, HIGH);
    digitalWrite(MOTOR_B_DIR, LOW);
  }

  analogWrite(MOTOR_A_PWM, 255);
  analogWrite(MOTOR_B_PWM, 255);

  delay(duration);

  stop_motors();
}

/*
Motoren stoppen. Geschwindigkeit wird einfach auf 0 gesetzt. Wegen der Getriebe muss nicht aktiv gebremst werden
*/
void stop_motors(void)
{
  analogWrite(MOTOR_A_PWM, 0);
  analogWrite(MOTOR_B_PWM, 0);
}


/*
Liefert Abstand- und Winkelinformationen zu eine Seite in ein "side_info" struct zurueck
aufpassen, kann sein, dass hier malloc und pointern benÃ¶tigt werden! -> zumindest waere es von der Speicherverbrauch her besser
*/
struct side_info get_side_info(char side)
{
  struct side_info result;	/*struct in dem das Ergebnis gespeichert wird*/

  int sensorPins[3];		/*array zum speichern der Sensorpins auf der benoetigten Seite*/
  int sensorDist[3];		/*array zum speichern der gemessenen Sensorwerte*/

  if (side)                     /*falls rechte Seite benutzt werden soll*/
  {
    sensorPins[0] = IR_1;
    sensorPins[1] = IR_2;
    sensorPins[2] = IR_3;
  }
  else
  {
    sensorPins[0] = IR_4;
    sensorPins[1] = IR_5;
    sensorPins[2] = IR_6;
  }
  int i = 0;
  for (i = 0; i < 3; i++)		/*auslesen der sensoren und werte in array speichern*/
  {
    sensorDist[i] = get_ir_dist(sensorPins[i]);
  }

  result.average_distance = calc_average(sensorDist, 3);	/*Durchschnittlicher Entfernung vom Wand wird berechnet und im Ergebnis gespeichert*/
  result.angle = calc_angle(sensorDist);				/*Berechne winkel zwischen Roboter und den Wand*/

  return result;
}

/*
Diese Funktion berechnet den Mittelwert aus drei integern und gibt es als integer zurÃ¼ck
*/
int calc_average(int values[], int size_of_array)
{
  int sum = 0;
  int i = 0;

  for (i = 0; i < size_of_array; i++)
  {
    sum += values[i];
  }

  return (sum / size_of_array);
}

/*
Diese funktion berechnet den Winkel zwischen Roboter und Wand und liefert einen Wert in Grad zurÃ¼ck
Falls Winkel positiv ist, bedeutet das, dass Roboter in Richtung Wand steht
Falls Winkel negaitv ist, Roboter steht in Richtung weg von der Wand

Berechnung erfolg entweder Basierend auf Situation 1 oder 2 (Bilder im Ordner) abhaengig davon, wie Sensoren Distanz messen, hier muss der Eine auskommentiert werden
Die Verwendung von Pointern verschwendet weniger RAM
*/
int calc_angle(int values[])
{
  int opposite = values[2] - values[0];		/*Wegunterschied zwischen den hinten und vorne gemessenen Entfernung*/

  /*Situation 1*/
  double angle = atan2((double)opposite, (double)DIST_IR_FRONT_BACK);
  
  /*da atan2 winkel in radian liefert, muessen die noh in grad konvertiert werden*/

  angle = angle / (2.0 * PI) * 360.0;

  return (int)angle;
}

/*
Diese Funktion liest analoge Werte der IR Sensoren aus und berechnet davon die Entfernung
Die zurueckgelieferte Werte muessen in Millimetern erfolgen!
*/
int get_ir_dist(int pin)
{
  int voltage = analogRead(pin);
  float distance = sqrt(1.0 / voltage);
  int dist = (int)(distance * 1000.0);
  return dist;
}
