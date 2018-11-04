/**
 * @file cdfReader.hpp
 * @author your name (you@domain.com)
 * @brief defines header for cdf reader file
 * @version 0.1
 * @date 2018-10-02
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef REDE_CDFREADER_HPP_
#define REDE_CDFREADER_HPP_

#include <string>
#include <fstream>
#include "neuralFluxDeclare.hpp"
using std::string;
using std::ifstream;

namespace neuralFlux {

/**
 * @brief class for reading common data format for ieee bus system data
 * 
 */
class CdfReader {
 public:
    CdfReader();

    /**
     * @brief reads a cdf file and returns the corresponding powerNet object structure
     * 
     */
    PowerNetPtr readNet(ifstream &in);

 private:
    double readBasePower(ifstream &input);
    void readBuses(ifstream &input, PowerNetPtr net, float basePower);
    void readBranches(ifstream &input, PowerNetPtr net, float basePower);
    BarInfo readBusLine(char *line, float basePower);
    BranchInfo readBranchLine(char *line, float beasPower);
};
}  // namespace neuralFlux
#endif  // REDE_CDFREADER_HPP_
