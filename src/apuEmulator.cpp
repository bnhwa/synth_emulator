#include "apuEmulator.h"
	//OSCList
	OscList::OscList(uint16_t limit){
		this->limit = limit;
	}

    void OscList::add_note(oscillator item) {
      // scale beats to that of program cycle, default 4/4 time
      // item.pos = item.pos/float(bpm);
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
      for (byte i = 0; i<cnt_notes(narr); i++){
        // narr[i]=  narr[i]*(float(note_offset)/float(audio_period));
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
    void APU::add_note_queue( uint16_t pitch, uint8_t octave, float note_len,float pos,uint16_t waveform){

      uint16_t tmp_ctr = uint16_t((float(note_len)*float(cycles_per_measure)/float(bpm)*speed));//
      uint16_t tmp_max_cnt = tmp_ctr;
      tmp_ctr+=tmp_max_cnt*adsr.getRelease();
      // tmp.pos = tmp.pos/float(bpm);
    	oscillator tmp = {
    		waveform,
    		get_freq(pitch,octave),
    		tmp_ctr,
    		pos/float(bpm),
    		tmp_max_cnt
    	};
      // tmp.ctr = uint16_t((float(tmp.ctr)*float(cycles_per_measure)/float(bpm)*speed));//
      // tmp.max_cnt = tmp.ctr;
      // tmp.ctr+=tmp.max_cnt*adsr.getRelease();
      // tmp.pos = tmp.pos/float(bpm);
    	sn_data.add_note(tmp);
    }
    void APU::append(oscillator item) {
      //depreciated as this rounds down floats, use normaladd_note_queue
      	//append oscillaror directly to be played,
        // item.ctr = uint16_t((float(item.ctr)*float(cycles_per_measure)/float(bpm)*speed));//
        // item.max_cnt = item.ctr;
        // item.pos = 0;
        // //if use adsr, add
        // if (item.pos!=0){
        //  item.pos = (item.pos)*cycles_per_measure/float(bpm)*speed;//
        //  // Serial.println(item.pos);
        // }
      	this->active_oscs.add_note(item);
   
    }
    void APU::remove(byte index) {
    	//remove active oscillator directly
      // if (index <=(this->active_oscs.osc_length   ))
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
          if (measure_subcount>=cycles_per_measure){
          	//increment measure            //add to stack
            measure_count+=1;
            measure_subcount=0;
          }

          next_cycle += cycle_period;

          bool iterate = ((cpu_cycles % 200)==0);
          if (iterate){
            //presorted note queue
            measure_subcount += 1;
            float m_pos = float(measure_count+(float(measure_subcount)/float(cycles_per_measure)))*speed;
            while (sn_data.osc_length>0){
              //assume song notes are fed in order
              oscillator* idx = &sn_data.data[0];
              if ((idx->pos)> m_pos){
                //break if next note is further along the measure
                break;
              }else{

                //mult pos for both, and append to stack
                // idx->ctr = uint16_t((float(idx->ctr)*float(cycles_per_measure)/float(bpm)*speed));//
                // idx->max_cnt = idx->ctr;
                // idx->ctr+=idx->max_cnt*adsr.getRelease();
                //if use adsr, add
                active_oscs.append(sn_data.data[0]);
                sn_data.remove(0);
              }
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
      // Serial.println(measure_count);

    }
    void APU::playAudio(byte index){
      oscillator* idx = &active_oscs.data[index];
      uint16_t wlen = wavedata[ idx->waveform].size;
      uint16_t* wpos = wavedata[ idx->waveform].wavedat;
      idx->pos+=idx->pitch;
      //play pitch, which is basically how fast we iterate through a wave
      //get location on waveform
      if (idx->pos >= wlen) idx->pos = 0;
      if (use_adsr){
        sigmaDeltaWrite(0, adsr.getADSR(idx->ctr,idx->max_cnt)*wpos[int(idx->pos)]);
      }else{
        sigmaDeltaWrite(0, wpos[int(idx->pos)]);
      }
    }
   

    //modify/get APU params
    void APU::setBPM(byte bpm){
      if (bpm)
      this->bpm = bpm;
    }
    void APU::setSpeed(float speed){
    	//set zero = reset
      if (speed>0){
      	this->speed = 1/speed;
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
    ADSR::ADSR(){
      sustain_level = 1-sustain_level;
      recalibrate();
    }
    void ADSR::recalibrate(){
      if (attack+decay+sustain<1){
        //add to attack,  sustain, release if things are all zeroed, 
        float recalibrate = (1-(attack+decay+sustain))/3;
        attack+=recalibrate;
        sustain+=recalibrate;
        release+=recalibrate;
      }
      dSlope=((maxAttack-sustain_level)/decay);
      dsr = attack+sustain+release;
      sr = sustain+release;
      
      //recount adsr_total
      if (decay==0){
        dsr = sr;
        aSlope = (sustain_level/(attack));
      }else{
        aSlope = (maxAttack/attack);
      }
      if (sustain==0){
        dsr-=attack;
        sr-=sustain;
        aSlope = (sustain_level/attack);
      }else{
      }
      adsr_tot = attack+decay+sustain+release;
    }
    void ADSR::setAttack(float a, float m){
      attack = a;
      maxAttack = m;
      recalibrate();
    }
    void ADSR::setDecay(float d){
      decay = d;
      recalibrate();
    }
    void ADSR::setSustain(float s, float s_lev){
      sustain = s;
      setSustainLevel(s_lev);
      recalibrate();
      
    }
    void ADSR::setSustainLevel(float s_lev){
      if (s_lev<=1.0) sustain_level = 1-s_lev;
    }
    void ADSR::setRelease(float r){
      release = r;
      recalibrate();
    }
     
    
    float ADSR::getADSR(uint16_t cnt, uint16_t max_cnt){
       float currpos = float(cnt)/float(max_cnt);//
       float diff = adsr_tot-attack;
       if ((attack>0) && (currpos>= diff)){
         return (attack-(currpos-dsr))*aSlope;
       }else if ((decay>0) && (currpos>=(diff-=decay))){
         //decay assume it highaf
         return ((currpos-sr))*dSlope+sustain_level;
         //slope ((1-sustain_level)/decay)
       }else if ((sustain>0) && (currpos>=(diff-=sustain))){
         //sustain
         return (sustain_level);//flat
       }else{
         return (currpos)*(sustain_level/release);
       }
    }
    float ADSR::getRelease(){
      return release;
     }
