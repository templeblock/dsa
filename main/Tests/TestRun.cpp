/*
 * TestRun.cpp
 *
 *	Abstract class that allows for easily creating testcases (incl. logging)
 *
 *  Created on: 26.09.2012
 *      Author: jung
 */

#include "TestRun.h"

/**
 * Constructor
 *
 * @param nruns: number of runs to execute
 * @param logfileName: filename of the logfile (relative or full path)
 */
dsa::TestRun::TestRun(const UINT32 nruns, std::string logfileName) {
	nruns_ = nruns;
	hasLogfile_ = false;
	logfileName_ = logfileName;
	if(logfileName_.length() > 0) {
		try {
			logfile_.open(logfileName_.c_str());
			hasLogfile_ = true;
		} catch (std::ofstream::failure &failure) {
			std::cout << "Error opening logfile " << logfileName_ << ". Error was:" << std::endl;
			std::cout << failure.what() << std::endl;
		}
	} else
		nruns_ = nruns;
}

/**
 * Destructor
 */
dsa::TestRun::~TestRun() {
	if(hasLogfile_)
		logfile_.close();
}

/**
 * Print one line to std::cout
 *
 * @param line: text to print
 */
void dsa::TestRun::println(std::string& line) {
	std::cout << line << std::endl;
}

/**
 * Write one line to the logfile (if any)
 *
 * @param line: text to write
 */
void dsa::TestRun::writeln(std::string& line) {
	if(hasLogfile_)
		logfile_ << line << std::endl;
}

/**
 * Print one line on the console and write it to the logfile (if any). This is the same as running println and writeln.
 *
 * @param line: text to print and write
 */
void dsa::TestRun::log(std::string& line) {
	println(line);
	writeln(line);
}

/**
 * Write the determined results to both console and logfile (if any). This is the same as running printResults and writeResults.
 */
void dsa::TestRun::logResults() {
	printResults();
	writeResults();
}
