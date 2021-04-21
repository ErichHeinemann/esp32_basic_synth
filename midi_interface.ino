/*
 * a simple implementation to use midi
 *
 * Author: Marcel Licence
 */


/*
 * look for midi interface using 1N136
 * to convert the MIDI din signal to
 * a uart compatible signal
 */
// #define RXD2 16 /* U2RRXD */
// #define TXD2 17

MIDI_CREATE_INSTANCE( HardwareSerial, Serial2, MIDI );

/* use define to dump midi data */
#define DEBUG_MIDI

// #define MIDITHROUGHOFF




/* constant to normalize midi value to 0.0 - 1.0f */
#define NORM127MUL	0.007874f

inline void Midi_NoteOn(uint8_t note ){
    Synth_NoteOn(note);
}

inline void Midi_NoteOff(uint8_t note ){
    Synth_NoteOff(note);
}

/*
 * this function will be called when a control change message has been received
 */

inline void Midi_ControlChange(uint8_t channel, uint8_t data1, uint8_t data2){
    if( data1 == 17 ){
        if( channel < 10 ){
            Synth_SetSlider(channel,  data2 * NORM127MUL);
        }
    }
    if(( data1 == 18) && (channel == 1 )){
        Synth_SetSlider(8,  data2 * NORM127MUL);
    }

    if( ( data1 == 16) && (channel < 9 ) ){
        Synth_SetRotary(channel, data2 * NORM127MUL);

    }
    if( ( data1 == 18) && (channel == 0) ){
        Synth_SetRotary(8,  data2 * NORM127MUL);
    }
}

/*
 * function will be called when a short message has been received over midi
 */
/*
inline void HandleShortMsg(uint8_t *data){
    uint8_t ch = data[0] & 0x0F;

    switch (data[0] & 0xF0){

    // note on
    case 0x90:
        if (data[2] > 0){
            Midi_NoteOn(data[1]);
        }else{
            Midi_NoteOff(data[1]);
        }
        break;
    // note off
    case 0x80:
        Midi_NoteOff(data[1]);
        break;
    case 0xb0:
        Midi_ControlChange(ch, data[1], data[2]);
        break;
    }
}
*/
// void Midi_Setup(){
//    Serial2.begin(31250, SERIAL_8N1, RXD2, TXD2);
//    pinMode(RXD2, INPUT_PULLUP);  /* 25: GPIO 16, u2_RXD */
// }


inline void Midi_Setup(){
    MIDI.begin(MIDI_CHANNEL_OMNI);
    // MIDI_Thru to off to spent some CPU-Time
#ifdef MIDITHROUGHOFF
    MIDI.turnThruOff();
#endif    
    // Define Callbacks
    MIDI.setHandleNoteOn( handleNoteOn );
    MIDI.setHandleNoteOff( handleNoteOff );     
    MIDI.setHandlePitchBend( handlePitchBend );      
    MIDI.setHandleControlChange( handleControlChange );  
    MIDI.setHandleProgramChange( handleProgramChange );
    MIDI.setHandleStart( handleStart );    
    MIDI.setHandleStop( handleStop );    
    MIDI.setHandleClock( handleClock );    
    MIDI.setHandleContinue( handleContinue );      

    Serial.println("Setup done");
}


void handleNoteOn(byte channel, byte note, byte velocity){
  
#ifdef DEBUG_MIDI
   Serial.println();
   Serial.printf("NoteOn %X %X %X\n", channel, note, velocity);
#endif
#ifdef FILTERCHANNEL
  if( channel == FILTERCHANNEL ){
#endif
  // Ideas for Pitch:
  // - calculate a Pitch based on the played octave
  // - calculate a pitch based on the played velocity
  // - use the lookup-table "pitches" to play a note chromatically
  
   
   /*
   // select this note for the currrent Editor
   // derive a new Pitch based on the played octave
   // get the Octave:
   new_pitch = 0.5;
   // uint8_t octave = floor( note / 12 );
   // Serial.printf("Octave %d %d \n", octave,  note - 45 ); 

   uint8_t mynote = 56; // Note, wo der Kammerton A3 drauf liegt
   if( note >= 44 && note <= 82 ){ 
     new_pitch = (float) pitches[ note - 45 ];
   }

   Sampler_SelectNote( mynote );    
   Sampler_SetSoundPitch( new_pitch );
   */
   // Serial.printf("new_pitch %f \n", new_pitch ); 
   // Sampler_NoteOn( note , velocity );

  if( velocity > 0){
      Midi_NoteOn(note -12);
  }else{
      Midi_NoteOff( note -12);
  }

#ifdef FILTERCHANNEL
  }
#endif
}

