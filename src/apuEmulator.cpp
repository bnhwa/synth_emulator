#include "apuEmulator.h"
	//OSCList
	OscList::OscList(uint16_t limit){
		this->limit = limit;
	}

    void OscList::add_note(oscillator item, uint32_t cycles_per_measure, float speed, byte bpm) {
      // scale 4/beats to that of program cycle
      // 1 bpm is approximately 1 sec
      //length in cycles
      item.ctr = uint16_t((float(item.ctr)*float(cycles_per_measure)/float(bpm)*speed));//
      if (item.pos!=0){
      	item.pos = (item.pos)*cycles_per_measure/float(bpm)*speed;//
      	Serial.println(item.pos);
      }
      append(item);
    }
	void OscList::append(oscillator item){
	  if (osc_length < limit){
        data[osc_length++] = item;
      }
      // Serial.println("length");
      // Serial.println(osc_length);
	}
    void OscList::remove(byte index) {
        if (index >= limit) return;
        memmove(&data[index], &data[index+1], (osc_length - index - 1)*osc_size);
        osc_length--;
    }
	//APU
    APU::APU(byte pin_use){
      mic_pin = pin_use;
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
    void APU::add_oscillator_queue(uint16_t waveform, uint16_t pitch, uint8_t octave, float note_len,float pos){
    	oscillator tmp = {
    		waveform,
    		get_freq(pitch,octave),
    		note_len,
    		pos,
    		note_len
    	};
    	sn_data.add_note(tmp,cycles_per_measure,(this->speed),bpm);
    }
    // void APU::add_oscillator_queue(oscillator item){
    // 	//add to measure queue
    // 	sn_data.add_note(item,cycles_per_measure,(this->speed),bpm);
    // }
    void APU::append(oscillator item) {
      	//append oscillaror directly to be played, raw
      	// scale 4/beats to that of program cycle
      	// 1 bpm is approximately 1 sec
      	//length in cycles
      	this->active_oscs.add_note(item,cycles_per_measure,(this->speed),bpm);
   
    }
    void APU::remove(byte index) {
    	//remove active oscillator directly 
        this->active_oscs.remove(index);
    }

    void APU::iterateAll(){
      next_cycle = ESP.getCycleCount();
      next_audio = ESP.getCycleCount();

      while ((active_oscs.osc_length+sn_data.osc_length)>0){
        uint32_t t_now = ESP.getCycleCount();
        if (t_last > t_now) {
          next_cycle = t_now;
          next_audio = t_now;
        }
    
        if (t_now >= next_cycle) {
          //clock audio processing unit
          if (cpu_cycles>=cycles_per_measure){
          	//increment measure
            measure_count+=1;
            cpu_cycles = 0;
          }
          next_cycle += cycle_period;
          //queued
          bool iterate = ((cpu_cycles % 200)==0);
          for (byte i =0; i< sn_data.osc_length;i++){
            //update counter for enqueued oscillators
			oscillator* idx = &sn_data.data[i];
			if (iterate) idx->pos-=1;
            if(idx->pos <=0){
            	idx->pos=0;
            	active_oscs.append(sn_data.data[i]);
                sn_data.remove(i);
                
            }
          }
          //active
          for (byte i =0; i< active_oscs.osc_length;i++){
            //update counter for current oscillators
			oscillator* idx = &active_oscs.data[i];
			if (iterate) idx->ctr-=1;
            if(idx->ctr <=0){
                active_oscs.remove(i);

            }
          }

          cpu_cycles += 1;
        }
    
        if (t_now >= next_audio) {
          //iterate notes
          next_audio += audio_period;
    
          audio_counter++;
          if (audio_counter >= audio_divisor) {
            //play audio
            audio_counter = 0;
            for (byte i =0; i<active_oscs.osc_length;i++){
              playAudio(i);
            }
          }
        }
        t_last = t_now;
      }

    }
    void APU::playAudio(byte index){
      oscillator* idx = &active_oscs.data[index];
      uint16_t wlen = wavedata[ idx->waveform].size;
      uint16_t* wpos = wavedata[ idx->waveform].wavedat;
      idx->pos+=idx->pitch;
      //play pitch, which is basically how fast we iterate through a wave
      //get location on waveform
      if (idx->pos >= wlen) idx->pos = 0;
        sigmaDeltaWrite(0, wpos[int(idx->pos)]);
    }
   

    //modify/get APU params
    void APU::setBPM(byte bpm){
      if (bpm)
      this->bpm = bpm;
    }
    void APU::setSpeed(float speed){
    	//set zero = reset
      if (speed>0){
      	this->speed = speed;
      	cycles_per_measure = def_cycles_per_measure*speed;
  	  }else{
  	  	this->speed = 1;
  	  	cycles_per_measure = def_cycles_per_measure;
  	  }
    }
    uint16_t APU::num_active_oscillators() {//get note frequency
      return this->active_oscs.osc_length;
    }

    //ADSR
    ADSR::ADSR(){}
    float getADSR(float pos, float maxpos){
    	float currpos = pos/maxpos;
    	return 0.0;
    }
