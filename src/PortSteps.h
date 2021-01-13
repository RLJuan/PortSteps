/*
*
*	Lib para el control de motores paso a paso unipolares de 4 hilos + GND
*	mediante escritura rápida en puerto (B,C,D).
*
*	@autor: Juan R. L.
*
*/

#ifndef PortSteps_h
#define PortSteps_h

/* ------------------------------ MACROS ------------------------------ */

// valores de delayUnit

#define MS false
#define US true

// valores de stepRotation

#define LEFT false
#define RIGHT true

// tipos de secuencia

#define SIMPLE 0	// pasos simples: (OOOX) - (OOXO) - (OXOO) - (XOOO) 
#define WAVE 1		// pasos en onda: (OOXX) - (OXXO) - (XXOO) - (XOOX) 
#define MIXED 2		// mixto (medios-pasos). Se obtiene el doble de pasos: (OOOX) - (OOXX) - (OOXO)...

#define PINCONFIG(A,B,C,D) ((1<<A)|(1<<B)|(1<<C)|(1<<D))
#define PORTCONFIG(DPORT,A,B,C,D) DPORT |= PINCONFIG(A,B,C,D)
#define SIMPLESTEP(A) 1<<A
#define WAVESTEP(A,B) 1<<B|1<<A
#define RESETSTEPS(STEPS) for(int i=0; i<8; i++) STEPS[i] = B00000000
#define DELAYMICROSECONDS delayMicroseconds
#define DELAYMILLISECONDS [](unsigned int value) -> void { delay(value); }

/* ---------------------------- FIN MACROS ---------------------------- */


typedef bool delayUnit;
typedef bool stepRotation;

struct PortStep
{
	private:

		//Puerto y pines: deben definirse en el programa mediante setPort(...)

		volatile unsigned char *port = NULL;
		unsigned char p[4];

		//Pointers a función

		void (*stepDelay)(unsigned int unit) = DELAYMICROSECONDS; //por defecto apunta a microsegundos
		void (*nextStep)(PortStep*) = stepRight;

		//Vars

		unsigned char _step = 0, maxSteps = 8;
		byte _steps[8], outputMask;
		unsigned int period = 1000; //por defecto 1 milisegundo

		//Funciones inline

		inline void writeStep() { *port = *port & outputMask | _steps[_step]; }
		inline static void stepLeft(PortStep *_this) { _this->_step++; if (_this->_step>= _this->maxSteps) _this->_step = 0; }
		inline static void stepRight(PortStep *_this) { if (_this->_step<=0) _this->_step = _this->maxSteps; _this->_step--; }

	public:

		/*	
			->	"setPort" permite habilitar los pines (a,b,c,d) del puerto (&port) que controlarán el motor.
			->	"setStepDelay" establece el periodo (period) entre pasos y su unidad (MS ó US).
			->	"setSequence" establece la secuencia (SIMPLE, WAVE ó MIXED). Ver en macros: tipos de secuencia.
			->	"setRotation" establece el sentido de giro del motor (LEFT ó RIGHT).
			->	"getRotation" obtiene el sentido de giro del motor (LEFT ó RIGHT).
			->	"run" hace girar el motor los pasos (steps) requeridos.
			->	"stepSync" permite dar un paso con retraso final (para poder usarlo en bucle).
			->	"step" permite dar un paso (sin retraso final).
		*/

		void setPort(volatile unsigned char &_port, unsigned char _a, unsigned char _b, unsigned char _c, unsigned char _d);
		void setStepDelay(unsigned int period, delayUnit MSorUS);
		void setSequence(unsigned char sequence);
		void setRotation(stepRotation LEFTorRIGHT);
		stepRotation getRotation();
		void run(int steps);
		void stepSync();
		void step();

};

inline String credits() { return "Juan R. L."; }

#endif
