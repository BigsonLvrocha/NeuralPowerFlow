/**
 * @brief tests for the bar class
 * 
 * @file barraTest.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-24
 * @copyright 2018
 */

#include <iostream>
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../rede/barra.hpp"
#include "../util/complexutils.h"


namespace tt = boost::test_tools;
using neuralFlux::BarPtr;
using neuralFlux::Bar;

BOOST_AUTO_TEST_CASE(SlackTest) {
    BarPtr bar = Bar::newSlack(1, 0);
    BOOST_TEST(bar->getP() == 0, tt::tolerance(0.001));
    BOOST_TEST(bar->getQ() == 0, tt::tolerance(0.001));
    BOOST_TEST(bar->getV() == 1, tt::tolerance(0.001));
    BOOST_TEST(bar->getTeta() == 0, tt::tolerance(0.001));
    BOOST_TEST(bar->getBsh() == 0, tt::tolerance(0.001));
};

BOOST_AUTO_TEST_CASE(PQTest) {
    BarPtr bar = Bar::newPQ(1, 3);
    BOOST_TEST(bar->getP() == 1, tt::tolerance(0.001));
    BOOST_TEST(bar->getQ() == 3, tt::tolerance(0.001));
    BOOST_TEST(bar->getV() == 1, tt::tolerance(0.001));
    BOOST_TEST(bar->getBsh() == 0, tt::tolerance(0.001));
};

BOOST_AUTO_TEST_CASE(PVTest) {
    BarPtr bar = Bar::newPV(3, 1);
    BOOST_TEST(bar->getP() == 3, tt::tolerance(0.001));
    BOOST_TEST(bar->getQ() == 0, tt::tolerance(0.001));
    BOOST_TEST(bar->getV() == 1, tt::tolerance(0.001));
    BOOST_TEST(bar->getBsh() == 0, tt::tolerance(0.001));
};

BOOST_AUTO_TEST_CASE(setE) {
    BarPtr bar = Bar::newPQ(1, 1);

    bar->setE(std::complex<double>(1, 1));
    BOOST_TEST(bar->getTeta() == M_PI_4, tt::tolerance(0.001));
    BOOST_TEST(bar->getV() == sqrt(2), tt::tolerance(0.001));

    bar->setE(std::complex<double>(1, -1));
    BOOST_TEST(bar->getTeta() == -M_PI_4, tt::tolerance(0.001));
    BOOST_TEST(bar->getV() == sqrt(2), tt::tolerance(0.001));

    bar->setE(std::complex<double>(-1, 1));
    BOOST_TEST(bar->getV() == sqrt(2), tt::tolerance(0.001));
    BOOST_TEST(bar->getTeta() == 3*M_PI_4, tt::tolerance(0.001));

    bar->setE(std::complex<double>(-1, -1));
    BOOST_TEST(bar->getV() == sqrt(2), tt::tolerance(0.001));
    BOOST_TEST(bar->getTeta() == -3*M_PI_4, tt::tolerance(0.001));

    bar->setE(std::complex<double>(1./2., std::sqrt(3.)/2.));
    BOOST_TEST(bar->getV() == 1., tt::tolerance(0.001));
    BOOST_TEST(bar->getTeta() == M_PI/3., tt::tolerance(0.001));
}

BOOST_AUTO_TEST_CASE(setTeta) {
    BarPtr bar = Bar::newPQ(1, 1);
    std::complex<double> e;

    bar->setTeta(M_PI_2);
    e = bar->getE();
    BOOST_TEST(e.real() == 0., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == 1., tt::tolerance(0.001));

    bar->setTeta(-M_PI_2);
    e = bar->getE();
    BOOST_TEST(e.real() == 0., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == -1., tt::tolerance(0.001));


    bar->setTeta(M_PI_4);
    e = bar->getE();
    BOOST_TEST(e.real() == std::sqrt(2.)/2., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == std::sqrt(2.)/2., tt::tolerance(0.001));


    bar->setTeta(-M_PI_4);
    e = bar->getE();
    BOOST_TEST(e.real() == std::sqrt(2.)/2., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == -std::sqrt(2.)/2., tt::tolerance(0.001));

    bar->setTeta(3*M_PI_4);
    e = bar->getE();
    BOOST_TEST(e.real() == -std::sqrt(2.)/2., tt::tolerance(0.001));
    BOOST_TEST(e.imag() ==  std::sqrt(2.)/2., tt::tolerance(0.001));

    bar->setTeta(-3*M_PI_4);
    e = bar->getE();
    BOOST_TEST(e.real() == -std::sqrt(2.)/2., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == -std::sqrt(2.)/2., tt::tolerance(0.001));

    bar->setTeta(5*M_PI_4);
    e = bar->getE();
    BOOST_TEST(e.real() == -std::sqrt(2.)/2., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == -std::sqrt(2.)/2., tt::tolerance(0.001));


    bar->setTeta(M_PI/6.);
    e = bar->getE();
    BOOST_TEST(e.real() == std::sqrt(3.)/2., tt::tolerance(0.001));
    BOOST_TEST(e.imag() == 1/2., tt::tolerance(0.001));
}

BOOST_AUTO_TEST_CASE(setV) {
    BarPtr bar = Bar::newPQ(1, 1);
    std::complex<double> e;

    bar->setTeta(M_PI_4);
    bar->setV(2);
    e = bar->getE();
    BOOST_TEST(e.real() == std::sqrt(2.), tt::tolerance(0.001));
    BOOST_TEST(e.imag() == std::sqrt(2.), tt::tolerance(0.001));
}

BOOST_AUTO_TEST_CASE(setVZero) {
    BarPtr bar = Bar::newPQ(1, 1);
    std::complex<double> e;

    bar->setTeta(M_PI_4);
    bar->setV(0);
    e = bar->getE();

    BOOST_TEST(e.real()== 0, tt::tolerance(0.001));
    BOOST_TEST(e.imag()== 0, tt::tolerance(0.001));
    BOOST_TEST(bar->getTeta() == 0, tt::tolerance(0.001));
}
