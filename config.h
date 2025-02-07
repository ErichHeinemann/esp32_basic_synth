/*
 * config.h
 *
 * Put all your project settings here (defines, numbers, etc.)
 * configurations which are requiring knowledge of types etc.
 * shall be placed in z_config.ino (will be included at the end)
 *
 *  Created on: 12.05.2021
 *      Author: Marcel Licence
 */

#define SAMPLE_BUFFER_SIZE  1

#ifndef CONFIG_H_
#define CONFIG_H_

// #define ESP32_AUDIO_KIT

#ifdef ESP32_AUDIO_KIT

/* on board led */
#define LED_PIN     19 // IO19 -> D5

#define ADC_INPUTS  8
#define ADC_MUL_S0_PIN  23
#define ADC_MUL_S1_PIN  18
#define ADC_MUL_S2_PIN  14
#define ADC_MUL_S3_PIN  5    /* <- not used, this has not been tested */
#define ADC_MUL_SIG_PIN 12

#else /* ESP32_AUDIO_KIT */

// Not the audio-kit
/* on board led */
#define LED_PIN     2

/*
 * Define and connect your PINS to DAC here
 */

#ifdef I2S_NODAC
#define I2S_NODAC_OUT_PIN   22  /* noisy sound without DAC, add capacitor in series! */
#else
/*
 * pins to connect a real DAC like PCM5102
 */
#define I2S_BCLK_PIN    25
#define I2S_WCLK_PIN    27
#define I2S_DOUT_PIN    26
#endif

// #define ADC_INPUTS  8
// #define ADC_MUL_S0_PIN  33
// #define ADC_MUL_S1_PIN  32
// #define ADC_MUL_S2_PIN  13
// #define ADC_MUL_SIG_PIN 12

#endif /* ESP32_AUDIO_KIT */

/*
 * You can modify the sample rate as you want
 */

#ifdef ESP32_AUDIO_KIT
#define SAMPLE_RATE 44100
#define SAMPLE_SIZE_16BIT
#else
#define SAMPLE_RATE 48000
#define SAMPLE_SIZE_32BIT
#endif

// #define ADC_TO_MIDI_ENABLED /* this will enable the adc module */
// #define ADC_TO_MIDI_LOOKUP_SIZE 8 /* should match ADC_INPUTS */



#endif /* CONFIG_H_ */
