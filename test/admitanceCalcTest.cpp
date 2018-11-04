/**
 * @brief 
 * 
 * @file admitanceCalcTest.cpp
 * @author Luiz Victor Linhares Rocha <luizvictorlrocha@gmail.com>
 * @date 2018-09-22
 * @copyright 2018
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../libraries/Eigen/Eigen"
#include "../rede/barra.hpp"
#include "../rede/branch.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/admitanceCalc.hpp"
#include "../util/complexutils.h"


namespace tt = boost::test_tools;

using neuralFlux::PowerNet;
using neuralFlux::PowerNetPtr;
using neuralFlux::Bar;
using neuralFlux::BarPtr;
using neuralFlux::BranchPtr;
using neuralFlux::AdmitanceCalc;

struct FPower {
    FPower() : net(new PowerNet()) {
        net->addSlackBar(1, 1., 0.);
        net->addPQBar(2, 0.3, 0.);
        net->addPQBar(3, 0.3, 0.);
        net->addPQBar(4, 0.3, 0.);
        net->addPQBar(5, 0.3, 0.);

        net->connect(1, 2, 0., 0.1, 0., 0.98);
        net->connect(2, 3, 0., 0.1, 0., 1., 0.2);
        net->connect(2, 5, 0.1, 0.1);
        net->connect(3, 4, 0.1, 1.0);
        net->connect(4, 5, 0.1, 1.0);

        answer << std::complex<double>(0., -9.604),
            std::complex<double>(0, 9.8),
            std::complex<double>(0., 0.),
            std::complex<double>(0., 0.),
            std::complex<double>(0., 0.),
            std::complex<double>(0., 9.8),
            std::complex<double>(5., -25.),
            std::complex<double>(1.98669, 9.80067),
            std::complex<double>(0., 0.),
            std::complex<double>(-5., 5.),
            std::complex<double>(0., 0.),
            std::complex<double>(-1.98669, 9.80067),
            std::complex<double>(0.0990099, -10.9901),
            std::complex<double>(-0.0990099, 0.990099),
            std::complex<double>(0., 0.),
            std::complex<double>(0., 0.),
            std::complex<double>(0., 0.),
            std::complex<double>(-0.0990099, 0.990099),
            std::complex<double>(0.19802, -1.9802),
            std::complex<double>(-0.0990099, 0.990099),
            std::complex<double>(0., 0.),
            std::complex<double>(-5., 5.),
            std::complex<double>(0., 0.),
            std::complex<double>(-0.0990099, 0.990099),
            std::complex<double>(5.09901, -5.9901);
    }

    ~FPower() {
    }

    PowerNetPtr net;
    Eigen::Matrix<std::complex<double>, 5, 5> answer;
};

BOOST_FIXTURE_TEST_CASE(Admitance11, FPower) {
    BarPtr bar = net->getBar(1);
    std::complex<double> y = AdmitanceCalc::getAdmitanceKk(bar);
    BOOST_TEST(y.real() == answer(0, 0).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(0, 0).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(Admitance22, FPower) {
    BarPtr bar = net->getBar(2);
    std::complex<double> y = AdmitanceCalc::getAdmitanceKk(bar);
    BOOST_TEST(y.real() == answer(1, 1).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(1, 1).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(Admitance33, FPower) {
    BarPtr bar = net->getBar(3);
    std::complex<double> y = AdmitanceCalc::getAdmitanceKk(bar);
    BOOST_TEST(y.real() == answer(2, 2).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(2, 2).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(Admitance44, FPower) {
    BarPtr bar = net->getBar(4);
    std::complex<double> y = AdmitanceCalc::getAdmitanceKk(bar);
    BOOST_TEST(y.real() == answer(3, 3).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(3, 3).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(Admitance55, FPower) {
    BarPtr bar = net->getBar(5);
    std::complex<double> y = AdmitanceCalc::getAdmitanceKk(bar);
    BOOST_TEST(y.real() == answer(4, 4).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(4, 4).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(Admitance12, FPower) {
    BranchPtr branch = net->getBranchByIndex(0);
    std::complex<double> y = AdmitanceCalc::getAdmitanceKm(branch);
    BOOST_TEST(y.real() == answer(0, 1).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(0, 1).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(Admitance21, FPower) {
    BranchPtr branch = net->getBranchByIndex(0);
    std::complex<double> y = AdmitanceCalc::getAdmitanceMk(branch);
    BOOST_TEST(y.real() == answer(1, 0).real(), tt::tolerance(0.001));
    BOOST_TEST(y.imag() == answer(1, 0).imag(), tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(admitance, FPower) {
    Eigen::MatrixXcd Y = AdmitanceCalc::getMatrix(net);
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            BOOST_TEST(Y(i, j).real() == answer(i, j).real(), tt::tolerance(0.001));
            BOOST_TEST(Y(i, j).imag() == answer(i, j).imag(), tt::tolerance(0.001));
        }
    }
}

BOOST_FIXTURE_TEST_CASE(NetAdmitance, FPower) {
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            BOOST_TEST(net->getY(i, j).real() == answer(i, j).real(), tt::tolerance(0.001));
            BOOST_TEST(net->getY(i, j).imag() == answer(i, j).imag(), tt::tolerance(0.001));
        }
    }
}

BOOST_FIXTURE_TEST_CASE(BranchOnOff, FPower) {
    net->getBranchByIndex(4)->setBranchOn(false);
    PowerNetPtr net2(new PowerNet());
    net2->addSlackBar(1, 1., 0.);
    net2->addPQBar(2, 0.3, 0.);
    net2->addPQBar(3, 0.3, 0.);
    net2->addPQBar(4, 0.3, 0.);
    net2->addPQBar(5, 0.3, 0.);
    net2->connect(1, 2, 0., 0.1, 0., 0.98);
    net2->connect(2, 3, 0., 0.1, 0., 1., 0.2);
    net2->connect(2, 5, 0.1, 0.1);
    net2->connect(3, 4, 0.1, 1.0);
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            BOOST_TEST(net->getY(i, j).real() == net2->getY(i, j).real(), tt::tolerance(0.001));
            BOOST_TEST(net->getY(i, j).imag() == net2->getY(i, j).imag(), tt::tolerance(0.001));
        }
    }
}
