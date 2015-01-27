/* 
  Spikee Library for Arduino
  See the README file for more details.
 
  Written in 2015 by Marco Schwartz under a GPL license. 

  Version 0.1

  Changelog:

  Version 0.1: First release of the library

*/

#define MAX_NEURONS 5
#define MAX_OUTPUTS 5
#define SPIKE_FREQ_INTERVAL 5

class Neuron {

public:
  Neuron();
  ~Neuron();
  void update(float t, float dt);
  float getMembrane();
  float getgsynx();
  void setStimulus(float stimulus);

  bool getSpikeOut();
  void resetSpikeOut();
  void setSpikeIn();
  void setMembraneOutput(int pin_number);
  bool getMembraneOutput();
  int getOutputPin();

  float getSpikeFrequency();

private:

    // Spikes
    bool spike_in;
    bool spike_out;

    // Spike frequency
    float spike_frequency;
    float last_spikes[SPIKE_FREQ_INTERVAL];
    int spike_calculation_index;
    float last_spike;

    // Outputs
    bool print_membrane;
    int neuron_output_pin;

    // ID
    int id;
	  
    // Neuron variables
	  float V;
	  
    // Neuron parameters
    float EL;
    float Vt;
    float Vreset;
    float C;
    float gL;

    float dT;
    float Vexp;
    float exp_act;

    // Stimulus
    float I;

    // Syn
    float gsynx;
    float tau_syn_x;
    float e_syn_x;

};

class Network {
  
public:
  Network();
  ~Network();
  void connect(int input_neuron, int output_neuron);
  void update();
  void createNeuron();
  float getMembrane(int neuron_number);
  float getgsynx(int neuron_number);
  void setStimulus(int neuron_number, float stimulus);
  void setMembraneOutput(int neuron_number, int output_pin);

  float getSpikeFrequency(int neuron_number);

private:
  int neuron_index;
  Neuron neurons[MAX_NEURONS];
  int connections[MAX_NEURONS][MAX_OUTPUTS];

  float t;
  float dt;
};