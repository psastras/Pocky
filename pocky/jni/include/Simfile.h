/*
 * Simfile.h
 *
 *  Created on: Dec 6, 2011
 *      Author: jhk2
 */

#ifndef SIMFILE_H_
#define SIMFILE_H_

#include <string>
#include <vector>

namespace Pocky {

struct SimData {
	std::string title_, filepath_;
	double bpm_;
	double length_; // seconds
	double offset_;
	std::string author_;
	std::string music_;
	double msperbeat_;
};

struct SimNote {
	double time_;
	int x_;
	int y_;
};

class Simfile {
public:
	Simfile();
	virtual ~Simfile();

	static Simfile* parse(std::string filepath, bool data = true);

	SimData *getData() {
		return data_;
	}

	SimNote *getNextNote();
	int incrementPosition();
	int getPosition(){
		return position_;
	}

protected:
	SimData *data_;
	std::vector<SimNote> *notes_;
	int position_;
};
}
#endif /* SIMFILE_H_ */
