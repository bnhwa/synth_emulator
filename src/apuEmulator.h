#ifndef _APUEMULATOR_h
#define _APUEMULATOR_h

#include "arduino.h"
#define max_oscs 8 //max number of oscillators
//=========================================
// wave stuff
//=========================================
#define cnt_notes(x) (sizeof(x) / sizeof(x[0]))
//sinewave
#define SINE 0
//sawtooth
#define SAW 1

//bc cant have fully staggered arrays; use struct workaround
struct waves{//data about each?
  uint16_t *wavedat;
  uint16_t size;
};

//=========================================
//Oscillator
//=========================================
struct oscillator{
  //cuz potential variable lengths of waveforms
  uint16_t waveform;
  float pitch;
  float ctr;
  float pos;
};
#define osc_size sizeof(oscillator)

//=========================================
//Polytonic Audio Processing Unit
//=========================================

class APU {
	public:
	  //mic
    uint16_t mic_pin;
	  //vars
    bool playing = true;
    APU(byte pin_use);
    float getNote(byte n);
    float get_freq(byte l_nt,byte oct);
    void append(oscillator item);
    void remove(byte index);
    void iterateAll();
	  void playAudio(byte index);
    //change APU Params
    void setBPM(byte bpm);
    void setSpeed(float speed);
	private:
    //APU Vars
    const uint32_t APU_FREQ =  1789773 / 2 / 2; // APU is half speed of NES CPU
    const uint32_t cycle_period = F_CPU / APU_FREQ;
    const uint16_t audio_rate = 44100;
    const uint32_t audio_period = F_CPU / audio_rate;//5442
    const uint32_t note_offset = 1000;//1000 for esp32
    uint32_t next_audio = 0;
    uint32_t next_cycle = 0;
    uint32_t cpu_cycles = 0;//aka how many measures hae passed
    uint32_t apu_cycles = 0;
    uint32_t t_last = 0;
    const uint8_t audio_divisor = 2;
    uint8_t audio_counter = 0;
    //note freq array for lowest octave; gets converted to that based on APU emulator audio period
    float narr[13] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,0 };
    float dcval = 0;//value to output to pin, combined/scaled vals of oscillators
    uint16_t osc_length = 0;
    oscillator data[max_oscs];
    //synth/song params

    byte bpm = 4;
    float speed;
    uint16_t SineValues[256];;//set beats per measure, default 4/4
    uint16_t SawValues[256];
		void initPitch();
		void initWaves();
    //change this to list later
    waves wavedata[2]={
      {SineValues,cnt_notes(SineValues)},
      {SawValues,cnt_notes(SawValues)}
    };


};

#endif