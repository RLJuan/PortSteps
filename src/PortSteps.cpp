/*
*
*	Lib para el control de motores paso a paso unipolares de 4 hilos + GND 
*	mediante escritura rápida en puerto (B,C,D).
* 
*	@autor: Juan R. L.
*
*/

#include "Arduino.h"
#include "PortSteps.h"


void PortStep::setPort(volatile unsigned char &port, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	//Valores iniciales

	RESETSTEPS(_steps);
	p[0] = a; p[1] = b; p[2] = c; p[3] = d; this->port = &port;
	outputMask = ~PINCONFIG(p[0], p[1], p[2], p[3]);

	//Direccionamiento

		 if(this->port == &PORTD) PORTCONFIG(DDRD, a, b, c, d);
	else if(this->port == &PORTC) PORTCONFIG(DDRC, a, b, c, d);
	else if(this->port == &PORTB) PORTCONFIG(DDRB, a, b, c, d);

	setSequence(MIXED);
}

void PortStep::setSequence(unsigned char sequence)
{
	if (sequence == SIMPLE)
	{
		maxSteps = 4;
		for (int i = 0; i < maxSteps; i++) _steps[i] = SIMPLESTEP(p[i]);
	}
	else if (sequence == WAVE)
	{
		maxSteps = 4;
		for (int i = 0; i < maxSteps; i++) _steps[i] = WAVESTEP(p[i], p[i+1 < maxSteps ? i+1 : 0]);
	}
	else
	{
		maxSteps = 8;
		;	_steps[0] = SIMPLESTEP(p[0])	;	_steps[1] = WAVESTEP(p[0], p[1])	;
		;	_steps[2] = SIMPLESTEP(p[1])	;	_steps[3] = WAVESTEP(p[1], p[2])	;
		;	_steps[4] = SIMPLESTEP(p[2])	;	_steps[5] = WAVESTEP(p[2], p[3])	;
		;	_steps[6] = SIMPLESTEP(p[3])	;	_steps[7] = WAVESTEP(p[3], p[0])	;
	}
}

void PortStep::setStepDelay(unsigned int period, delayUnit MSorUS)
{
	this->period = period;
	stepDelay = MSorUS ? DELAYMICROSECONDS : DELAYMILLISECONDS;
}

void PortStep::setRotation(stepRotation LEFTorRIGHT) 
{ 
	nextStep = LEFTorRIGHT ? stepRight : stepLeft;
}

stepRotation PortStep::getRotation()
{
	return nextStep == stepRight;
}

void PortStep::run(int steps) { for(int i=0; i<=steps; i++) stepSync(); }
void PortStep::stepSync() { step(); stepDelay(period); }
void PortStep::step() { nextStep(this); writeStep(); }
