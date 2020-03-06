/*
 * PCMProperties.hpp
 *
 *  Created on: 6 Mar 2020
 *      Author: julianporter
 */

#ifndef PCMPROPERTIES_HPP_
#define PCMPROPERTIES_HPP_

#include <vector>
#include <alsa/asoundlib.h>

enum class PCMRateType {
	Fixed,
	Range,
	Discrete
};

struct PCMProperties {

	PCMRateType rateType;
	std::vector<unsigned> rates;
	unsigned minRate;
	unsigned maxRate;

	std::vector<snd_pcm_format_t> formats;

	std::vector<unsigned> channels;

};

#endif /* PCMPROPERTIES_HPP_ */
