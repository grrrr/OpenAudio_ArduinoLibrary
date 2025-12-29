/*
 * AudioSwitch_F32.h
 * 
 * AudioSwitch4
 * Created: Chip Audette, OpenAudio, April 2019
 * Purpose: Switch one input to 4 outputs, which should only trigger one of the 4
 *      audio processing paths (therebys saving computation on paths that you don't
 *      care about).
 * Assumes floating-point data.
 * From Tympan Library, to OpenAudio_ArduinoLibrary June 2020, add 8 position class.
 *          
 * This processes a single stream of audio data (ie, it is mono)       
 *          
 * MIT License.  use at your own risk.
*/

#ifndef AUDIOSWITCH_OA_F32_H
#define AUDIOSWITCH_OA_F32_H

#include <AudioStream_F32.h>

template <int outputs>
class AudioSwitch_OA_F32 : public AudioStream_F32 {
public:
  AudioSwitch_OA_F32() : AudioStream_F32(1, inputQueueArray) { setDefaultValues(); }
	AudioSwitch_OA_F32(const AudioSettings_F32 &settings) : AudioStream_F32(1, inputQueueArray) { setDefaultValues(); }
	
	void setDefaultValues(void) {
		outputChannel = 0;
	}

  virtual void update(void) {
    audio_block_f32_t *out=NULL;

    out = receiveReadOnly_f32(0);
    if (!out) return;

    AudioStream_F32::transmit(out,outputChannel); //just output to the one channel
    AudioStream_F32::release(out);
  }

  int setChannel(unsigned int channel) {
    if (channel >= outputs || channel < 0) return outputChannel;  //invalid!  stick with previous channel
    return outputChannel = channel;
  }

private:
  audio_block_f32_t *inputQueueArray[1];
  int outputChannel;
};

using AudioSwitch4_OA_F32 = AudioSwitch_OA_F32<4>;
//GUI: inputs:1, outputs:4  //this line used for automatic generation of GUI node
//GUI: shortName:Switch4

using AudioSwitch8_OA_F32 = AudioSwitch_OA_F32<8>;
//GUI: inputs:1, outputs:8  //this line used for automatic generation of GUI node
//GUI: shortName:Switch8

#endif