void handleNoteOff( byte channel, byte note, byte velocity){
#ifdef DEBUG_MIDI
  Serial.printf("NoteOff %X %X %X\n", channel, note, velocity);  
#endif

#ifdef FILTERCHANNEL
  if( channel == FILTERCHANNEL ){
#endif

  Midi_NoteOff( note-12 );

#ifdef FILTERCHANNEL
  }
#endif

}

void handleControlChange( byte channel, byte number, byte value){
  Serial.printf("CC %X %X %X\n", channel, number, value);
  // - with a CC, You could probably control the global Effects Bitcruscher, Global Pitch or the Filter

#ifdef FILTERCHANNEL
  if( channel == FILTERCHANNEL ){
#endif
  // - Sampler_SetPlaybackSpeed( NORM127MUL * value ); // value from 0 to 2
  // Modwheel CC==1  
  if( number ==1 ){
    // Effect_SetBiReso( NORM127MUL * value );
  } 
  // - Effect_SetBitCrusher( float value )
  // - Effect_SetBiCutoff(float value )

  // Or You could control the values for a specific Sampleplayer/Note ( select the note via Sampler_SelectNote( uint8_t note ) )
  // - Sampler_SetSoundPitch(float value) // for a selectedNote
  // - Sampler_SetDecay( uint8_t ch, uint8_t data1, uint8_t data2)  // data2 should be the Value from thee CC, channel and data1 could be ignored

#ifdef FILTERCHANNEL
  }
#endif
  
}

void handleProgramChange(byte channel, byte number){
  Serial.printf("ProgramChange %X %X \n", channel, number);  
#ifdef FILTERCHANNEL
  if( channel == FILTERCHANNEL ){
#endif
  
  // fill the I2S-Cache because the loading of other samples takes some time!
  for( int i = 0; i <= 512; i++){
    if( !i2s_write_samples( 0.0f, 0.0f ) ){
       // error!
    }
  }
  
  // Change progNumber
  // Here, we have 2 Programs with differenet samples, 0 and 1 ... 
  // Sampler_SetProgram( number );

#ifdef FILTERCHANNEL
  }
#endif
  
}  

void handleClock(void){
    // the Volca is sending Clock nonstop Serial.println("O Clock");  
}

void handlePitchBend( byte channel, int bend ){
  
  // Pitchbend is not a CC! and it is 14Bit!
  
  Serial.printf("Pitchbend %X %d\n", channel, bend);    
  // The Bend-Value goes from -8192 to 8191 
  // Normalize the Bend to a Value from 0 to 1 to use it in the sketch for effects; 
#ifdef FILTERCHANNEL
  if( channel == FILTERCHANNEL ){
#endif 
  globalBend = 1;
  if( bend > 0){
    // linear bend up
    globalBend = (float) 1.0f + (1.0f *bend) / 8191;
  } 

  if( bend < 0){
    // linear Bedn down: 0.5 - 1 = -8192 : 0
    globalBend = (float) 1.0f +  (1.0f * bend) / ( 8192*2 );
  } 

 
  Serial.printf("GlobalBend %X %3f\n", channel, globalBend );
 
  // Or someting if pitch is negativ and something else if the bend is positiv ... 
#ifdef FILTERCHANNEL
  }
#endif

}

void handleStart(void){
      Serial.println("> Start");  
}

void handleContinue(void){
      Serial.println("> Continue");  
}

void handleStop(void){
      Serial.println("# Stop");  
}

inline
void MIDI_Process(){
    MIDI.read(); 
}


/*
 * this function should be called continuously to ensure that incoming messages can be processed
 */

/*
void Midi_Process_xxxx()
{
    
     // watchdog to avoid getting stuck by receiving incomplete or wrong data
    
    static uint32_t inMsgWd = 0;
    static uint8_t inMsg[3];
    static uint8_t inMsgIndex = 0;

    //Choose Serial1 or Serial2 as required

    if (Serial2.available()){
        uint8_t incomingByte = Serial2.read();

#ifdef DUMP_SERIAL2_TO_SERIAL
        Serial.printf("%02x", incomingByte);
#endif
        // ignore live messages
        if(( incomingByte & 0xF0) == 0xF0 ){
            return;
        }

        if( inMsgIndex == 0 ){
            if( (incomingByte & 0x80) != 0x80 ){
                inMsgIndex = 1;
            }
        }

        inMsg[inMsgIndex] = incomingByte;
        inMsgIndex += 1;

        if (inMsgIndex >= 3){
#ifdef DUMP_SERIAL2_TO_SERIAL
            Serial.printf(">%02x %02x %02x\n", inMsg[0], inMsg[1], inMsg[2]);
#endif
            HandleShortMsg(inMsg);
            inMsgIndex = 0;
        }

        // reset watchdog to allow new bytes to be received
        inMsgWd = 0;
    }else{
        if (inMsgIndex > 0){
            inMsgWd++;
            if (inMsgWd == 0xFFF ){
                inMsgIndex = 0;
            }
        }
    }

}
*/
