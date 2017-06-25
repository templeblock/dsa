/*
 * TestRun.h
 *
 *  Created on: 26.09.2012
 *      Author: jung
 */

#ifndef TESTRUN_H_
#define TESTRUN_H_

#include<iostream>
#include<fstream>
#include<generic/types.h>

namespace dsa {

class TestRun {

	bool hasLogfile_;
	std::string logfileName_;

protected:

	UINT32 nruns_;
	std::ofstream logfile_;

	void println(std::string &line);	// >> cout
	void writeln(std::string &line);	// >> logfile_
	void log(std::string& line);		// = print() and write()
	void logResults();					// = printResults() and writeResults()
	virtual void printResults() = 0;	// >> cout
	virtual void writeResults() = 0;	// >> logfile
	virtual void update() = 0;			// make changes to the test

public:

	TestRun(const UINT32 nruns=1, std::string logfileName="");
	virtual ~TestRun();

	virtual void execute() = 0;
};

}
#endif /* TESTRUN_H_ */
