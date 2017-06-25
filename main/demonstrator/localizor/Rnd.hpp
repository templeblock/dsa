/*
 * Rnd.hpp
 *
 *  Created on: 18.03.2013
 *      Author: Bigontina
 */

#ifndef RND_HPP_
#define RND_HPP_

#include <cstdlib>

/*
 * Some helper functions to generate random values.
 * It is mainly used for generating random test cases.
 */
class Rnd {
public:
	static int seed;


	Rnd() {
		int now;
		time_t timer;
		struct tm y2k;
		double seconds;
		time(&timer);  /* get current time; same as: timer = time(NULL)  */
		seconds = difftime(timer,mktime(&y2k));
		int add = clock()+ seconds;
		Rnd::seed = Rnd::seed + add;
		srand(Rnd::seed);
	}

	float next(float min, float max) {
		return ((float)rand()/(float)RAND_MAX) * (max-min) + min;
	}

	float nextInt(int min, int max) {
		return((rand()%(max-min+1)) + min);
	}

	bool nextBool() {
		float rnd = this->next(0,2);
		return(rnd>1);
	}
};

int Rnd::seed = 0;

#endif /* RND_HPP_ */
