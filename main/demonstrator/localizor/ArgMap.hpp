/*
 * ArgMap.hpp
 *
 *  Created on: 10.03.2013
 *      Author: Bigontina
 */

#ifndef ARGMAP_HPP_
#define ARGMAP_HPP_

using namespace std;


/*
 * Parses the arguments and puts them in a map.
 * Arguments have to be in the form:
 * --<key> <value1> <value2> ...
 */
class ArgMap {

private:
	std::map<string,vector<string>> argmap;

public:
	ArgMap(const int argc, const char* const argv[]) {
		string key;
		for(int i=1;i<argc;i++) {
			string a = string(argv[i]);
			if(a.substr(0,2) == "--") {
				key = a.substr(2);
				argmap[key] = vector<string>();
			} else {
				if(argmap.find(key) == argmap.end()) {
					std::cout << "error: wrong arguments";
				} else {
					argmap[key].push_back(a);
				}
			}
		}
	}

	bool isset(string key) {
		return(argmap.find(key) != argmap.end());
	}

	bool isset(string key, int idx) {
		return(isset(key) && argmap[key].size()-1>=idx);
	}

	string getStr(string key, int idx, string defaultVal) {
		return (isset(key, idx) ? argmap[key][idx] : defaultVal);
	}

	int getInt(string key, int idx, int defaultVal) {
		return(isset(key, idx) ? std::atoi(argmap[key][idx].c_str()) : defaultVal);
	}

	float getFloat(string key, int idx, float defaultVal) {
		return(isset(key, idx) ? std::atof(argmap[key][idx].c_str()) : defaultVal);
	}

};


#endif /* ARGMAP_HPP_ */
