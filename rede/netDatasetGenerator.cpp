/**
 * @file netDatasetGenerator.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <limits>
#include <random>
#include "netDatasetGenerator.hpp"
#include "powerNet.hpp"

typedef std::numeric_limits< double > dbl;
namespace neuralFlux {

NetDatasetGenerator::NetDatasetGenerator() {
}

vector<int> NetDatasetGenerator::generateFromFile(
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
    int perReport) {
    PowerNetPtr net = this->reader.readNet(netFile);
    return this->generateFromNet(
        net,
        outInputFile,
        outSolutionFile,
        eng,
        dataPoints,
        deltaS,
        deltaS_,
        deltaP,
        deltaP_,
        deltaQ,
        deltaQ_,
        deltaPV,
        deltaPV_,
        report,
        perReport);
}

vector<int> NetDatasetGenerator::generateFromNet(
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
    int perReport) {
    this->net = net;
    this->handler.setNet(net);
    this->connector.setNet(net);
    this->printer.setNet(net);
    this->solver.setNet(net);
    vector<int> pmus = this->connector.insertBestPmusDeterministic();
    this->handler.saveState();
    outInputFile.precision(dbl::max_digits10);
    outSolutionFile.precision(dbl::max_digits10);
    report << "pmus in bars: ";
    for (auto it = pmus.begin(); it != pmus.end(); it++) {
        report << *it << ",";
    }
    report << std::endl << std::endl;
    this->printer.printHeaderWithPmu(outInputFile, outSolutionFile, pmus);
    for (int i = 0; i < dataPoints ; i++) {
        if (i % perReport == 0) {
            report << i << "/" << dataPoints << std::endl;
        }
        this->handler.resetState();
        this->handler.shuffleNet(
            deltaS,
            deltaS_,
            deltaP,
            deltaP_,
            deltaQ,
            deltaQ_,
            deltaPV,
            deltaPV_,
            eng);
        this->solver.solveNet(true, 0.00001);
        this->printer.printSolutionsWithPmu(outInputFile, outSolutionFile, pmus);
    }
    return pmus;
}

}  // namespace neuralFlux
