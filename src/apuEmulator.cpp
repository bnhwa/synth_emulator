#include "apuEmulator.h"

    APU::APU(byte pin_use){
      mic_pin = pin_use;
      dcval = 0;
      osc_length = 0;
      sigmaDeltaSetup(0, 88200);
      sigmaDeltaAttachPin(mic_pin, 0);
      sigmaDeltaWrite(0, 0);
      initPitch();
      initWaves();
    }
    void APU::initPitch(){
      //scale pitches to appropriate level
      //pitches are note_freq*1000*(1/audio_period);
      for (byte i = 0; i<cnt_notes(narr); i++){
//        Serial.println(audio_period);
        narr[i]=  narr[i]*(float(note_offset)/float(audio_period));
      }
    }
	void APU::initWaves(){
	  //sine wave
	  float k=(2*PI)/256;
	  float RadAngle;                           // Angle in Radians
	  // calculate sine values
	  for(uint16_t theta=0;theta<256;theta++) {
	    RadAngle=theta*k;               // 8 bit angle converted to radians
	    SineValues[theta]=(sin(theta*k)*127)+128; 
	  }
	  //sawtooth
	  for(uint16_t i=0;i<128;i++){
	    SawValues[i]=i*2;
		}
	  int j=0;
	  for(uint16_t i=126; i>0;i--,j++){
	  	SawValues[127+j]=i*2;
	  }
	}
    float APU::getNote(byte n){
      return narr[n];
    }
    float APU::get_freq(byte l_nt,byte oct) {//get note frequency
      return narr[l_nt] * pow(2, oct);
    }
    void APU::append(oscillator item) {
        if (osc_length < max_oscs){
          data[osc_length++] = item;
        }
   
    }
    void APU::remove(byte index) {
        if (index >= osc_length) return;
        memmove(&data[index], &data[index+1], (osc_length - index - 1)*osc_size);
        osc_length--;
    }

    void APU::iterateAll(){
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
    void APU::playAudio(byte index){
      oscillator* idx = &data[index];
      uint16_t wlen = wavedata[ idx->waveform].size;
      uint16_t* wpos = wavedata[ idx->waveform].wavedat;
      idx->pos+=idx->pitch;
      if (idx->pos >= wlen) idx->pos = 0;
        sigmaDeltaWrite(0, wpos[int(idx->pos)]);
    }
