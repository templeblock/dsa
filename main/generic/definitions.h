/*
 * definitions.h
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#include <ctime>

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

namespace dsa {

static const float PI = (float)3.1415926535898;
static const float SQRT2 = (float)1.4142135623730;
static const float msPerTick = 1000.0f/CLOCKS_PER_SEC;

// configuration options
// criteria for squares to consider
// canny options
static const int cannyLow = 70;
static const int cannyHigh = 180;
// other options
static const float minRotAngle = 0.0f;	// min angle for rotation

static const size_t UDP_BUFFER_SIZE = 1024;

static const char capital_alphabet[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
static const char small_alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

}
#endif /* DEFINITIONS_H_ */
