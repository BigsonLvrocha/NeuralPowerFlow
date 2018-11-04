/**
 * @file netSolutionPrinter.hpp
 * @author your name (you@domain.com)
 * @brief defines a printer for a solved power net
 * @version 0.1
 * @date 2018-10-04
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef REDE_NETSOLUTIONPRINTER_HPP_
#define REDE_NETSOLUTIONPRINTER_HPP_

#include <ostream>
#include <vector>
#include "neuralFluxDeclare.hpp"

using std::ostream;
using std::vector;

namespace neuralFlux {
class NetSolutionPrinter {
 public:
    explicit NetSolutionPrinter(PowerNetPtr net);
    NetSolutionPrinter();

    NetSolutionPrinter& setNet(PowerNetPtr);
    PowerNetPtr getNet();
    NetSolutionPrinter& printHeaderWithPmu(ostream &streamInput, ostream &streamOutput, vector<int> pmus);
    NetSolutionPrinter& printSolutionsWithPmu(ostream &streamInput, ostream &streamOutput, vector<int> pmus);

 private:
    PowerNetPtr net;
};
}  // namespace neuralFlux

#endif  // REDE_NETSOLUTIONPRINTER_HPP_
