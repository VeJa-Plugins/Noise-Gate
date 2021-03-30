/*
  ==============================================================================

 * VEJA NoiseGate
 * Copyright (C) 2021 Jan Janssen <jan@moddevices.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

#include "circular_buffer.h"
#include <math.h>

void ringbuffer_clear(ringbuffer_t *buffer, int size) 
{
    buffer->S=size;

	int q = 0;
	for ( q = 0; q < size; q++)
    {
    	buffer->m_buffer[q] = 0;
    }

    buffer->m_size = 0;
    buffer->m_front = 0;
    buffer->m_back  = buffer->S - 1;
}

void ringbuffer_push(ringbuffer_t *buffer) 
{
    buffer->m_back = (buffer->m_back + 1) % buffer->S;

    if(buffer->m_size == buffer->S)
    {
        buffer->m_front = (buffer->m_front + 1) % buffer->S;
    }
    else
    {
        buffer->m_size++;
    }
}
    
void ringbuffer_push_sample(ringbuffer_t *buffer, const float x) 
{
    ringbuffer_push(buffer);
    buffer->m_buffer[buffer->m_back] = x;
}
    
void ringbuffer_pop(ringbuffer_t *buffer) 
{
    if(buffer->m_size > 0 ) 
    {
        buffer->m_size--;
        buffer->m_front = (buffer->m_front + 1) % buffer->S;
    }
}
    
void ringbuffer_back_erase(ringbuffer_t *buffer, const int n) 
{
    if(n >= buffer->m_size)
    {
        ringbuffer_clear(buffer, buffer->S);
    }
    else 
    {
        buffer->m_size -= n;
        buffer->m_back = (buffer->m_front + buffer->m_size - 1) % buffer->S;
    }
}
    
void ringbuffer_front_erase(ringbuffer_t *buffer, const int n) 
{
    if(n >= buffer->m_size)
    {
        ringbuffer_clear(buffer, buffer->S);
    }
    else 
    {
        buffer->m_size -= n;
        buffer->m_front = (buffer->m_front + n) % buffer->S;
    }
}

int ringbuffer_peek_index(ringbuffer_t *buffer)
{
	int peek_index = 0;
	float peek_value = 0;
	
	int i = 0;
	for (i = 0; i < buffer->S; i++)
	{
		if (peek_value < buffer->m_buffer[i])
		{
			peek_value = buffer->m_buffer[i];
			peek_index = i;
		}
	}

	return peek_index;
}

float ringbuffer_push_and_calculate_power(ringbuffer_t *buffer, const float input)
{
    if (buffer->m_size++ < buffer->S)
    {
        //store and recalculate
        float pow = sqrtf(input * input) * (1.0f / buffer->S);
        //remove old sample and add new one to windowPower
        buffer->power += pow;
        ringbuffer_push_sample(buffer, pow);
    }
    else
    {
        //pop, adjust and recalculate
        float pow = sqrtf(input * input) * (1.0f / buffer->S);
        //remove old sample and add new one to windowPower
        buffer->power += pow - ringbuffer_front(buffer);
        ringbuffer_pop(buffer);
        ringbuffer_push_sample(buffer, pow);
    }

    return buffer->power;
}

float ringbuffer_front(ringbuffer_t *buffer) 
{ 
	return buffer->m_buffer[buffer->m_front]; 
}

float ringbuffer_back(ringbuffer_t *buffer) 
{ 
	return buffer->m_buffer[buffer->m_back]; 
}

float ringbuffer_get_val(ringbuffer_t *buffer, int index) 
{ 
	return buffer->m_buffer[index]; 
}

int ringbuffer_empty(ringbuffer_t *buffer) 
{ 
	return buffer->m_size == 0; 
}

int ringbuffer_full(ringbuffer_t *buffer)  
{ 
	return buffer->m_size == buffer->S; 
}

float * ringbuffer_get_first_pointer(ringbuffer_t *buffer)
{
    return &buffer->m_buffer[buffer->m_back];
}