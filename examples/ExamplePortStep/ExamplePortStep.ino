#include "PortSteps.h" 

PortStep motor;

void setup()
{
  motor.setPort(PORTD, 4, 5, 6, 7); // Pines 4, 5, 6 y 7 del puerto D.
  motor.setStepDelay(2, MS);  //2 ms ó 2000us. Puede usarse microsegundos ("US").
  motor.setSequence(MIXED);   // Secuencia de medio-paso. Puede usarse secuencia simple ("SIMPLE") y sencuencia en onda ("WAVE").
}

void loop()
{
  motor.setRotation(LEFT);    // Configura rotación a izquierdas
  motor.run(1000);            // Rota 500 pasos
  motor.setRotation(RIGHT);   // Configura rotación a derechas
  motor.run(1000);            // Rota 500 pasos
}
