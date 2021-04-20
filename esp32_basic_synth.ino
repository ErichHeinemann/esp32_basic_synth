/*
 * pinout of ESP32 DevKit found here:
 * https://circuits4you.com/2018/12/31/esp32-devkit-esp32-wroom-gpio-pinout/
 *
 * Author: Marcel Licence
 */
/* 2021-04-20 E.Heinemann, replaced custom MIDI Code the the Arduino MIDI Library 

*/

/*
 * required include files
 */
#include <arduino.h>
#include <driver/i2s.h>
#include <MIDI.h>
#include <WiFi.h>



/*
 * You can modify the sample rate as you want
 */
#define SAMPLE_RATE	48000


// #define ANALOG_INPUTS  // If this is defined, the system will use analog inputs of ADC 1 and ADC2 to react on inputs in the range of 0 - 2.5Volts!!


#ifdef ANALOG_INPUTS
volatile uint8_t analog_prescaler = 0;
uint8_t gpio_playbackspeed = 32; // Good GPIO-Ports on ADC1 with 12 Bit Resolution, max input is 2.5Volts. To use them with 5 Volts, please use 2 or more resistors to split the level. and protect the inputs from getting more than 3.3volts!!
uint8_t gpio_bitcrush =      33;
uint8_t gpio_biCutoff =      34;
uint8_t gpio_biReso   =      35;

uint8_t tmp_playbackspeed; // The Playbackspeed
uint8_t tmp_bitcrush;      // is bitcrusher active
uint8_t tmp_biCutoff;      // Cutoff-Frequency of the filter
uint8_t tmp_biReso;        // Resonance of the filter

uint8_t last_playbackspeed; // The Playbackspeed
uint8_t last_bitcrush;      // is bitcrusher active
uint8_t last_biCutoff;      // Cutoff-Frequency of the filter
uint8_t last_biReso;        // Resonance of the filter
#endif

// These values are only used to make an integration with MIDI, additional analog inputs or with an Menü slightly simplier
// The values could be 0 - 127 or floats ... 
uint8_t global_playbackspeed; // The Playbackspeed
uint8_t global_bitcrush;      // is bitcrusher active
uint8_t global_biCutoff;      // Cutoff-Frequency of the filter
uint8_t global_biReso;        // Resonance of the filter






/*
 * this is more an experiment required for other data formats
 */
union sampleTUNT
{
    uint64_t sample;
    int32_t ch[2];
} sampleDataU;


void setup()
{
    /*
     * this code runs once
     */
    delay(500);

    Serial.begin(115200);
    Serial.println();
    Serial.printf("Initialize Synth Module\n");
    Synth_Init();
    Serial.printf("Initialize I2S Module\n");
    setup_i2s();

    Serial.printf("Initialize Midi Module\n");
    Midi_Setup();

    Serial.printf("Turn off Wifi/Bluetooth\n");
#if 0
    setup_wifi();
#else
    WiFi.mode(WIFI_OFF);
#endif

#ifndef ESP8266
    btStop();
    // esp_wifi_deinit();
#endif

    Serial.printf("ESP.getFreeHeap() %d\n", ESP.getFreeHeap());
    Serial.printf("ESP.getMinFreeHeap() %d\n", ESP.getMinFreeHeap());
    Serial.printf("ESP.getHeapSize() %d\n", ESP.getHeapSize());
    Serial.printf("ESP.getMaxAllocHeap() %d\n", ESP.getMaxAllocHeap());

    Serial.printf("Firmware started successfully\n");

#if 0 /* activate this line to get a tone on startup to test the DAC */
    Synth_NoteOn(64);
#endif
}


/*
 * use this if something should happen every second
 * - you can drive a blinking LED for example
 */
inline void Loop_1Hz(void)
{

}


/*
 * our main loop
 * - all is done in a blocking context
 * - do not block the loop otherwise you will get problems with your audio
 */
void loop(){
    static uint32_t loop_cnt_1hz;
    static uint8_t loop_count_u8 = 0;

    loop_count_u8++;

    loop_cnt_1hz ++;
    if (loop_cnt_1hz >= SAMPLE_RATE){
        Loop_1Hz();
    }

#ifdef I2S_NODAC
    if( writeDAC(l_sample)){
        l_sample = Synth_Process();
    }
#else
   /* function returns always true / it blocks until samples are written to buffer */
    if( i2s_write_sample_32ch2(sampleDataU.sample)){
        float fl_sample, fr_sample;
        Synth_Process(&fl_sample, &fr_sample);
        sampleDataU.ch[0] = int32_t(fl_sample * 536870911.0f);
        sampleDataU.ch[1] = int32_t(fr_sample * 536870911.0f);
    }
#endif

    /*
     * Midi does not required to be checked after every processed sample
     * - we divide our operation by 8
     */
    if( loop_count_u8 % 8 == 0 ){
         MIDI_Process();
    }

#ifdef ANALOG_INPUTS
    // The valuees could be changed by Potentiometers or by MIDI
    analog_prescaler++;
    if( analog_prescaler > 23 ){
     tmp_playbackspeed = floor( analogRead( gpio_playbackspeed ) /32 );
     tmp_bitcrush = floor( analogRead( gpio_bitcrush ) /32 );
     tmp_biCutoff = floor( analogRead( gpio_biCutoff ) /32 );
     tmp_biReso = floor( analogRead( gpio_biReso ) /32 );
               
     if( tmp_playbackspeed !=  last_playbackspeed ){
        last_playbackspeed = tmp_playbackspeed;
        global_playbackspeed = tmp_playbackspeed;
        Sampler_SetPlaybackSpeed( NORM127MUL * global_playbackspeed );
     }  
     if( tmp_bitcrush !=  last_bitcrush ){
        last_bitcrush = tmp_bitcrush;
        global_bitcrush = tmp_bitcrush;
        // Effect_SetBitCrusher( NORM127MUL * global_bitcrush )
     }  
     if( tmp_biCutoff !=  last_biCutoff ){
        last_biCutoff = tmp_biCutoff;
        global_biCutoff = tmp_biCutoff;
        // Effect_SetBiCutoff( NORM127MUL * global_biCutoff )
     }  
     if( tmp_biReso !=  last_biReso ){
        last_biReso = tmp_biReso;
        global_biReso = tmp_biReso;
        // Effect_SetBiReso( NORM127MUL * global_biReso );
     }

    // ADD here your favorite things
    // trigger samples via buttons or control-voltagee or do something else here.
    // request data from touch-sensors?

    
     
     analog_prescaler = 0;
    }
    
#endif




}
