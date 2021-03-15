#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#define MAX_BUFFER_SIZE 256

typedef struct RINGBUFFER_T {
	int S;
	float m_buffer[MAX_BUFFER_SIZE];
	int m_size;
	int m_front;
	int m_back ;
	float power;
} ringbuffer_t;

void ringbuffer_clear(ringbuffer_t *buffer, int size); 
void ringbuffer_push(ringbuffer_t *buffer);
void ringbuffer_push_sample(ringbuffer_t *buffer, const float x); 
void ringbuffer_pop(ringbuffer_t *buffer); 
void ringbuffer_back_erase(ringbuffer_t *buffer, const int n);
void ringbuffer_front_erase(ringbuffer_t *buffer, const int n); 
int ringbuffer_peek_index(ringbuffer_t *buffer);
float ringbuffer_push_and_calculate_power(ringbuffer_t *buffer, const float input);
float ringbuffer_front(ringbuffer_t *buffer);
float ringbuffer_back(ringbuffer_t *buffer);
float ringbuffer_get_val(ringbuffer_t *buffer, int index); 
int ringbuffer_empty(ringbuffer_t *buffer); 
int ringbuffer_full(ringbuffer_t *buffer);
float * ringbuffer_get_first_pointer(ringbuffer_t *buffer);  

#endif // __RINGBUFFER_H__
