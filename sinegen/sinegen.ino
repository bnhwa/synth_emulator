//https://www.xtronical.com/basics/audio/dacs-on-esp32/

//=========================================
// APU stuff
//=========================================
long fps = 7680;//30 iterations of the 256 mat
byte oscpins[] = {25,26,27,28,29,30, 12,13};
//=========================================
// wave stuff
//=========================================
#define cnt_notes(x) (sizeof(x) / sizeof(x[0]))
//sinewave
#define SINE 0
uint16_t SineValues[256];
//sawtooth
#define SAW 1
uint16_t SawValues[256];
//bc cant have fully staggered arrays; use struct workaround
struct waves{//data about each?
  uint16_t *wavedat;
  uint16_t size;
};
waves wavedata[]{
  {SineValues,cnt_notes(SineValues)},
  {SawValues,cnt_notes(SineValues)}
};
//=========================================
//Notes
//=========================================

#define C 0
#define CS 1
#define D 2
#define DS 3
#define E 4
#define F 5
#define  FS 6
#define  G 7
#define  GS 8
#define  A 9
#define  AS 10
#define  B 11
#define R 12 //rest
byte C_N[][2] = {{C,0}};
byte CS_N[][2] = {{CS,0}};
byte D_N[][2] = {{D,0}};
byte DS_N[][2] = {{DS,0}};
byte E_N[][2] = {{E,0}};
byte F_N[][2] = {{F,0}};
byte  FS_N[][2]= {{FS,0}};
byte  G_N[][2] = {{G,0}};
byte  GS_N[][2] = {{GS,0}};
byte  A_N[][2] = {{A,0}};
byte  AS_N[][2] = {{AS,0}};
byte  B_N[][2] = {{B,0}};
byte R_N[][2]  = {{0,0}}; //rest
byte C_T_[][2] ={{C,0}, {E,0}, {G,0}, {C,1}};//CEG C+ {freq,offset}, freq offse
byte CS_T_[][2]  = {{CS,0}, {F,0}, {GS,0}, {CS,1}};
byte D_T_[][2]  = {{D,0}, {FS,0}, {A,0}, {D,1}};

byte DS_T_[][2]  = {{DS,0}, {F,0}, {AS,0}, {DS,1}};
byte E_T_[][2]  = {{E,0}, {GS,0}, {B,0}, {E,1}};
byte F_T_[][2]  = {{F,0}, {A,0}, {C,1}, {F,1}};
byte FS_T_[][2]  = {{FS,0}, {C,0}, {DS,1}, {FS,1}};
byte G_T_[][2]  = {{G,0}, {B,0}, {D,1}, {G,1}};
byte GS_T_[][2]  = {{GS,0}, {C,1}, {DS,1}, {GS,1}};
byte A_T_[][2]  = {{A,0}, {CS,0}, {E,1}, {A,1}};
byte AS_T_[][2]  = {{AS,0}, {D,0}, {F,1}, {AS,1}};
byte B_T_[][2]  = {{B,0}, {DS,0}, {FS,1}, {B,1}};


byte A_DISS_[][2]  = {{A,0}, {CS,0}, {E,1}, {GS,1}};
byte CS_FOUR_[][2]  = {{CS,0}, {E,0}, {A,0}, {CS,1}};
#define C_T 13 
#define CS_T 14
#define D_T 15

#define DS_T 16
#define E_T 17
#define F_T 18
#define FS_T 19
#define G_T 20
#define GS_T 21
#define A_T 22
#define AS_T 23
#define B_T 24

#define A_DISS 25
#define CS_FOUR 26
//get num notes per each,
#define cnt_notes(x) (sizeof(x) / sizeof(x[0]))
struct NOTES
{
  byte (*notes)[2];
  uint16_t note_count;
};
//Note struct, basically pseudo hashtable
#define C 0
#define CS 1
#define D 2
#define DS 3
#define E 4
#define F 5
#define  FS 6
#define  G 7
#define  GS 8
#define  A 9
#define  AS 10
#define  B 11
#define R 12 //rest
NOTES noteTable[] = {
  { C_N , cnt_notes(C_N)},
  { CS_N , cnt_notes(CS_N)},
  { D_N , cnt_notes(D_N)},
  { DS_N , cnt_notes(DS_N)},
  { E_N , cnt_notes(E_N)},
  { F_N , cnt_notes(F_N)},
  {  FS_N, cnt_notes(FS_N)},
  {  G_N , cnt_notes(G_N)},
  {  GS_N , cnt_notes(GS_N)},
  {  A_N , cnt_notes(A_N)},
  {  AS_N , cnt_notes(AS_N)},
  {  B_N , cnt_notes(B_N)},
  { R_N  , cnt_notes(C_N)},
  { C_T_, cnt_notes(C_T_)},
  { CS_T_, cnt_notes(CS_T_)},
  { D_T_, cnt_notes(D_T_)},
  { DS_T_, cnt_notes(DS_T_)},
  { E_T_, cnt_notes(E_T_)},
  { F_T_, cnt_notes(F_T_)},
  { FS_T_, cnt_notes(FS_T_)},
  { G_T_, cnt_notes(G_T_)},
  { GS_T_, cnt_notes(GS_T_)},
  { A_T_, cnt_notes(A_T_)},
  { AS_T_, cnt_notes(AS_T_)},
  { B_T_, cnt_notes(B_T_)},
  { A_DISS_, cnt_notes(A_DISS_)},
  { CS_FOUR_, cnt_notes(CS_FOUR_)}

};
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
    
    oscillator data[8];//8 oscillators at once
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
        if (osc_length < 8){
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
