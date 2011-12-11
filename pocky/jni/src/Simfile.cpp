/*
 * Simfile.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: jhk2
 */

#include "../include/Simfile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "pineapple/jni/extern/Common.h"
#include "pineapple/jni/extern/Engine.h"
#include <stdlib.h>

using namespace std;

namespace Pocky {

Simfile::Simfile() {
	data_ = new SimData();
	notes_ = new vector<SimNote>();
	position_ = -1;
}

Simfile::~Simfile() {
	delete data_;
	delete notes_;
}

Simfile* Simfile::parse(std::string filepath) {
	LOGI("parsing simfile at %s", filepath.c_str());
	Simfile *newsim = new Simfile();

	size_t size;
	unsigned char *raw = Pineapple::Engine::instance()->readResourceFromAPK(
			filepath.c_str(), size);
	string str = (const char *) raw;
	string actual = str.substr(0, size);

	bool header = false;
	bool data = false;

	char buffer[256];
	stringstream sstm(actual);
	string bufstring;

	LOGI("loaded resource, starting parse");
	while (!sstm.eof()) {
		sstm.getline(buffer, 256);
		bufstring = buffer;
		LOGI("read line: %c", buffer);
		if ((buffer[0]) == '#') {
			LOGI("comment, ignoring");
			continue;
		}
		if (!header) {
			// look for the header
			if (!strcmp(buffer, "[HEADER]")) {
				header = true;
			}
		} else {
			// past the header
			if (data) {
				LOGI("looking for data line");
				// reading data
				// look for absolute coordinate data
				float time;
				int x, y;
				if (sscanf(buffer, "%f %d,%d", &time, &x, &y) == 3) {
					SimNote ns;
					ns.time_ = time;
					ns.x_ = x;
					ns.y_ = y;
					newsim->notes_->push_back(ns);
					LOGI(
							"added note at time %f, position %d,%d", ns.time_, ns.x_, ns.y_);
				} else if (sscanf(buffer, "%f", &time) == 1) {
					// it's a rando
					SimNote ns;
					ns.time_ = time;
					ns.x_ = -1;
					ns.y_ = -1;
					newsim->notes_->push_back(ns);
					LOGI("added random note at time %f", ns.time_);

				} else {
					//					LOGI("weird line in data");
				}
			} else {
				//				LOGI("checking for data tag");
				// check for the data tag
				if (!strcmp(buffer, "[DATA]")) {
					LOGI("found data tag");
					data = true;
					continue;
				}
				//				LOGI("looking for header information");
				// otherwise we're reading the header information
				char key[32];
				char value[32];
				if (sscanf(buffer, "%s %s", key, value) != 2) {
					LOGI("found some gibberish");
					continue;
				}LOGI("scanning header data, key: %s, value: %s", key, value);
				if (!strcmp(key, "TITLE")) {
					newsim->getData()->title_ = value;
					LOGI("title: %s", newsim->getData()->title_.c_str());
				} else if (!strcmp(key, "BPM")) {
					newsim->getData()->bpm_ = atof(value);
					newsim->getData()->msperbeat_ = 1000.0
							/ (newsim->getData()->bpm_ / 60.0);
					LOGI("BPM: %f", newsim->getData()->bpm_);
				} else if (!strcmp(key, "LENGTH")) {
					newsim->getData()->length_ = atof(value);
					LOGI("length: %f", newsim->getData()->length_);
				} else if (!strcmp(key, "AUTHOR")) {
					newsim->getData()->author_ = value;
					LOGI("author: %s", newsim->getData()->author_.c_str());
				} else if (!strcmp(key, "MUSIC")) {
					newsim->getData()->music_ = value;
					LOGI("music: %s", newsim->getData()->author_.c_str());
                                } else if (!strcmp(key, "OFFSET")) {
                                        newsim->getData()->offset_ = atof(value);
                                } else {
					//					LOGI("got a weird tag in the header");
				}
			}
		}
	}LOGI("finished parsing");
	newsim->position_ = 0;
	return newsim;
}

SimNote* Simfile::getNextNote() {
	if (position_ < 0) {
		LOGI("empty simfile, can't get next note");
		return NULL;
	}
//	LOGI("current position is %d, out of a total of %d", position_, notes_->size());
	return &notes_->at(position_);
}

int Simfile::incrementPosition() {
	position_++;
	if(position_ >= notes_->size()){
		position_ = -1;
	}
	return position_;
}
}
