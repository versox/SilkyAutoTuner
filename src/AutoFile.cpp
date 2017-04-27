/*
 * AutoFile.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Software
 */
#include "AutoFile.h"
#include "fio/parser.h"
#include <iostream>


AutoFile::AutoFile(const char* file)
: parser(inStream)
{
	inStream.open(file);
	if(inStream.fail()) {
		std::cout << "File loading failed!" << std::endl;
	}
}

std::string AutoFile::readCummulativeSilky(bool redIncluded)
{
	std::stringstream command;
	command << "SilkyDriveCmd{";
	
	std::vector<double> leftBlue;
	std::vector<double> rightBlue;
	std::vector<double> leftRed;
	std::vector<double> rightRed;

	int leftBlueCount = parser.readVector(leftBlue);
	int rightBlueCount = parser.readVector(rightBlue);
	int leftRedCount = 0;
	int rightRedCount = 0;


	if(leftBlueCount != 0 && leftBlueCount == rightBlueCount) {
		if(redIncluded) {
			leftRedCount = parser.readVector(leftRed);
			rightRedCount = parser.readVector(rightRed);

			if(leftRedCount != 0 && leftRedCount == rightRedCount) {
				command << "std::vector<double> {";
				command << leftBlue[0];
				for(int i = 1; i < leftBlue.size(); i++) {
					command << ", " << leftBlue[i];
				}
				command << "}, std::vector<double> {";
				command << rightBlue[0];
				for(int i = 1; i < rightBlue.size(); i++) {
					command << ", " << rightBlue[i];
				}
				command << "}, std::vector<double> {";
				command << leftRed[0];
				for(int i = 1; i < leftRed.size(); i++) {
					command << ", " << leftRed[i];
				}
				command << "}, std::vector<double> {";
				command << rightRed[0];
				for(int i = 1; i < rightRed.size(); i++) {
					command << ", " << rightRed[i];
				}
				command << "})";
			} else {
				std::cout << "*** FAILED *** Not the same number of segments cummulative" << std::endl;
				return NULL;
			}
		} else {
			command << "std::vector<double> {";
			command << leftBlue[0];
			for(int i = 1; i < leftBlue.size(); i++) {
				command << ", " << leftBlue[i];
			}
			command << "}, std::vector<double> {";
			command << rightBlue[0];
			for(int i = 1; i < rightBlue.size(); i++) {
				command << ", " << rightBlue[i];
			}
			command << "})";
		}
	} else {
		std::cout << "*** FAILED *** Not the same number of segments cummulative" << std::endl;
		return NULL;
	}
	
	return command.str();
}

std::string AutoFile::readDifferentialSilky(bool redIncluded)
{
	std::stringstream command;
	command << "SilkyDriveCmd{";
	std::vector<double> leftBlue;
	std::vector<double> rightBlue;
	std::vector<double> leftRed;
	std::vector<double> rightRed;
	int leftCountBlue = parser.readVector(leftBlue);
	int rightCountBlue = parser.readVector(rightBlue);
	int leftCountRed = 0;
	int rightCountRed = 0;

	if(leftCountBlue != 0 && leftCountBlue == rightCountBlue) {
		for(int i = 1; i < leftCountBlue; i++) {
			leftBlue[i] += leftBlue[i-1];
			rightBlue[i] += rightBlue[i-1];
		}
		if(redIncluded) {
			leftCountRed = parser.readVector(leftRed);
			rightCountRed = parser.readVector(rightRed);
			if(leftCountRed != 0 && leftCountRed == rightCountRed) {
				for(int i = 1; i < leftCountRed; i++) {
					leftRed[i] += leftRed[i-1];
					rightRed[i] += rightRed[i-1];
				}
				command << "std::vector<double> {";
				command << leftBlue[0];
				for(int i = 1; i < leftBlue.size(); i++) {
					command << ", " << leftBlue[i];
				}
				command << "}, std::vector<double> {";
				command << rightBlue[0];
				for(int i = 1; i < rightBlue.size(); i++) {
					command << ", " << rightBlue[i];
				}
				command << "}, std::vector<double> {";
				command << leftRed[0];
				for(int i = 1; i < leftRed.size(); i++) {
					command << ", " << leftRed[i];
				}
				command << "}, std::vector<double> {";
				command << rightRed[0];
				for(int i = 1; i < rightRed.size(); i++) {
					command << ", " << rightRed[i];
				}
				command << "})";
				return command.str();
			} else {
				std::cout << "*** FAILED *** Not the same number of segments differential" << std::endl;
				return NULL;
			}
		} else {
			command << "std::vector<double> {";
			command << leftBlue[0];
			for(int i = 1; i < leftBlue.size(); i++) {
				command << ", " << leftBlue[i];
			}
			command << "}, std::vector<double> {";
			command << rightBlue[0];
			for(int i = 1; i < rightBlue.size(); i++) {
				command << ", " << rightBlue[i];
			}
			command << "})";
			return command.str();
		}
	} else {
		std::cout << "*** FAILED *** Not the same number of segments differential" << std::endl;
		return NULL;
	}
}

