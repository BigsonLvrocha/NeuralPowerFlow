/**
 * @file powerCalcTest.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-03
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#define BOOST_TEST_DYN_LINK
#include "../libraries/Eigen/Eigen"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../rede/barra.hpp"
#include "../rede/branch.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/powerCalc.hpp"
#include "../rede/admitanceCalc.hpp"
#include "../util/complexutils.h"


namespace tt = boost::test_tools;

using neuralFlux::PowerNet;
using neuralFlux::PowerNetPtr;
using neuralFlux::complexo;

struct FfPower2 {
    FfPower2():net( new PowerNet() ) {
        net->addSlackBar(1, 1., 0.);
        net->addPQBar(2, 0.2, 0.);
        net->addPQBar(3, 0.2, 0.);
        net->addPQBar(4, 0.2, 0.);
        net->addPQBar(5, 0.2, 0.);

        net->connect(1, 2, 0., 0.1, 0., 0.98);
        net->connect(2, 3, 0., 0.1, 0., 1, 0.2);
        net->connect(2, 5, 0.1, 0.1);
        net->connect(3, 4, 0.1, 1.0);
        net->connect(4, 5, 0.1, 1.0);

        net->setFlatStart();
        net->updateY();

        answer<< 0.,  -0.196,
                 1.98669, 0.399334,
                 -1.98669,  0.199334,
                 0.,  -0.,
                 0., -0.;
    }

    PowerNetPtr net;
    Eigen::Matrix<double, 5, 2> answer;
};

BOOST_FIXTURE_TEST_CASE(testPower, FfPower2) {
    complexo valor;
    for (size_t i = 0; i < 5; i++) {
        BOOST_TEST(net->getSk(i).real() == answer(i, 0), tt::tolerance(0.0001));
        BOOST_TEST(net->getSk(i).imag() == answer(i, 1), tt::tolerance(0.0001) );
    }
}
