/*
 * StrobeController.h
 *
 *  Created on: 21.10.2013
 *      Author: jung
 */

#ifndef STROBECONTROLLER_H_
#define STROBECONTROLLER_H_

#include <cassert>
#include <vector>

namespace dsa {

struct LampEnergy {
	std::size_t duration;	// duration in ms
	double amperage;		// amperage in ampere
	double voltage;			// voltage in volts

	LampEnergy(double amperage, std::size_t duration=0, double voltage=0.0);
	bool operator>(LampEnergy const &rhs) const;
	bool operator>=(LampEnergy const &rhs) const;
	bool operator<(LampEnergy const &rhs) const;
	bool operator<=(LampEnergy const &rhs) const;
};

class StrobeController {

protected:
	std::vector<bool> on_;
	std::vector<LampEnergy> maxNorm_;
	std::vector<LampEnergy> maxFlash_;

	bool isOn();
	void assertNormOverload(std::size_t output, LampEnergy &energy);
	void assertFlashOverload(std::size_t output, LampEnergy &energy);

public:
	StrobeController();
	StrobeController(std::vector<LampEnergy> &maxNorm, std::vector<LampEnergy> &maxFlash);
	virtual ~StrobeController();

	virtual bool isOn(std::size_t output=0);
	virtual bool startContinuous(std::size_t output, double amperage, double voltage=0.0);
	virtual bool startContinuous(std::size_t output, LampEnergy &energy);
	virtual bool startContinuous(std::size_t output=0);
	virtual bool stopContinuous(std::size_t output=0);
	virtual bool flash(std::size_t output, double amperage, std::size_t duration, double voltage=0.0);
	virtual bool flash(std::size_t output, LampEnergy &energy);
	virtual bool flash(std::size_t output=0);
};

}
#endif /* STROBECONTROLLER_H_ */
