/*
 * PCM.cpp
 *
 *  Created on: 5 Mar 2020
 *      Author: julianporter
 */

#include "PCM.hpp"
#include "errors.hpp"
#include <vector>
#include <algorithm>


static std::vector<snd_pcm_format_t> formats {
		SND_PCM_FORMAT_S8,
		SND_PCM_FORMAT_U8,
		SND_PCM_FORMAT_S16_LE,
		SND_PCM_FORMAT_S16_BE,
		SND_PCM_FORMAT_U16_LE,
		SND_PCM_FORMAT_U16_BE,
		SND_PCM_FORMAT_S24_LE,
		SND_PCM_FORMAT_S24_BE,
		SND_PCM_FORMAT_U24_LE,
		SND_PCM_FORMAT_U24_BE,
		SND_PCM_FORMAT_S32_LE,
		SND_PCM_FORMAT_S32_BE,
		SND_PCM_FORMAT_U32_LE,
		SND_PCM_FORMAT_U32_BE,
		SND_PCM_FORMAT_FLOAT_LE,
		SND_PCM_FORMAT_FLOAT_BE,
		SND_PCM_FORMAT_FLOAT64_LE,
		SND_PCM_FORMAT_FLOAT64_BE,
		SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
		SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
		SND_PCM_FORMAT_MU_LAW,
		SND_PCM_FORMAT_A_LAW,
		SND_PCM_FORMAT_IMA_ADPCM,
		SND_PCM_FORMAT_MPEG,
		SND_PCM_FORMAT_GSM,
		SND_PCM_FORMAT_SPECIAL,
		SND_PCM_FORMAT_S24_3LE,
		SND_PCM_FORMAT_S24_3BE,
		SND_PCM_FORMAT_U24_3LE,
		SND_PCM_FORMAT_U24_3BE,
		SND_PCM_FORMAT_S20_3LE,
		SND_PCM_FORMAT_S20_3BE,
		SND_PCM_FORMAT_U20_3LE,
		SND_PCM_FORMAT_U20_3BE,
		SND_PCM_FORMAT_S18_3LE,
		SND_PCM_FORMAT_S18_3BE,
		SND_PCM_FORMAT_U18_3LE,
		SND_PCM_FORMAT_U18_3BE,
};

static std::vector<unsigned> rates {
		4000,
		5512,
		8000,
		11025,
		16000,
		22050,
		32000,
		44100,
		48000,
		64000,
		88200,
		96000,
		176400,
		192000,
};


direction_t value(const Direction &d) {
	return static_cast<direction_t>(d);
}

PCM::PCM(const char *device_name,const Direction &direction) {
	snd_pcm_t *p;
	auto e=snd_pcm_open(&p, device_name,value(direction), SND_PCM_NONBLOCK);
	ALSAError::wrap(e);
	pcm=p;
}

PCM::~PCM() {
	if(pcm!=nullptr) {
		snd_pcm_close(pcm);
		pcm=nullptr;
	}
}

PCMProperties PCM::properties() const {
	snd_pcm_hw_params_t *params;
	snd_pcm_hw_params_alloca(&params);
	auto e = snd_pcm_hw_params_any(pcm, params);
	ALSAError::wrap(e);

	PCMProperties props;

	// channels

	unsigned min,max;
	ALSAError::wrap(snd_pcm_hw_params_get_channels_min(params, &min));
	ALSAError::wrap(snd_pcm_hw_params_get_channels_max(params, &max));

	for(auto ch=min;ch<=max;++ch) {
		if (!snd_pcm_hw_params_test_channels(pcm, params, ch)) {
			props.channels.push_back(ch);
		}
	}

	// formats

	std::for_each(formats.begin(),formats.end(),[this,params,&props](auto format) {
		if (!snd_pcm_hw_params_test_format(pcm, params, format)) {
			props.formats.push_back(format);
		}
	});

	// rates

	ALSAError::wrap(snd_pcm_hw_params_get_rate_min(params, &min,nullptr));
	ALSAError::wrap(snd_pcm_hw_params_get_rate_max(params, &max,nullptr));

	if(min==max) {
		props.rateType = PCMRateType::Fixed;
		props.rates.push_back(min);
	}
	else if(!snd_pcm_hw_params_test_rate(pcm, params, min + 1, 0)) {
		props.rateType = PCMRateType::Range;
		props.rates.push_back(min);
		props.rates.push_back(max);
	}
	else {
		props.rateType = PCMRateType::Discrete;
		std::for_each(rates.begin(),rates.end(),[this,params,&props](auto rate) {
			if (!snd_pcm_hw_params_test_rate(pcm, params, rate,0)) {
				props.rates.push_back(rate);
			}
		});
	}
	props.minRate=min;
	props.maxRate=max;

	return props;
}


