/**
 * @brief tests the branch class
 * 
 * @file branchTest.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-24
 * @copyright 2018
 */

#include <iostream>
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../rede/barra.hpp"
#include "../rede/branch.hpp"

using neuralFlux::Bar;
using neuralFlux::BarPtr;
using neuralFlux::Branch;
using neuralFlux::BranchPtr;
using neuralFlux::complexo;

BOOST_AUTO_TEST_CASE(branchCreation) {
    BarPtr barK = Bar::newPQ(1, 1);
    BarPtr barM = Bar::newSlack(1, 0);
    BranchPtr branch = Branch::connectBranch(barK, barM, 0.1, 1.0);

    BOOST_TEST(barK->getNConnections() == 1u);
    BOOST_TEST(barK->getConnection(0) == branch);
    BOOST_TEST(barM->getNConnections() == 1u);
    BOOST_TEST(barM->getConnection(0) == branch);
    BOOST_CHECK_THROW(barK->getConnection(2), std::out_of_range);
    BOOST_CHECK(barK->getConnection(0)->getM() == barM);
}

BOOST_AUTO_TEST_CASE(branchDeletion) {
    BarPtr barK = Bar::newPQ(1, 1);
    BarPtr barM = Bar::newSlack(1, 0);

    BranchPtr branch = Branch::connectBranch(barK, barM, 0.1, 1.0);
    branch.reset();
    BOOST_TEST(barK->getNConnections() == 0u);
    BOOST_TEST(barM->getNConnections() == 0u);
}

BOOST_AUTO_TEST_CASE(multipleBranchDeletion) {
    BarPtr barK = Bar::newPQ(1, 1);
    BarPtr barM = Bar::newSlack(1, 0);
    BarPtr barL = Bar::newPQ(1, 1);

    BranchPtr branchKm = Branch::connectBranch(barK, barM, 0.1, 1.0);
    BranchPtr branchKl = Branch::connectBranch(barK, barL, 0.1, 1.0);
    BranchPtr branchMl = Branch::connectBranch(barM, barL, 0.1, 1.0);
    BOOST_TEST(barK->getNConnections() == 2u);
    BOOST_TEST(barM->getNConnections() == 2u);
    BOOST_TEST(barL->getNConnections() == 2u);
    branchKm.reset();
    BOOST_TEST(barK->getNConnections() == 1u);
    BOOST_TEST(barM->getNConnections() == 1u);
    BOOST_TEST(barL->getNConnections() == 2u);
    branchKl.reset();
    BOOST_TEST(barK->getNConnections() == 0u);
    BOOST_TEST(barM->getNConnections() == 1u);
    BOOST_TEST(barL->getNConnections() == 1u);
    BOOST_TEST(barM->getConnection(0) == branchMl);
    BOOST_TEST(barL->getConnection(0) == branchMl);
}

BOOST_AUTO_TEST_CASE(testId) {
    BarPtr barK = Bar::newPQ(1, 1, 1);
    BarPtr barM = Bar::newSlack(1, 0, 1);
    BranchPtr branch = Branch::connectBranch(barK, barM, 0.1, 1.0);
    BOOST_TEST(branch->getId() == 0x0000000100000001ul);
}

BOOST_AUTO_TEST_CASE(testSet) {
    BarPtr barK = Bar::newPQ(1, 1, 1);
    BarPtr barM = Bar::newSlack(1, 0, 1);
    BranchPtr branch = Branch::connectBranch(barK, barM, 0.1, 1.0);
    branch->setRkm(1);
    branch->setXkm(1);
    branch->setZkm(complexo(1, 1));
    branch->setYkm(complexo(1, 1));
    branch->setGkm(1);
    branch->setBkm(1);
    BOOST_TEST(branch->isChanged() == true);
    BOOST_TEST(branch->getRkm() == 1.0/2.0);
    BOOST_TEST(branch->getXkm() == -1.0/2.0);
}

BOOST_AUTO_TEST_CASE(testChange) {
    BarPtr barK = Bar::newPQ(1, 1, 1);
    BarPtr barM = Bar::newSlack(1, 0, 1);
    BranchPtr branch = Branch::connectBranch(barK, barM, 0.1, 1.0);
    branch->setGkm(1);
    BOOST_TEST(branch->isChanged() == true);
    branch->clearChanged();
    BOOST_TEST(branch->isChanged() == false);
}
