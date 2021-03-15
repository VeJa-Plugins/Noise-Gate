#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

/**********************************************************************************************************************************************************/

#define PLUGIN_URI "http://VeJaPlugins.com/plugins/Release/NoiseGate"

#define MAP(x, Imin, Imax, Omin, Omax)      ( x - Imin ) * (Omax -  Omin)  / (Imax - Imin) + Omin;

typedef enum {
    PLUGIN_INPUT,
    PLUGIN_KEY,
    PLUGIN_OUTPUT,
    PLUGIN_THRESHOLD,
    PLUGIN_ATTACK,
    PLUGIN_HOLD,
    PLUGIN_DECAY
}PortIndex;

/**********************************************************************************************************************************************************/

typedef struct{
    
    //ports
    float*            input;
    float*              key;
    float*           output;
    float*        threshold;
    float*           attack;
    float*             hold;
    float*            decay;

} NoiseGate;

/**********************************************************************************************************************************************************/
//                                                                 local functions                                                                        //
/**********************************************************************************************************************************************************/

/**********************************************************************************************************************************************************/
static LV2_Handle
instantiate(const LV2_Descriptor*   descriptor,
double                              samplerate,
const char*                         bundle_path,
const LV2_Feature* const* features)
{
    NoiseGate* self = (NoiseGate*)malloc(sizeof(NoiseGate));

    return (LV2_Handle)self;
}
/**********************************************************************************************************************************************************/
static void connect_port(LV2_Handle instance, uint32_t port, void *data)
{
    NoiseGate* self = (NoiseGate*)instance;

    switch (port)
    {
        case PLUGIN_INPUT:
            self->input = (float*) data;
            break;
        case PLUGIN_KEY:
            self->key = (float*) data;
            break;
        case PLUGIN_OUTPUT:
            self->output = (float*) data;
            break;
        case PLUGIN_THRESHOLD:
            self->threshold = (float*) data;
            break;
        case PLUGIN_ATTACK:
            self->attack = (float*) data;
            break;
        case PLUGIN_HOLD:
            self->hold = (float*) data;
            break;
        case PLUGIN_DECAY:
            self->decay = (float*) data;
            break;
    }
}
/**********************************************************************************************************************************************************/
void activate(LV2_Handle instance)
{
    // TODO: include the activate function code here
}

/**********************************************************************************************************************************************************/
void run(LV2_Handle instance, uint32_t n_samples)
{
    NoiseGate* self = (NoiseGate*)instance;    

    uint32_t i;
    for (i = 0; i < n_samples; ++i)
    {
        self->input[i] = self->output[i];
    }
}

/**********************************************************************************************************************************************************/
void deactivate(LV2_Handle instance)
{
    // TODO: include the deactivate function code here
}
/**********************************************************************************************************************************************************/
void cleanup(LV2_Handle instance)
{
    free(instance);
}
/**********************************************************************************************************************************************************/
const void* extension_data(const char* uri)
{
    return NULL;
}
/**********************************************************************************************************************************************************/
static const LV2_Descriptor Descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};
/**********************************************************************************************************************************************************/
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    if (index == 0) return &Descriptor;
    else return NULL;
}
/**********************************************************************************************************************************************************/
