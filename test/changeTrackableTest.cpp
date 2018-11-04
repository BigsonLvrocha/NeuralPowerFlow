/**
 * @brief tests the change trackable class
 * 
 * @file changeTrackableTest.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-24
 * @copyright 2018
 */

#include <iostream>
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../rede/barra.hpp"
#include "../rede/branch.hpp"
#include "../util/complexutils.h"

namespace tt = boost::test_tools;
using neuralFlux::BarPtr;
using neuralFlux::Bar;
using neuralFlux::BranchPtr;
using neuralFlux::Branch;

BOOST_AUTO_TEST_CASE(barChange) {
    BarPtr bar = Bar::newSlack(1, 0);
    bar->clearChanged();

    bar->setBsh(0.1);
    BOOST_TEST(bar->isChanged());
    bar->clearChanged();
    bar->setBsh(0.1);
    BOOST_TEST(!bar->isChanged());

    bar->setType(Bar::PQ);
    BOOST_TEST(bar->isChanged());
    bar->clearChanged();
    bar->setType(Bar::PQ);
    BOOST_TEST(!bar->isChanged());
}

BOOST_AUTO_TEST_CASE(branchChange) {
    BarPtr bar1 = Bar::newSlack(1, 0);
    BarPtr bar2 = Bar::newPQ(-0.2, 0);
    BranchPtr branch = Branch::connectBranch(bar1, bar2, 0.1, 0.2, 0.3, 0.4, 0.5);
    branch->clearChanged();
    branch->setRkm(0.1);
    branch->setXkm(0.2);
    branch->setBshkm(0.3);
    branch->setA(0.4);
    branch->setPhi(0.5);
    BOOST_TEST(!branch->isChanged());
    branch->setRkm(0.2);
    BOOST_TEST(branch->isChanged());
    branch->clearChanged();
    branch->setXkm(0.3);
    BOOST_TEST(branch->isChanged());
    branch->clearChanged();
    branch->setBshkm(0.4);
    BOOST_TEST(branch->isChanged());
    branch->clearChanged();
    branch->setA(0.5);
    BOOST_TEST(branch->isChanged());
    branch->clearChanged();
    branch->setPhi(0.6);
    BOOST_TEST(branch->isChanged());
    branch->clearChanged();
}
