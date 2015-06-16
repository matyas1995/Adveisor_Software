/*sicherstellen, dass der header nur einmal eingebunden wird, um Compilerfehler vorzubeugen*/
#ifndef _HARDWARE_H
#define _HARDWARE_H

/*Alle Groessenangaben sind in Millimetern!!!*/
#include <math.h>
#include <Arduino.h>
#include <AFMotor.h>

/*niedrigste Sensornummer ist vorne, hoechste hinten*/
#define FRONT_RIGHT_TRIG 46
#define FRONT_RIGHT_ECHO 48
#define BACK_RIGHT_TRIG 30
#define BACK_RIGHT_ECHO 28
#define FRONT_FRONT_TRIG 41
#define FRONT_FRONT_ECHO 39
/*hier muss noch die Drehgeschwindigkeit der Roboters gemessen und eingetragen werden*/
#define MILLIS_PER_DEGREE 7.7777777777
#define LEFT_SIDE 0
#define RIGHT_SIDE 1
/*Abstand zwischen vorderen und hinteres Seiten-IR Sensor, wird fuer die Winkelbestimmung zur Wand benoetigt*/
#define DIST_IR_FRONT_BACK 140.0
/*Toleranz der Abweichung vom Mittellinie in mm*/
#define TOLERANCE 10
/*Parameter, der definiert, we hart gegengesteuert werden soll*/
#define HARDNESS 4
/*Bezeichnet die Breite des Roboters*/
#define ROBOT_WIDTH 150
/*Bezeichnet die Breite der Fahrbahn, damit eine variable Abweichung vom Mittellinie eingestellt werden kann*/
#define TRACK_WIDTH 300

static AF_DCMotor motor_right(2); //Motor an rechter Seite
static AF_DCMotor motor_left(1); //Motor an linker Seite

struct sensor
{
  int echo_pin;
  int trig_pin;
};

static struct sensor sensor1, sensor2, sensor3;

struct side_info
{
  int angle;
  int average_distance;
};

/*
Diese Funktion muss im Setup() teil des Arduino Programms aufgerufen werden, um die Hardware zu initialisieren
*/
void hardware_setup(void);

/*
Diese Funktion ist aequivalent zur geradeaus_fahren Funktion, nur mit der Unterschied, dass ich hier versuche, keine Mischung von Englischen und Deutschen woertern zu verwenden
distance_from_middle bezeichnet die Abweichung nach LINKS vom Mittellinie
use_side beschreibt, auf welche Seite die Sensoren verwendet werden sollen
0 ist links, 1 ist rechts (LEFT_SIDE und RIGHT_SIDE benutzen)
*/
void drive_straight(int, char, char);

/*
diese Funktion bestimmt die Motorgeschwindigkeit anhand der Abweichung vom vorgegebenen Mittelwert
hier koennen verschiedene Regelalgorithmen implementiert werden
*/
int regulate_speed(char, int, int);


/*
Diese Funktion bestimmt die Motorengeschwindigkeit anhand der Winkel in dem der Roboter auf den Wand steht.
Die quadratische Korrektur ist nötig da wir im Moment nur eine Sehr vage Winkelmessung hinbekommen
*/
int regulate_angle(char, int);

/*
Diese Funktion ist da, um das Roboter fahren lassen zu koennen
direction steht fuer Richtung, 1 ist vorwaerts, 0 fuer rueckwaerts
*/
void drive(char, char, char);

/*
Diese Funktion dreht den Motor in die angegebene Richtung (1 fÃ¼r Rechtsdreh) um ungefaehr den angegebenen winkel in Grad
*/
void turn(char, short);

/*
Motoren stoppen. Geschwindigkeit wird einfach auf 0 gesetzt. Wegen der Getriebe muss nicht aktiv gebremst werden
*/
void stop_motors(void);

/*
Liefert Abstand- und Winkelinformationen zu eine Seite in ein "side_info" struct zurueck
aufpassen, kann sein, dass hier malloc und pointern benÃ¶tigt werden! -> waere zumindest von der Speicherverbrauch her besser
*/
struct side_info get_side_info(char);

/*
Diese Funktion berechnet den Mittelwert aus drei integern und gibt es als integer zurÃ¼ck
*/
int calc_average(int[], int);

/*
Diese funktion berechnet den Winkel zwischen Roboter und Wand und liefert einen Wert in Grad zurÃ¼ck
Falls Winkel positiv ist, bedeutet das, dass Roboter in Richtung Wand steht
Falls Winkel negaitv ist, Roboter steht in Richtung weg von der Wand
*/
int calc_angle(int[]);

/*
Diese Funktion liest analoge Werte der IR Sensoren aus und berechnet davon die Entfernung
Die zurueckgelieferte Werte muessen in Millimetern erfolgen!
*/
int get_dist(struct sensor);

#endif

