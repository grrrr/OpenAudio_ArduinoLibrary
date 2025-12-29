/*
 * AudioMixer
 * 
 * AudioMixer4
 * Created: Patrick Radius, December 2016
 * Purpose: Mix up to 4 audio channels with individual gain controls.
 * Assumes floating-point data.
 *          
 * This processes a single stream fo audio data (ie, it is mono)       
 *
 * Extended to AudioMixer8
 * By: Chip Audette, OpenAudio, Feb 2017
 *          
 * MIT License.  use at your own risk.
*/

#ifndef AUDIOMIXER_F32_H
#define AUDIOMIXER_F32_H

#include <arm_math.h> 
#include <AudioStream_F32.h>

template <int inputs>
class AudioMixer_F32 : public AudioStream_F32 {
public:
  AudioMixer_F32() : AudioStream_F32(inputs, inputQueueArray) { setDefaultValues(); }
	AudioMixer_F32(const AudioSettings_F32 &settings) : AudioStream_F32(inputs, inputQueueArray) { setDefaultValues(); }
	
	void setDefaultValues(void) {
		for (int i=0; i<inputs; i++) multiplier[i] = 1.0;
	}
	
  virtual void update(void) {
    audio_block_f32_t *in, *out=NULL;

    //get the first available channel
    int channel = 0;
    while  (channel < inputs) {
      out = receiveWritable_f32(channel);
      if (out) break;
      channel++;
    }
    if (!out) return;  //there was no data output array.  so exit.

    if(!multiplier[channel])
      arm_fill_f32(0, out->data, out->length);
    else if(multiplier[channel] != 1)
      arm_scale_f32(out->data, multiplier[channel], out->data, out->length);

    //add in the remaining channels, as available
    for(channel++; channel < inputs; channel++) {
      if(multiplier[channel]) {
        in = receiveReadOnly_f32(channel);
        if (in) {
          if(multiplier[channel] == 1)
            arm_add_f32(out->data, in->data, out->data, out->length);
          else {
            audio_block_f32_t *tmp = allocate_f32();
              arm_scale_f32(in->data, multiplier[channel], tmp->data, tmp->length);
            arm_add_f32(out->data, tmp->data, out->data, tmp->length);
            AudioStream_F32::release(tmp);
          }

          AudioStream_F32::release(in);
        } else {
          //do nothing, this vector is empty
        }
      }
    }
    AudioStream_F32::transmit(out);
    AudioStream_F32::release(out);
  }

  void gain(unsigned int channel, float gain) {
    if (channel >= inputs || channel < 0) return;
    multiplier[channel] = gain;
  }

private:
  audio_block_f32_t *inputQueueArray[inputs];
  float multiplier[inputs];
};

using AudioMixer4_F32 = AudioMixer_F32<4>;
//GUI: inputs:4, outputs:1  //this line used for automatic generation of GUI node
//GUI: shortName:Mixer4

using AudioMixer8_F32 = AudioMixer_F32<8>;
//GUI: inputs:8, outputs:1  //this line used for automatic generation of GUI node
//GUI: shortName:Mixer8

#endif
