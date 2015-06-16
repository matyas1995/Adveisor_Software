#include "hardware.h"

/*
Diese Funktion muss im Setup() teil des Arduino Programms aufgerufen werden, um die Hardware zu initialisieren
*/
void hardware_setup(void)
{
  motor_right.setSpeed(255);
  motor_left.setSpeed(255);
  motor_right.run(RELEASE);
  motor_left.run(RELEASE);
  sensor1.trig_pin = FRONT_RIGHT_TRIG;
  sensor1.echo_pin = FRONT_RIGHT_ECHO;
  sensor2.trig_pin = BACK_RIGHT_TRIG;
  sensor2.echo_pin = BACK_RIGHT_ECHO;
  sensor3.trig_pin = FRONT_FRONT_TRIG;
  sensor3.echo_pin = FRONT_FRONT_ECHO;
  pinMode(sensor1.trig_pin, OUTPUT);
  pinMode(sensor1.echo_pin, INPUT);
  pinMode(sensor2.trig_pin, OUTPUT);
  pinMode(sensor2.echo_pin, INPUT);
  pinMode(sensor3.echo_pin, INPUT);
  pinMode(sensor3.trig_pin, OUTPUT);
}

/*
Diese Funktion ist aequivalent zur geradeaus_fahren Funktion, nur mit der Unterschied, dass ich hier versuche, keine Mischung von Englischen und Deutschen woertern zu verwenden
distance_from_middle bezeichnet die Abweichung nach LINKS vom Mittellinie
use_side beschreibt, auf welche Seite die Sensoren verwendet werden sollen
	0 ist links, 1 ist rechts (LEFT_SIDE und RIGHT_SIDE benutzen)
*/
void drive_straight(int distance_from_middle, char use_side, char wanted_speed)
{
  char motorA_speed = wanted_speed;
  char motorB_speed = wanted_speed;

  int tolerance_left;
  int tolerance_right;

  int target_distance;
  struct side_info side = get_side_info(use_side);		/*Abhaengig, welche Seite beruecksichtigt werden soll, lese die entsprechende Seite aus*/

  if (use_side)	/*falls  die Sensoren auf der rechten Seite beruecksichtigt werden sollen*/
  {
    //target_distance = (TRACK_WIDTH / 2) - (ROBOT_WIDTH) + distance_from_middle;		/*bestimme soll-entfernung vom Wand, basierend auf den Roboter- sowie Fahrbahnbreite und der gewÃ¼nschten Abweichung vom Mittellinie*/

    target_distance = distance_from_middle;    /*anscheinend will der arduino sich nur schlecht auf diese weise steuern lassen weil die angaben ein bissl unpraezise sind

    /*Toleranz beruecksichtigen*/
    tolerance_left = target_distance + TOLERANCE;
    tolerance_right = target_distance - TOLERANCE;

    /*pruefen, ob roboter ausserhalb der Toleranzen ist und Motorengeschiwndigkeiten ggf. anpassen*/
    if (tolerance_right > side.average_distance)
    {
      motorA_speed = regulate_speed(motorA_speed, target_distance, side.average_distance);		/*Regelalgorithmus fuer Motorengeschwindigkeit ist in ein separates Funktion damit man verschiedene Regelalgorithmen testen kann*/
    }
    else if (tolerance_left < side.average_distance)
    {
      motorB_speed = regulate_speed(motorB_speed, target_distance, side.average_distance);
    }

    /*Winkel zur Wand Pruefen und ggf. gegensteuern*/

    if (side.angle > 0)
    {
      motorB_speed = regulate_angle(motorB_speed, side.angle);
    }
    else if (side.angle < 0)
    {
      motorA_speed = regulate_angle(motorA_speed, side.angle);
    }

  }
  else			/*falls die Sensoren auf der linken Seite beruecksichtigt werden sollen*/
  {
    //target_distance = (TRACK_WIDTH / 2) - (ROBOT_WIDTH) - distance_from_middle;		/*bestimme soll-entfernung vom Wand, basierend auf den Roboter- sowie Fahrbahnbreite und der gewÃ¼nschten Abweichung vom Mittellinie*/

    target_distance = distance_from_middle;

    /*Toleranz beruecksichtigen*/
    tolerance_left = target_distance - TOLERANCE;
    tolerance_right = target_distance + TOLERANCE;

    /*pruefen, ob roboter ausserhalb der Toleranzen ist und Motorengeschiwndigkeiten ggf. anpassen*/
    if (tolerance_right < side.average_distance)
    {
      motorB_speed = regulate_speed(motorB_speed, target_distance, side.average_distance);
    }
    else if (tolerance_left > side.average_distance)
    {
      motorA_speed = regulate_speed(motorA_speed, target_distance, side.average_distance);
    }

    /*Winkel zur Wand Pruefen und ggf. gegensteuern*/

    if (side.angle > 0)
    {
      motorA_speed = regulate_angle(motorA_speed, side.angle);
    }
    else if (side.angle < 0)
    {
      motorB_speed = regulate_angle(motorB_speed, side.angle);
    }
  }

  drive(1, motorA_speed, motorB_speed);
}

