/**
 * @file cdfReaderTest.cpp
 * @author your name (you@domain.com)
 * @brief defines testing for cdfReader class
 * @version 0.1
 * @date 2018-10-02
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <fstream>
#include <iostream>
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../rede/neuralFluxDeclare.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/cdfReader.hpp"

using neuralFlux::PowerNetPtr;
using neuralFlux::CdfReader;
using std::ifstream;
namespace tt = boost::test_tools;

const char filePath[] = "/home/luiz/projects/neuralPowerFlow/netData/ieee14cdf.txt";

BOOST_AUTO_TEST_CASE(ReadNet) {
    CdfReader reader = CdfReader();
    ifstream in = ifstream(filePath);
    PowerNetPtr net = reader.readNet(in);
    BOOST_TEST(net->getNBars() == 14);
    BOOST_TEST(net->getNBranches() == 20);
}
