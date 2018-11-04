#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include "../rede/barra.hpp"
#include "../rede/branch.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/admitanceCalc.hpp"
#include "../rede/newtonSolver.hpp"
#include "../util/complexutils.h"
#include "../libraries/Eigen/Eigen"

namespace tt = boost::test_tools;

using neuralFlux::PowerNet;
using neuralFlux::PowerNetPtr;

struct FfPower {
    FfPower():net( new PowerNet() ) {
        net->addSlackBar(1, 1., 0.);
        net->addPQBar(2, -0.2, 0.);
        net->addPQBar(3, -0.2, 0.);
        net->addPQBar(4, -0.2, 0.);
        net->addPQBar(5, -0.2, 0.);
        net->connect(1, 2, 0., 0.1, 0., 0.98);
        net->connect(2, 3, 0., 0.1, 0., 1., 0.2);
        net->connect(2, 5, 0.1, 0.1);
        net->connect(3, 4, 0.1, 1.0);
        net->connect(4, 5, 0.1, 1.0);
        answer<< 0.809165,  0.129218,
                 0.970334, -0.0851953,
                 0.966433,  0.0737597,
                 0.93559,  -0.128705,
                 0.946063, -0.108161;
    }

    ~FfPower() {}

    PowerNetPtr net;
    Eigen::Matrix<double, 5, 2> answer;
};

BOOST_FIXTURE_TEST_CASE(powerFlowNewton, FfPower) {
    neuralFlux::NewtonSolver solver(net);

    solver.solveNet(true, 0.000001, 100000);

    BOOST_TEST(net->getBar(1)->getP() == answer(0, 0), tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == answer(0, 1), tt::tolerance(0.0001));
    for (size_t i = 1; i < 5; i++) {
        BOOST_TEST(net->getBar(i+1)->getV() == answer(i, 0), tt::tolerance(0.0001));
        BOOST_TEST(net->getBar(i+1)->getTeta() == answer(i, 1), tt::tolerance(0.0001));
    }
}

BOOST_FIXTURE_TEST_CASE(powerFlowPVNewton, FfPower) {
    neuralFlux::NewtonSolver solver(net);
    BOOST_TEST(net->getBar(5)->getNConnections() == 2u);
    net->deleteBranch(4, 5);
    BOOST_TEST(net->getBar(5)->getNConnections() == 1u);
    net->deleteBranch(2, 5);
    BOOST_TEST(net->getNBranches() == 3u);
    BOOST_TEST(net->getBar(5)->getNConnections() == 0u);
    net->deleteBar(5);
    net->addPVBar(5, -0.2, answer(4, 0));
    net->connect(2, 5, 0.1, 0.1);
    net->connect(4, 5, 0.1, 1.0);
    solver.solveNet(true, 0.000001, 100000);
    BOOST_TEST(net->getBar(1)->getP() == answer(0, 0), tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == answer(0, 1), tt::tolerance(0.0001));
    for (size_t i = 1 ; i < 5 ; i++) {
        BOOST_TEST(net->getBar(i+1)->getV() == answer(i, 0), tt::tolerance(0.0001));
        BOOST_TEST(net->getBar(i+1)->getTeta() == answer(i, 1), tt::tolerance(0.0001));
    }
    BOOST_TEST(net->getBar(5)->getQ() == 0., tt::tolerance(0.0001));
}

