/*
 * PCM.hpp
 *
 *  Created on: 5 Mar 2020
 *      Author: julianporter
 */

#ifndef PCM_HPP_
#define PCM_HPP_

#include <alsa/asoundlib.h>
#include "PCMProperties.hpp"

using direction_t = enum _snd_pcm_stream;

enum class Direction : int {
	Playback = SND_PCM_STREAM_PLAYBACK,
	Capture = SND_PCM_STREAM_CAPTURE
};



direction_t value(const Direction &);



class PCM {
private:
	snd_pcm_t *pcm = nullptr;

public:
	PCM(const char *device_name,const Direction &direction);
	virtual ~PCM();

	PCMProperties properties() const;


};

#endif /* PCM_HPP_ */
