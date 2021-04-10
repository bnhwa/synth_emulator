#include <notes.h>

//=========================================
//Polytonic Audio Processing Unit
//=========================================
struct oscillator{
  //cuz potential variable lengths of waveforms
  byte waveform;
  float pitch;
  float ctr;
  float pos;
};

//oscillators up to 8 notes played at once; used in junction with garbage collector
#define osc_size sizeof(oscillator)
#define max_oscs 8 //max number of oscillators
class APU {
public:
    //mic
    byte mic_pin;
    //vars
    bool playing = true;
    const uint32_t APU_FREQ =  1789773 / 2 / 2; // APU is half speed of NES CPU, and we are running half the resolution of that to stay light.
    const uint32_t cycle_period = F_CPU / APU_FREQ;
    const uint16_t audio_rate = 44100;
    const uint32_t audio_period = F_CPU / audio_rate;//5442
    const uint32_t note_offset = 1000;//1000 for esp32
    uint32_t next_audio = 0;
    uint32_t next_cycle = 0;
    uint32_t cpu_cycles = 0;
    uint32_t apu_cycles = 0;//
    uint32_t t_last = 0;
    uint32_t cycles_delta = 0;
    uint32_t cycles_so_far = 0;
    const uint8_t audio_divisor = 2;
    uint8_t audio_counter = 0;
    //tables
    //note freq array for lowest octave; gets converted to that based on APU emulator audio period
    float narr[13] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,0 };
    float dcval = 0;//value to output to pin, combined/scaled vals of oscillators
    byte osc_length = 0;
    oscillator data[max_oscs];
    APU(byte pin_use){
      mic_pin = pin_use;
      dcval = 0;
      osc_length = 0;
      sigmaDeltaSetup(0, 88200);
      sigmaDeltaAttachPin(mic_pin, 0);
      sigmaDeltaWrite(0, 0);
      initPitch();
    }
    void initPitch(){
      //scale pitches to appropriate level
      //pitches are note_freq*1000*(1/audio_period);
      for (byte i = 0; i<cnt_notes(narr); i++){
//        Serial.println(audio_period);
        narr[i]=  narr[i]*(float(note_offset)/float(audio_period));
      }
    }
    //
    float getNote(byte n){
      return narr[n];
    }
    float get_freq(byte l_nt,byte oct) {//get note frequency
      return narr[l_nt] * pow(2, oct);
    }
    void append(oscillator item) {
        if (osc_length < max_oscs){
          data[osc_length++] = item;
        }
   
    }
    void remove(byte index) {
        if (index >= osc_length) return;
        memmove(&data[index], &data[index+1], (osc_length - index - 1)*osc_size);
        osc_length--;
    }

    void iterateAll(){
      next_cycle = ESP.getCycleCount();
      next_audio = ESP.getCycleCount();

      while (osc_length>0){
        uint32_t t_now = ESP.getCycleCount();
        if (t_last > t_now) {
          next_cycle = t_now;
          next_audio = t_now;
        }
    
        if (t_now >= next_cycle) {
          //clock audio processing unit
          next_cycle += cycle_period;
          for (byte i =0; i<osc_length;i++){
            //update counter for current oscillators
            oscillator* idx = &data[i];
            idx->ctr-=0.001;//quarter note = 200
            if(idx->ctr <=0){
                remove(i);
              return;
            }
          }
          apu_cycles += 2;
        }
    
        if (t_now >= next_audio) {
          //iterate notes
          next_audio += audio_period;
    
          audio_counter++;
          if (audio_counter >= audio_divisor) {
            //play audio
            audio_counter = 0;
            for (byte i =0; i<osc_length;i++){
              playAudio(i);
            }
          }
        }
        t_last = t_now;
      }

    }
    void playAudio(byte index){
      oscillator* idx = &data[index];
      uint16_t wlen = wavedata[ idx->waveform].size;
      uint16_t* wpos = wavedata[ idx->waveform].wavedat;
      // waves wdat = wavedata[(*idx).waveform];
      //generate sounds based on https://wiki.nesdev.com/w/index.php/APU_Noise
//      float p =3.1826534362366776;//3.0045938993;//16.351*9*(1/audio_rate);
      idx->pos+=idx->pitch;
      if (idx->pos >= wlen) idx->pos = 0;
        sigmaDeltaWrite(0, wpos[int(idx->pos)]);
    }

};
//List l {.dcval=0,.length =0,.data={}};
APU apu= APU(25);

void initWaves(){
  //sine wave
  float k=(2*PI)/256;
  float RadAngle;                           // Angle in Radians
  // calculate sine values
  for(uint16_t theta=0;theta<256;theta++) {
    RadAngle=theta*k;               // 8 bit angle converted to radians
    SineValues[theta]=(sin(theta*k)*127)+128; 
  }
  //sawtooth
  for(uint16_t i=0;i<128;i++)
    SawValues[i]=i*2;
  int j=0;
  for(uint16_t i=126; i>0;i--,j++)
    SawValues[127+j]=i*2;
}

void setup(){
  //500 frames
//  sigmaDeltaSetup(0, 88200);
//  sigmaDeltaAttachPin(25, 0);
//  sigmaDeltaWrite(0, 0);
//l= APU(25);
  oscillator test = {
    .waveform = 1,
    .pitch = apu.get_freq(E,1),
    .ctr=2000,
    .pos = 0
  }; 
  oscillator eli = {
    .waveform = 1,
    .pitch = apu.get_freq(E,1),
    .ctr=10000,
    .pos = 0
  }; 
  oscillator gre = {
    .waveform = 1,
    .pitch = apu.get_freq(G,1),
    .ctr=5000,//1 beat
    .pos = 0
  }; 
  Serial.begin(9600);
//  Serial.print(0);
  apu.append(test);
//  apu.append(eli);
//  apu.append(gre);
//  Serial.println(l.data[0].waveform);
//  apu.remove(0);
//  Serial.println(l.data[0].waveform);
  
  initWaves();
  apu.iterateAll();
}

void loop(){
  //1 is base
//  for(float i=0;i<256;i+=1){//double is pitch//16.35
//    dacWrite(25,SineValues[int(i)]);//gain is 15
////   dacWrite(25,SawValues[int(i)]);
////    Serial.println(SineValues[i]);
//  }
//  delay(1);

//  l.iterateAll();
  Serial.println(apu.narr[0]);
  delay(1000);
}
