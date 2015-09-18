// Import required libraries
#include <Spikee.h>

// Create neuron
Neuron output_neuron;

// Time control
float t;
float dt = 1;

// Neuron pin
neuron_pin = 6;

void setup(void)
{  
  Serial.begin(115200);
  pinMode(6,OUTPUT);
  
  // Start counting
  t = 0;  
}

void loop() {
  
  // Apply constant stimulus
  output_neuron.setStimulus(1000);
  
  // Update neuron state
  output_neuron.update(t,dt); 
  //Serial.println(output_neuron.getMembrane());
  
  // Map to output
  int led_state = map(output_neuron.getMembrane(), -65, 0, 0, 255);
  Serial.println(led_state);
  analogWrite(6,led_state);
  
  delay(dt);
 
}
