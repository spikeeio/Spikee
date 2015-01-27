#include "Spikee.h"
#include "math.h"
#include "arduino.h"

Neuron::Neuron() {
	Vt = -45.0;
	Vreset = -65.0;
	EL = -65.0;
	V = EL*1e-3;
	gL = 20.0;
	C = 2000.0; 

	dT = 2.0;
	Vexp = -40.0;
  exp_act = 0.0;

	gsynx = 0.0;
	tau_syn_x = 5.0;
	e_syn_x = 0.0;

	spike_in = false;
	spike_out = false;

  print_membrane = false;
  neuron_output_pin = 0;
  spike_calculation_index = 0;
}

Neuron::~Neuron() {

}

void Neuron::setMembraneOutput(int the_output_pin) {

  pinMode(the_output_pin, OUTPUT);
  neuron_output_pin = the_output_pin;   
  print_membrane = true;

}

float Neuron::getSpikeFrequency() {

  return spike_frequency;

}

bool Neuron::getMembraneOutput() {

  return print_membrane;
}

int Neuron::getOutputPin() {

  return neuron_output_pin;
}

void Neuron::update(float t, float dt) {

  if (spike_in) {
  	//Serial.println("Spike received !");
  	spike_in = false;
    gsynx = gsynx + 100;
  }

  // Update gsynx
  gsynx = gsynx - gsynx * dt / (tau_syn_x);

  // Calculate new membrane potential
  if (V < Vt*1e-3) {
    V = V + dt*1e-3*( -gsynx*1e-9*(V-e_syn_x*1e-3)/(C*1e-12) -gL*1e-9*(V-EL*1e-3)/(C*1e-12) + exp_act*gL*1e-9*dT*1e-3*exp((V-Vexp*1e-3)/(dT*1e-3))/(C*1e-12) + I*1e-12/(C*1e-12));
  }
  else {
    V = Vreset*1e-3;
    spike_out = true;

    if (t - last_spike != 0){
      spike_frequency = 1/(t - last_spike)*1000;
    }
    else {spike_frequency = 0;}
    

    last_spike = t;

  } 

  // Update output
  if (print_membrane) {
    int led_state = map(V*1000, EL, Vt, 0, 255);
    if (V*1000 <= (EL + 0.15)) {led_state = 0;}
    analogWrite(neuron_output_pin, led_state);
  }

}

float Neuron::getgsynx(){
  return gsynx;
}

bool Neuron::getSpikeOut(){
  return spike_out;
}

void Neuron::resetSpikeOut(){
  spike_out = false;
}

void Neuron::setSpikeIn(){
  spike_in = true;
}

float Neuron::getMembrane() {
	return V*1000;
}

void Neuron::setStimulus(float stimulus){
  I = stimulus;
}

Network::Network() {

	neuron_index = 0;

  t = 0.;
  dt = 1.;

}

Network::~Network() {

}

void Network::createNeuron() {

  Neuron new_neuron;
  neurons[neuron_index] = new_neuron;
  neuron_index++;
	
}

void Network::update() {

  // Go through all neurons
  float start_time = micros();
  for (int i = 0; i < neuron_index; i++) {
  	neurons[i].update(t,dt);

  	if (neurons[i].getSpikeOut()) {

  	  // Reset status
      //Serial.print("Spike emited by neuron ");
      //Serial.println(i);
  	  neurons[i].resetSpikeOut();

  	  // Send spike to targets
  	  for (int j = 0; j < MAX_OUTPUTS; j++){
  	  	if (connections[i][j]) {
  	  	  //Serial.print("Spike received by neuron ");
  	      //Serial.println(connections[i][j]);

  	      neurons[connections[i][j]].setSpikeIn();	
  	  	}
  	    
  	  }	
  	}
  }

  t = t + dt;
  // Slow down ?
  //float sim_time = micros() - start_time;
  //if (sim_time < dt*1000) {
    //delayMicroseconds(dt - sim_time);
  //}
	
}

float Network::getgsynx(int neuron_number){
  return neurons[neuron_number].getgsynx();
}

float Network::getMembrane(int neuron_number) {

 	return neurons[neuron_number].getMembrane();
 	
}

void Network::setStimulus(int neuron_number, float stimulus) {

 	neurons[neuron_number].setStimulus(stimulus);
 	
}

float Network::getSpikeFrequency(int neuron_number) {

  return neurons[neuron_number].getSpikeFrequency();
  
}

void Network::connect(int input_neuron, int output_neuron) {
  
  bool output_added = false;

  
  for (int i = 0; i < MAX_OUTPUTS; i++) {

    if (!connections[input_neuron][i] && !output_added){
      connections[input_neuron][i] = output_neuron;
      output_added = true;
    }
  }
}

void Network::setMembraneOutput(int neuron_number, int output_pin) {
  
  neurons[neuron_number].setMembraneOutput(output_pin);
}