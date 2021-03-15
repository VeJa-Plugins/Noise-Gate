/*
  ==============================================================================

	compressor_core.c
    Author: Jarno verheese &  Jan Janssen

  ==============================================================================
*/

#include "gate_core.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/*******************************************************************************
								global functions
*******************************************************************************/

void Gate_Init(gate_t *gate)
{
    gate->_alpha = 1.0f;
    gate->_rmsValue = 0.0f;
    gate->_keyValue = 0.0f;
    gate->_upperThreshold = 0.0f;
    gate->_lowerThreshold = 0.0f;
    gate->_attackTime = 0;
    gate->_decayTime = 0;
    gate->_holdTime = 0;
    gate->_attackCounter = 0;
    gate->_decayCounter = 0;
    gate->_holdCounter = 0;
    gate->_currentState = IDLE;
    gate->_tau = 0;

    ringbuffer_clear(&gate->window, 128);
}

void Gate_UpdateParameters(gate_t *gate, const uint32_t sampleRate, const uint32_t attack, const uint32_t hold,
                            const uint32_t decay, const uint32_t alpha, const float upperThreshold,
                            const float lowerThreshold)
{
    gate->_tau = sampleRate * 0.001f; //sample time in ms
    
    gate->_upperThreshold = powf(10.0f, (upperThreshold / 20.0f)); // dB to level
    gate->_lowerThreshold = powf(10.0f, (lowerThreshold / 20.0f));
    gate->_attackTime = attack * gate->_tau;
    gate->_decayTime = decay * gate->_tau;
    gate->_holdTime = hold * gate->_tau;
    gate->_alpha = alpha;
}

float Gate_ApplyGate(gate_t *gate, const float input, const float key)
{
    //get new keyValue
    gate->_keyValue = ringbuffer_push_and_calculate_power(&gate->window, key);

    switch (gate->_currentState)
    {
        case IDLE:
            gate->_rmsValue = sqrtf(gate->_keyValue * gate->_keyValue) * 0.707106781187;
            if (gate->_rmsValue > gate->_upperThreshold)
            {
                if (gate->_attackCounter > gate->_attackTime)
                {
                    gate->_currentState = HOLD;
                    gate->_holdCounter = 0;
                    gate->_attackCounter = 0;
                    return input;
                }
                else
                {
                    if (gate->_attackCounter != 0) 
                    {
                        gate->_attackCounter++;
                        return (input * powf((float)gate->_attackCounter, 2.0f) / powf((float)gate->_attackTime, 2.0f));
                    }
                    else 
                    {
                        gate->_attackCounter++;
                        return 0.0f;
                    }
                }
            }
            else
                return 0.0f;
        break;

        case HOLD:
            gate->_rmsValue = sqrtf(gate->_keyValue * gate->_keyValue) * 0.707106781187;
            if (gate->_rmsValue > gate->_lowerThreshold)
                gate->_holdCounter = 0;
            else if (gate->_holdCounter < gate->_holdTime)
                gate->_holdCounter++;
            else if (gate->_holdCounter >= gate->_holdTime)
            {
                gate->_currentState = DECAY;
                gate->_decayCounter = 0;
            }

            return input;
        break;

        case DECAY:
            if (gate->_decayCounter > gate->_decayTime)
            {
                gate->_currentState = IDLE;
                return 0.0f;
            }
            else
            {
                float dif = (float)gate->_decayCounter - (float)gate->_decayTime;
                if (gate->_decayCounter != 0) 
                {
                    gate->_decayCounter++;
                    return input * powf(dif, 2.0f) / powf((float)gate->_decayTime, 2.0f);
                }
                else
                {
                    gate->_decayCounter++;
                    return input;
                }
            }
        break;
    }  

    //ERROR
    return 0; 
}