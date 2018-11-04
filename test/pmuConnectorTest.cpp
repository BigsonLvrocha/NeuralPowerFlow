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
#include "../rede/pmuConnector.hpp"

using neuralFlux::BarConnectionInfoPtr;
using neuralFlux::PmuConnector;
using neuralFlux::CdfReader;
using std::ifstream;
using neuralFlux::PowerNetPtr;
using std::vector;

BOOST_AUTO_TEST_CASE(pmu_conn_test_constructor) {
    CdfReader reader;
    ifstream input("/home/luiz/projects/neuralPowerFlow/netData/ieee14cdf.txt");
    if (!input) {
        BOOST_TEST_FAIL("error in file");
    }
    PowerNetPtr net = reader.readNet(input);
    PmuConnector conn(net);
    PowerNetPtr net2 = conn.getNet();
    BOOST_TEST(net == net2);
}

BOOST_AUTO_TEST_CASE(pmu_conn_test_14) {
    CdfReader reader;
    ifstream input("/home/luiz/projects/neuralPowerFlow/netData/ieee14cdf.txt");
    if (!input) {
        BOOST_TEST_FAIL("error in file");
    }
    PowerNetPtr net = reader.readNet(input);
    PmuConnector conn(net);
    BOOST_TEST(net->getNBars() == 14);
    std::default_random_engine eng(0);
    vector<BarConnectionInfoPtr> connections = conn.insertPmus(&eng);
    int score = conn.getScore();
    BOOST_TEST(score == 2176);
}

BOOST_AUTO_TEST_CASE(pmu_conn_14) {
    CdfReader reader;
    ifstream input("/home/luiz/projects/neuralPowerFlow/netData/ieee14cdf.txt");
    if (!input) {
        BOOST_TEST_FAIL("error in file");
    }
    PowerNetPtr net = reader.readNet(input);
    PmuConnector conn(net);
    vector<int> answer = conn.insertBestPmusDeterministic();
    BOOST_TEST(answer.size() == 2);
}
