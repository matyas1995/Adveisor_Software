#ifndef _SOFTWARE_H
#define _SOFTWARE_H
#define KREISVERKEHR_TOLERANZ 10
#include "hardware.h"


static int bc_counter; // Zaehler zum Einlesen des Barcodes
static int barc_read;

void treppe_rampe(void);

void kreisverkehr(void);

void barriere(void);

static void ir_interrupt();

#endif
