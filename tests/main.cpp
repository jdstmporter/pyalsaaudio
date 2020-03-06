/*
 * main.cpp
 *
 *  Created on: 6 Mar 2020
 *      Author: julianporter
 */

#include <iostream>
#include "PCM.hpp"

int main(int argc, char **argv) {

	PCM p("hw:0",Direction::Playback);
	std::cout << "Opened hw:0" << std::endl;
	auto props = p.properties();

	std::cout << "Rates (min,max) = (" << props.minRate << ", " << props.maxRate << ") :";
	switch(props.rateType) {
	case PCMRateType::Fixed:
		std::cout << "fixed rate of " << props.rates[0] << std::endl;
		break;
	case PCMRateType::Range:
		std::cout << "range from " << props.rates[0] << " to " << props.rates[1] << std::endl;
		break;
	case PCMRateType::Discrete:
		for(auto it=props.rates.begin();it!=props.rates.end();it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
}



