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

namespace Pocky{

struct SimData{
	std::string title_;
	double bpm_;
	double length_;// seconds
	std::string author_;

};

struct SimNote{
	double time_;
	int x_;
	int y_;
};

class Simfile {
public:
	Simfile();
	virtual ~Simfile();

	static void parse(std::string filepath);

	SimData *getData() {return data_;}

	SimNote *getNextNote();
	void incrementPosition();

protected:
	SimData *data_;
	std::vector<SimNote *> *notes_;
	int position_;
};
}
#endif /* SIMFILE_H_ */
