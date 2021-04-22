#include <apuEmulator.h>
#include <notes.h>


APU apu= APU(25);



void setup(){

  Serial.begin(9600);
//  //note,octave,note_len,note_pos,waveform within measure

apu.add_note_queue(A,5,1.1,0.0,0);
apu.add_note_queue(GS,5,1.1,1.0,0);
apu.add_note_queue(CS,4,7.0,1.0,0);
apu.add_note_queue(A,3,7.0,1.0,0);
apu.add_note_queue(E,5,1.1,2.0,0);
apu.add_note_queue(CS,5,1.1,3.0,0);
apu.add_note_queue(B,4,1.1,4.0,0);
apu.add_note_queue(CS,5,3.0,5.0,0);
apu.add_note_queue(FS,5,1.1,8.0,0);
apu.add_note_queue(E,5,1.1,9.0,0);
apu.add_note_queue(C,4,7.0,9.0,0);
apu.add_note_queue(A,3,7.0,9.0,0);
apu.add_note_queue(D,5,1.1,10.0,0);
apu.add_note_queue(A,4,1.1,11.0,0);
apu.add_note_queue(G,4,1.1,12.0,0);
apu.add_note_queue(A,4,3.0,13.0,0);
apu.add_note_queue(F,5,1.1,16.0,0);
apu.add_note_queue(E,5,1.1,17.0,0);
apu.add_note_queue(D,4,4.0,17.0,0);
apu.add_note_queue(AS,3,7.0,17.0,0);
apu.add_note_queue(D,5,1.1,18.0,0);
apu.add_note_queue(AS,4,1.1,19.0,0);
apu.add_note_queue(C,5,1.1,20.0,0);
apu.add_note_queue(G,5,3.0,21.0,0);
apu.add_note_queue(AS,3,3.0,21.0,0);
apu.add_note_queue(G,3,3.0,21.0,0);
apu.add_note_queue(FS,5,0.5,24.0,0);
apu.add_note_queue(F,5,0.5,24.5,0);
apu.add_note_queue(E,5,7.0,25.0,0);
apu.add_note_queue(G,4,7.0,25.0,0);
apu.add_note_queue(E,4,7.0,25.0,0);
apu.add_note_queue(CS,4,7.0,25.0,0);
apu.add_note_queue(A,3,7.0,25.0,0);
apu.add_note_queue(E,5,1.0,40.0,0);
apu.add_note_queue(F,5,1.0,41.0,0);
apu.add_note_queue(E,5,1.0,42.0,0);
apu.add_note_queue(A,5,1.0,43.0,0);
apu.add_note_queue(E,5,1.0,44.0,0);
apu.add_note_queue(F,5,1.0,45.0,0);
apu.add_note_queue(E,5,2.0,46.0,0);
apu.add_note_queue(D,5,1.0,48.0,0);
apu.add_note_queue(E,5,1.0,49.0,0);
apu.add_note_queue(D,5,1.0,50.0,0);
apu.add_note_queue(G,5,1.0,51.0,0);
apu.add_note_queue(D,5,1.0,52.0,0);
apu.add_note_queue(E,5,1.0,53.0,0);
apu.add_note_queue(D,5,2.0,54.0,0);
apu.add_note_queue(B,4,1.0,56.0,0);
apu.add_note_queue(C,5,1.0,57.0,0);
apu.add_note_queue(B,4,1.0,58.0,0);
apu.add_note_queue(AS,4,1.0,59.0,0);
apu.add_note_queue(B,4,1.0,60.0,0);
apu.add_note_queue(E,5,1.0,61.0,0);
apu.add_note_queue(G,5,1.0,62.0,0);
apu.add_note_queue(B,5,3.0,63.0,0);
apu.add_note_queue(A,5,1.0,66.0,0);
apu.add_note_queue(G,5,1.0,66.5,0);
apu.add_note_queue(FS,5,1.0,67.0,0);
apu.add_note_queue(G,5,1.0,68.0,0);
apu.add_note_queue(FS,5,1.0,69.0,0);
apu.add_note_queue(DS,5,1.0,70.0,0);
apu.add_note_queue(E,5,1.0,71.0,0);


//  apu.setSpeed(8);
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
  Serial.println("end");
//  String ok = "3.5";
//  Serial.println(ok.toFloat());
  delay(1000);
}
