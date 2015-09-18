// Import required libraries
#include <Spikee.h>

// Create neuron
Neuron output_neuron;

// Time control
float t;
float dt = 1;

void setup(void)
{  
  Serial.begin(115200);
  pinMode(6,OUTPUT);
  
  // Start counting
  t = 0;  
}

void loop() {
  
  // Get stimulus
  int sensor_input = analogRead(A0);
  //Serial.println(sensor_input);
  float stimulus = map(sensor_input, 0, 1023, 0, 1000);
  output_neuron.setStimulus(stimulus);
  //Serial.println(stimulus);
  
  // Update neuron state
  output_neuron.update(t,dt); 
  //Serial.println(output_neuron.getMembrane());
  
  // Map to output
  int led_state = map(output_neuron.getMembrane(), -65, 0, 0, 255);
  Serial.println(led_state);
  analogWrite(6,led_state);
  
  delay(dt);
 
}
