/*
 * This is a simple implementation of a delay line
 * - level adjustable
 * - feedback
 * - length adjustable
 *
 * Author: Marcel Licence
 */


/* max delay can be changed but changes also the memory consumption */
#define MAX_DELAY	11100

#define MAX_DELAY_L  10002
#define MAX_DELAY_R  11100

// #define MAX_DELAY  5100
/*
 * module variables
 */
float *delayLine_l;
float *delayLine_r;
float delayToMix = 0.2;
float delayFeedback = 0.2;
uint32_t delayLen_l = 10000;
uint32_t delayLen_r = 11098;

uint32_t delayIn_l = 0;
uint32_t delayIn_r = 0;
uint32_t delayOut_l = 0;
uint32_t delayOut_r = 0;

void Delay_Init(void)
{
    delayLine_l = (float *)malloc(sizeof(float) * MAX_DELAY);
    if (delayLine_l == NULL)
    {
        Serial.printf("No more heap memory!\n");
    }
    delayLine_r = (float *)malloc(sizeof(float) * MAX_DELAY);
    if (delayLine_r == NULL)
    {
        Serial.printf("No more heap memory!\n");
    }
    Delay_Reset();
}

void Delay_Reset(void)
{
    for (int i = 0; i < MAX_DELAY_L; i++)
    {
        delayLine_l[i] = 0;
    }
    for (int i = 0; i < MAX_DELAY_R; i++)
    {
        delayLine_r[i] = 0;
    }

}

void Delay_Process(float *signal_l, float *signal_r)
{
    delayLine_l[delayIn_l] = *signal_l;
    delayLine_r[delayIn_r] = *signal_r;

    delayOut_l = delayIn_l + (1 + MAX_DELAY_L - delayLen_l);
    delayOut_r = delayIn_r + (1 + MAX_DELAY_R - delayLen_r);

    if (delayOut_l >= MAX_DELAY_L)
    {
        delayOut_l -= MAX_DELAY_L;
    }
    if (delayOut_r >= MAX_DELAY_R)
    {
        delayOut_r -= MAX_DELAY_R;
    }

    *signal_l += delayLine_l[delayOut_l] * delayToMix;
    *signal_r += delayLine_r[delayOut_r] * delayToMix;

    delayLine_l[delayIn_l] += delayLine_l[delayOut_l] * delayFeedback;
    delayLine_r[delayIn_r] += delayLine_r[delayOut_r] * delayFeedback;

    delayIn_l ++;
    delayIn_r ++;

    if (delayIn_l >= MAX_DELAY_L)
    {
        delayIn_l = 0;
    }
    if (delayIn_r >= MAX_DELAY_R)
    {
        delayIn_r = 0;
    }
}

void Delay_SetFeedback(uint8_t unused, float value)
{
    delayFeedback = value;
    Serial.printf("delay feedback: %0.3f\n", value);
}

void Delay_SetLevel(uint8_t unused, float value)
{
    delayToMix = value;
    Serial.printf("delay level: %0.3f\n", value);
}

void Delay_SetLength(uint8_t unused, float value)
{
    delayLen_l = (uint32_t)(((float)MAX_DELAY_L - 1.0f) * value);
    delayLen_r = (uint32_t)(((float)MAX_DELAY_R - 1.0f) * value);
    Serial.printf("delay length: %0.3fms\n", delayLen_r * (1000.0f / ((float)SAMPLE_RATE)));
}
