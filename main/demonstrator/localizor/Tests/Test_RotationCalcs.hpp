/*
 * Test_RotationCalcs.hpp
 *
 *  Created on: 24.03.2013
 *      Author: user
 */

#ifndef TEST_ROTATIONCALCS_HPP_
#define TEST_ROTATIONCALCS_HPP_

#include <RotationCalcs.hpp>
#include <Rnd.hpp>


class TestRotationCalcs {
public:
	void test() {

		vector<float> rots;
//		rots.push_back(-M_PI*0.6);
		rots.push_back(M_PI*2 -1e-10);
		rots.push_back(1e-10);

		cout << "input" << endl;
		for(int i=0;i<rots.size();i++) cout << i << ": " << rots[i] << endl;
		rearrangeRotations(rots);
		cout << "rearrangeRotation done" << endl;
		for(int i=0;i<rots.size();i++) cout << i << ": " << rots[i] << endl;

	}

	float average(vector<float> vals) {
		float sum = 0;
		for(int i=0;i<vals.size();i++)
			sum += vals[i];
		float avg = sum/vals.size();
		return avg;
	}

	void testAveraging() {
		int count = 1000000;
		for(int i=0;i<count;i++) {
			if(!testAveragingSingle()) throw;
		}
	}

	bool testAveragingSingle() {

		cout << "== start Test ==" << endl;
		vector<float> rots;

		Rnd rnd = Rnd();

		int count = rnd.nextInt(2,20);

		cout << "initializing rots" << endl;
		for(int i=0;i<count;i++) {
			rots.push_back(rnd.next(0.001,M_PI));
			cout << "  " << i << ") " << rots[i] << endl;
		}

		cout << "calculating avgbefore" << endl;
		normalizeRotation(rots);
		float avgbefore = average(rots);
		cout << "  " << "avgbefore: " << avgbefore << endl;

		cout << "adding shift" << endl;
		float shift = rnd.next(0,M_PI);
		for(int i=0;i<count;i++) {
			rots[i] += shift;
			cout << "  " << i << ") " << rots[i] << endl;
		}

		avgbefore += shift;
		normalizeRotation(avgbefore);
		cout << "  " << "avgbefore: " << avgbefore << endl;

		cout << "adding 2*PI" << endl;
		for(int i=0;i<count;i++) {
			rots[i] += M_PI*2 * rnd.nextInt(-10, 10);
			cout << "  " << i << ") " << rots[i] << endl;
		}

		cout << "calculating avgafter" << endl;
		rearrangeRotations(rots);
		float avgafter = average(rots);
		normalizeRotation(avgafter);
		cout << "  " << avgafter << endl;

		cout << "comparing" << endl;
		cout << "  " << avgbefore << " == " << avgafter << endl;
		cout << "  " << (avgbefore == avgafter) << endl;
		return(abs(avgbefore - avgafter) <= 0.0001);

	}

};


#endif /* TEST_ROTATIONCALCS_HPP_ */
