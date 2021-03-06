#ifndef _APUEMULATOR_h
#define _APUEMULATOR_h

#include "arduino.h"
#define max_oscs 20 //max number of oscillators
#define max_notes_queue 512 //max notes in queue
//TODO, for mem purposes encode/decode note input as bytes
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
//Song Structs
//=========================================


//=========================================
//Oscillator
//=========================================
struct oscillator{
  //cuz potential variable lengths of waveforms
  uint16_t waveform;
  float pitch;
  uint16_t ctr;// if memory isnt important, change it to float ctr; uint16_t makes max note length 16 measures
  float pos;//if memory isnt important, change it to float pos;
  uint16_t max_cnt;//original note length; we store this because its needed for ADSR,
  //when inactive, pos is used to count down till oscillator is played and appended on active stack
  //when active, used to track waveform position 
};
#define osc_size sizeof(oscillator)
//=========================================
//ADSR Envelope waveform
//=========================================
class ADSR{
    public:
        ADSR();
        float getADSR(uint16_t cnt, uint16_t max_cnt);

        void setAttack(float a, float m);
        void setDecay(float d);
        void setSustain(float s, float s_lev);
        void setSustainLevel(float s_lev);
        void setRelease(float r);
        float getRelease();
    private:
        //depending on memory considerations, use uints and float casting,
        //but sound is not as smooth
        float attack = 0.08;
        float maxAttack = 1.0;
        float decay = 0.08;
        float sustain = 0.84;
        float sustain_level = 0.1;
        float release = 0.1;
        float adsr_tot = attack+decay+sustain+release;
        float dsr = 0;
        float sr = 0;
        
        float aSlope=0;
        float dSlope = 0;
        void recalibrate();

};

//=========================================
//Polytonic Audio Processing Unit
//=========================================
//Measure "stack"
//String "AS5_pos_length"
class OscList{
  public:
    OscList(uint16_t limit);
    uint16_t osc_length = 0;
    uint16_t limit;
    oscillator data[max_notes_queue];
    void add_note(oscillator item );
    void append(oscillator item);
    void remove(byte index);

};
//APU
class APU {
	public:
	  //mic
    uint16_t mic_pin;
	  //vars
    bool playing = true;
    bool use_adsr = true;
    APU(byte pin_use);
    float getNote(byte n);
    float get_freq(byte l_nt,byte oct);
    void append(oscillator item);//direct append to play
    void remove(byte index);//remove directly
    void iterateAll();
	void playAudio(byte index);
    //change/get APU Params
    void setBPM(byte bpm);
    void setSpeed(float speed);
    void add_note_queue(uint16_t pitch, uint8_t octave, float note_len,float pos,uint16_t waveform);
    uint16_t num_active_oscillators(); 
	private:
    //APU Vars
    const uint32_t APU_FREQ =  1789773 / 2 / 2;
    const uint32_t cycle_period = F_CPU / APU_FREQ;
    const uint16_t audio_rate = 44100;
    const uint32_t audio_period = F_CPU / audio_rate;//5442
    const uint32_t note_offset = 64;//1000 for esp32
    const uint32_t def_cycles_per_measure = 4000;
    uint32_t next_audio = 0;
    uint32_t next_cycle = 0;
    uint32_t cpu_cycles = 0;//4000 cycles by default = 1 measure
    uint32_t cycles_per_measure = def_cycles_per_measure;
    uint32_t measure_iter_cnt = 0;//iterations per measure
    uint32_t measure_count = 0;//measure by measure, 
    uint32_t measure_subcount = 0;//number of measure subcycles
    uint32_t t_last = 0;
    const uint8_t audio_divisor = 2;
    uint8_t audio_counter = 0;
    //note freq array for lowest octave; gets converted to that based on APU emulator audio period
    float narr[13] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,0 };
    OscList active_oscs = OscList(max_oscs);
    OscList sn_data = OscList(max_notes_queue);
    byte bpm = 4;
    float speed=1;
    uint16_t SineValues[256];//set beats per measure, default 4/4
    uint16_t SawValues[256];
		void initPitch();
		void initWaves();
    //change this to list later
    waves wavedata[2]={
      {SineValues,cnt_notes(SineValues)},
      {SawValues,cnt_notes(SawValues)}
    };
    //ADSR envelope generator
    ADSR adsr = ADSR();
    


};

#endif