/*
Diese Funktion bestimmt die Motorengeschwindigkeit anhand der Winkel in dem der Roboter auf den Wand steht.
Die quadratische Korrektur ist nötig da wir im Moment nur eine Sehr vage Winkelmessung hinbekommen
*/
int regulate_angle(char motor_speed, int angle)
{
  motor_speed -= pow(angle, 2) / HARDNESS;
  if (motor_speed < 50)
  {
    motor_speed = 50;
  }
  return motor_speed;
}

/*
diese Funktion bestimmt die Motorgeschwindigkeit anhand der Abweichung vom vorgegebenen Mittelwert
hier koennen verschiedene Regelalgorithmen implementiert werden
*/
int regulate_speed(char motor_speed, int target_distance, int distance)
{
  motor_speed -= (distance - target_distance) * HARDNESS;
  if (motor_speed < 50)
  {
    motor_speed = 50;
  }
  return motor_speed;
}

/*
Diese Funktion ist da, um das Roboter fahren lassen zu koennen
direction steht fuer Richtung, 1 ist vorwaerts, 0 fuer rueckwaerts
*/
void drive(char direction, char motorA_speed, char motorB_speed)
{
  motor_right.setSpeed(motorA_speed);
    motor_left.setSpeed(motorB_speed);
  if (direction)						/*Richtung setzen*/
  {
    motor_right.run(FORWARD);
    motor_left.run(FORWARD);
  }
  else
  {
    motor_right.run(BACKWARD);
    motor_left.run(BACKWARD);
  }
}

/*
Diese Funktion dreht den Motor in die angegebene Richtung (1 fuer Rechtsdreh) um ungefaehr den angegebenen winkel in Grad
*/
void turn(char direction, short degree)
{
  int duration = (int)(MILLIS_PER_DEGREE * (float)degree);

  motor_right.setSpeed(255);
  motor_left.setSpeed(255);

  if (direction)
  {
    motor_right.run(BACKWARD);
    motor_left.run(FORWARD);
  }
  else
  {
    motor_right.run(FORWARD);
    motor_left.run(BACKWARD);
  }

  delay(duration);

  stop_motors();
}

/*
Motoren stoppen. Geschwindigkeit wird einfach auf 0 gesetzt. Wegen der Getriebe muss nicht aktiv gebremst werden
*/
void stop_motors(void)
{
  motor_right.run(RELEASE);
  motor_left.run(RELEASE);
}


/*
Liefert Abstand- und Winkelinformationen zu eine Seite in ein "side_info" struct zurueck
aufpassen, kann sein, dass hier malloc und pointern benÃ¶tigt werden! -> zumindest waere es von der Speicherverbrauch her besser
*/
struct side_info get_side_info(char side)
{
  struct side_info result;	/*struct in dem das Ergebnis gespeichert wird*/

  struct sensor sensors[2];		/*array zum speichern der Sensorpins auf der benoetigten Seite*/
  int sensorDist[2];		/*array zum speichern der gemessenen Sensorwerte*/

  if (side)                     /*falls rechte Seite benutzt werden soll*/
  {
    sensors[0] = sensor1;
    sensors[1] = sensor2;
  }
  else
  {
    /*Muss dringend geändert werden!!!!!!*/
    sensors[0] = sensor1;
    sensors[1] = sensor2;
  }
  int i = 0;
  for (i = 0; i < 2; i++)		/*auslesen der sensoren und werte in array speichern*/
  {
    sensorDist[i] = get_dist(sensors[i]);
  }

  result.average_distance = calc_average(sensorDist, 2);	/*Durchschnittlicher Entfernung vom Wand wird berechnet und im Ergebnis gespeichert*/
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
  int opposite = values[0] - values[1];		/*Wegunterschied zwischen den hinten und vorne gemessenen Entfernung*/

  /*Situation 1*/
  //double angle = atan2((double)opposite, (double)DIST_IR_FRONT_BACK);

  /*da atan2 winkel in radian liefert, muessen die noh in grad konvertiert werden*/

 // angle = angle / (2.0 * PI) * 360.0 * 10;
 
 

 // return (int)angle;
 
 return opposite;
}

/*
Diese Funktion liest analoge Werte der IR Sensoren aus und berechnet davon die Entfernung
Die zurueckgelieferte Werte muessen in Millimetern erfolgen!
*/
int get_dist(struct sensor Sensor)
{
  long duration = 0;

  digitalWrite(Sensor.trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Sensor.trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Sensor.trig_pin, LOW);
  duration = pulseIn(Sensor.echo_pin, HIGH);
  duration = (duration / 2.0) / 2.91;
  return (int)duration;
}

