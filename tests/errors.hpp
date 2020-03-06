/*
 * errors.hpp
 *
 *  Created on: 5 Mar 2020
 *      Author: julianporter
 */

#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <exception>
#include <string>
#include <sstream>
#include <alsa/asoundlib.h>

class ALSAError : std::exception {
private:
	std::string message;
	int error;
public:
	ALSAError(const int e) : std::exception() {
		message=snd_strerror(e);
		error=e;
	}
	ALSAError(const std::string &m) : message(m), error(0) {};
	ALSAError(const ALSAError &) = default;
	ALSAError & operator=(const ALSAError &) = default;
	virtual ~ALSAError() = default;

	const char * what() const noexcept {
		std::stringstream s;
		if(error!=0) s << "Error " << error << " ";
		if(message.size()>0) s << message;
		return s.str().c_str();
	}

	static void wrap(int code) {
		if(code<0) throw ALSAError(code);
	}
};




#endif /* ERRORS_HPP_ */
