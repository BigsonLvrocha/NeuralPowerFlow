/**
 * @file netDatasetGenerator.hpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief class for generating a dataset given a powerNet
 * @version 0.1
 * @date 2018-10-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef REDE_NETDATASETGENERATOR_HPP_
#define REDE_NETDATASETGENERATOR_HPP_

#include <random>
#include <fstream>
#include <vector>
#include "neuralFluxDeclare.hpp"
#include "netStateHandler.hpp"
#include "netSolutionPrinter.hpp"
#include "gausSiedelSolver.hpp"
#include "cdfReader.hpp"
#include "pmuConnector.hpp"

using std::vector;
using std::ifstream;
using std::ofstream;

namespace neuralFlux {

class NetDatasetGenerator {
 public:
    NetDatasetGenerator();
    vector<int> generateFromFile(
        ifstream &netFile,
        ofstream &outInputFile,
        ofstream &outSolutionFile,
        std::default_random_engine *eng,
        uint32_t dataPoints,
        double deltaS,
        double deltaS_,
        double deltaP,
        double deltaP_,
        double deltaQ,
        double deltaQ_,
        double deltaPV,
        double deltaPV_,
        ostream &report,
        int perReport = 1000);

    vector<int> generateFromNet(
        PowerNetPtr net,
        ofstream &outInputFile,
        ofstream &outSolutionFile,
        std::default_random_engine *eng,
        uint32_t dataPoints,
        double deltaS,
        double deltaS_,
        double deltaP,
        double deltaP_,
        double deltaQ,
        double deltaQ_,
        double deltaPV,
        double deltaPV_,
        ostream &report,
        int perReport = 1000);

 private:
    PowerNetPtr net;
    NetStateHandler handler;
    CdfReader reader;
    NetSolutionPrinter printer;
    GausSeidelSolver solver;
    PmuConnector connector;
};

}

#endif  // REDE_NETDATASETGENERATOR_HPP_