std::string AutoFile::readWait() {
	std::stringstream command;
	command << "WaitCmd(";
	std::vector<double> parameters;
	if(parser.readVector(parameters) != 0) {
		double time = parameters[0];
		command << time << ")";
		return command.str();
	} else {
		return "";
	}
}

std::string AutoFile::readDriveStraight(bool redIncluded) {
	std::stringstream command;
	std::vector<double> parametersBlue;
	std::vector<double> parametersRed;
	double blueDistance;
	double redDistance;
	if(parser.readVector(parametersBlue) != 0) {
		blueDistance = parametersBlue[0];
	} else {
		return NULL;
	}
	if(redIncluded) {
		if(parser.readVector(parametersRed) != 0) {
			redDistance = parametersRed[0];
		} else {
			return NULL;
		}
		command << "AHRSDriveStraightCmd(" << blueDistance << ", " << redDistance << ")";
		return command.str();
	} else {
		command << "AHRSDriveStraightCmd(" << blueDistance << ")";
		return command.str();
	}
}

std::string AutoFile::readShoot() {
	return "ShootCmdGrp()";
}

std::string AutoFile::readSpinUp() {
	return "SpinUpCmd(BOILER_SHOT_SHOOTER_SPEED)";
}

std::string AutoFile::readGearFlaps(bool open) {
	std::stringstream command;

	command << "OpenGearFlapsCmd(" << open << ")";
	return command.str();
}

std::string AutoFile::readSilkyRotate() {
	std::stringstream command;
	std::vector<double> params;
	if(parser.readVector(params) != 0) {
		command << "SilkyRotateCmd(" << params[0] << ")";
	} else {
		command << __func__ << " Failed " << __LINE__;
	}
	return command.str();
}

std::vector<AutoFile::Operation>& AutoFile::readFile(void)
{
	char cmd;
	bool parallel;
	while(!parser.readCommand(cmd, parallel)) {

		std::string cb;

		switch(cmd) {
			//Normal silky
			case 's':
				cb = readCummulativeSilky(false);
				break;
			case 'S':
				cb = readCummulativeSilky(true);
				break;
			//Differential Silky
			case 'd':
				cb = readDifferentialSilky(false);
				break;
			case 'D':
				cb = readDifferentialSilky(true);
				break;
			//Wait
			case 'w':
			case 'W':
				cb = readWait();
				break;
			//Linear ie. drive straight
			case 'l':
				cb = readDriveStraight(false);
				break;
			case 'L':
				cb = readDriveStraight(true);
				break;
			//FIRE!!!
			case 'f':
			case 'F':
				cb = readShoot();
				break;
			//Spin UP
			case 'u':
			case 'U':
				cb = readSpinUp();
				break;
			case 'g':
				cb = readGearFlaps(false);
			case 'G':
				cb = readGearFlaps(true);
				break;
			case 'r':
			case 'R':
				cb = readSilkyRotate();
				break;
			case '#':
				break;
			default:
				std::cout << "unexpected command:" << cmd << std::endl;
				break;
		}
		if(cb[0] != '\0') {
			commands.push_back(Operation(cb, parallel));
		}
	}
	return commands;
}

int AutoFile::resetFile(const char *fileName)
{
	inStream.close();
	inStream.open(fileName);
	inStream.seekg(0);  // go back to beginning of the file.
	if(inStream.fail()) {
		std::cout << "File loading failed! : " << fileName << std::endl;
		return -1;
	}
	return 0;
}
