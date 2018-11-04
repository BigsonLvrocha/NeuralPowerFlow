/**
 * @file barConnectionInfoTest.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief test the barConnection
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../rede/neuralFluxDeclare.hpp"
#include "../rede/barra.hpp"
#include "../rede/barConnectionInfo.hpp"

using neuralFlux::BarPtr;
using neuralFlux::Bar;
using neuralFlux::BarConnectionInfo;
using neuralFlux::BarConnectionInfoPtr;

BOOST_AUTO_TEST_CASE(test_constructor) {
    BarPtr bar = BarPtr(Bar::newSlack(1, 0));
    BarConnectionInfo barInfo(bar);
    BOOST_TEST(bar == barInfo.getBar());
    BOOST_TEST(3 == barInfo.getLevel());
}

BOOST_AUTO_TEST_CASE(test_apply_level) {
    BarPtr bar1 = BarPtr(Bar::newSlack(1, 0));
    BarPtr bar2 = BarPtr(Bar::newPQ(0.1, 0));
    BarConnectionInfoPtr barInfo1(new BarConnectionInfo(bar1, 0));
    BarConnectionInfoPtr barInfo2(new BarConnectionInfo(bar2));
    barInfo1->setBarConnection(barInfo2);
    barInfo2->setBarConnection(barInfo1);
    barInfo1->applyLevelToNeighbours();
}
