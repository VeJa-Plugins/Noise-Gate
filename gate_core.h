/*
  ==============================================================================

	gate_core.h
  Author: Jarno verheese &  Jan Janssen

  ==============================================================================
*/

#ifndef GATE_CORE_H_INCLUDED
#define GATE_CORE_H_INCLUDED

#include <stdint.h> 
#include "circular_buffer.h"

typedef enum {
    IDLE,
    HOLD,
    DECAY
} gate_state_t;

typedef struct GATE_T {
    float _alpha;
    float _rmsValue, _keyValue, _upperThreshold, _lowerThreshold;
    uint32_t _attackTime, _decayTime, _holdTime;
    uint32_t _attackCounter, _decayCounter, _holdCounter;
    uint32_t _currentState;
    uint32_t _tau;
    
    gate_state_t state;

    ringbuffer_t window;
} gate_t;

/// <summary>This method called to initialize the Gate</summary>
void Gate_Init(gate_t *gate);

/// <summary>This method called to apply the Gate to the input sample</summary>
/// <param name="input">Holds input sample</param> 
/// <param name="output">Holds the address where the output should be written to</param> 
float Gate_ApplyGate(gate_t *gate, const float input, const float key);
                
/// <summary>This method called to set the length of the window</summary>
/// <param name="length">Holds the value that determines the window length</param>  
void Gate_UpdateParameters(gate_t *gate, const uint32_t sampleRate, const uint32_t attack, const uint32_t hold,
                      const uint32_t decay, const uint32_t alpha, const float upperThreshold,
                      const float lowerThreshold);

#endif //GATE_CORE_H_INCLUDED
