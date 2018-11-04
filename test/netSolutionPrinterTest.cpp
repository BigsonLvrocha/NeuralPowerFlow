/**
 * @file barConnectionInfoTest.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief test the Pmu connection
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#define BOOST_TEST_DYN_LINK

#include <random>
#include <fstream>
#include <vector>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../rede/neuralFluxDeclare.hpp"
#include "../rede/cdfReader.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/netStateHandler.hpp"
#include "../rede/pmuConnector.hpp"
#include "../rede/netSolutionPrinter.hpp"
#include "../rede/powerFlowSolver.hpp"
#include "../rede/gausSiedelSolver.hpp"
#include "../rede/barConnectionInfo.hpp"

using neuralFlux::PowerNetPtr;
using neuralFlux::NetStateHandler;
using neuralFlux::PmuConnector;
using neuralFlux::CdfReader;
using neuralFlux::NetSolutionPrinter;
using neuralFlux::GausSeidelSolver;
using neuralFlux::BarConnectionInfoPtr;
using std::ifstream;
using std::ofstream;
using std::vector;

BOOST_AUTO_TEST_CASE(net_solution_printer_complete_solution) {
    CdfReader reader;
    ifstream input("/home/luiz/projects/neuralPowerFlow/netData/ieee14cdf.txt");
    PowerNetPtr net = reader.readNet(input);
    PmuConnector conn(net);
    std::default_random_engine eng(0);
    std::vector<BarConnectionInfoPtr> pmuData = conn.insertPmus(&eng);
    std::vector<int> pmuPos = conn.getPmuBars(pmuData);
    NetStateHandler stateHandler(net);
    GausSeidelSolver solver(net);
    stateHandler.shuffleNet(
        0.01,
        0.20,
        0.01,
        0.02,
        0.005,
        0.005,
        0.01,
        0.01,
        &eng);
    solver.solveNet(true, 0.00001, 50000);
    ofstream inputFile("input.csv");
    ofstream resolutionFile("output.csv");
    NetSolutionPrinter printer(net);
    printer.printHeaderWithPmu(inputFile, resolutionFile, pmuPos);
    printer.printSolutionsWithPmu(inputFile, resolutionFile, pmuPos);
}
