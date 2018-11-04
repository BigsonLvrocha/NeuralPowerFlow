/**
 * @brief test the net factory class
 * 
 * @file NetFactoryTest.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-24
 * @copyright 2018
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../rede/netFactory.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/branch.hpp"

using neuralFlux::NetFactory;
using neuralFlux::PowerNetPtr;
using neuralFlux::PowerNet;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE(barAdd) {
    NetFactory fac;

    fac.addSlack(1, 0.1, 0.2, 0.3, 0.4, 0.5);
    fac.addPQ(2, 0.1, 0.2, 0.3, 0.4, 0.5);
    fac.addPV(3, 0.1, 0.2, 0.3, 0.4, 0.5);

    PowerNetPtr net(fac.newNet());

    BOOST_TEST(net->getNBars() == 3u);
    BOOST_TEST(net->getNBranches() == 0u);

    BOOST_TEST(net->getBar(1)->getV() ==    0.1 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(1)->getTeta() == 0.2 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(1)->getP() == 0.3 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(1)->getQ() == 0.4 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(1)->getBsh() == 0.5 , tt::tolerance(0.01));

    BOOST_TEST(net->getBar(2)->getP() == 0.1, tt::tolerance(0.01));
    BOOST_TEST(net->getBar(2)->getQ() == 0.2 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(2)->getV() == 0.3 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(2)->getTeta() == 0.4 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(2)->getBsh() == 0.5 , tt::tolerance(0.01));

    BOOST_TEST(net->getBar(3)->getP() == 0.1 , tt::tolerance(0.01));
    BOOST_TEST(net->getBar(3)->getV() == 0.2, tt::tolerance(0.01));
    BOOST_TEST(net->getBar(3)->getTeta() == 0.3, tt::tolerance(0.01));
    BOOST_TEST(net->getBar(3)->getQ() == 0.4, tt::tolerance(0.01));
    BOOST_TEST(net->getBar(3)->getBsh() == 0.5, tt::tolerance(0.01));
}

BOOST_AUTO_TEST_CASE(barAddByNet) {
    PowerNetPtr net(new PowerNet);

    net->addSlackBar(1, 0.1, 0.2, 0.3, 0.4, 0.5);
    net->addPQBar(2, 0.1, 0.2, 0.3, 0.4, 0.5);
    net->addPVBar(3, 0.1, 0.2, 0.3, 0.4, 0.5);
    NetFactory fac(net);

    PowerNetPtr net2 = fac.newNet();

    BOOST_TEST(net2->getNBars() == 3u);
    BOOST_TEST(net2->getNBranches() == 0u);

    BOOST_TEST(net2->getBar(1)->getV() ==    0.1 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(1)->getTeta() == 0.2 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(1)->getP() == 0.3 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(1)->getQ() == 0.4 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(1)->getBsh() == 0.5 , tt::tolerance(0.01));

    BOOST_TEST(net2->getBar(2)->getP() == 0.1, tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(2)->getQ() == 0.2 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(2)->getV() == 0.3 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(2)->getTeta() == 0.4 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(2)->getBsh() == 0.5 , tt::tolerance(0.01));

    BOOST_TEST(net2->getBar(3)->getP() == 0.1 , tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(3)->getV() == 0.2, tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(3)->getTeta() == 0.3, tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(3)->getQ() == 0.4, tt::tolerance(0.01));
    BOOST_TEST(net2->getBar(3)->getBsh() == 0.5, tt::tolerance(0.01));
}


BOOST_AUTO_TEST_CASE(branchAdd) {
    NetFactory fac;
    fac.addSlack(1, 0.1, 0.2, 0.3, 0.4, 0.5);
    fac.addPQ(2, 0.1, 0.2, 0.3, 0.4, 0.5);
    fac.addPV(3, 0.1, 0.2, 0.3, 0.4, 0.5);
    fac.addBranch(1, 2, 0.1, 0.2, 0.3, 0.4, 0.5);
    fac.addBranch(2, 3, 0.1, 0.2, 0.3, 0.4, 0.5);
    PowerNetPtr net = fac.newNet();
    BOOST_TEST(net->areConnected(1, 2));
    BOOST_TEST(net->areConnected(2, 3));
    BOOST_TEST(!net->areConnected(1, 3));
    BOOST_TEST(net->getBranch(1, 2)->getRkm() == 0.1, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getXkm() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getBshkm() == 0.3, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getA() == 0.4, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getPhi() == 0.5, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getRkm() == 0.1, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getXkm() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getBshkm() == 0.3, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getA() == 0.4, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getPhi() == 0.5, tt::tolerance(0.001));
}

BOOST_AUTO_TEST_CASE(branchAddByNet) {
    PowerNetPtr net2(new PowerNet());
    net2->addSlackBar(1, 0.1, 0.2, 0.3, 0.4, 0.5);
    net2->addPQBar(2, 0.1, 0.2, 0.3, 0.4, 0.5);
    net2->addPVBar(3, 0.1, 0.2, 0.3, 0.4, 0.5);
    net2->connect(1, 2, 0.1, 0.2, 0.3, 0.4, 0.5);
    net2->connect(2, 3, 0.1, 0.2, 0.3, 0.4, 0.5);
    NetFactory fac(net2);
    PowerNetPtr net = fac.newNet();
    BOOST_TEST(net->areConnected(1, 2));
    BOOST_TEST(net->areConnected(2, 3));
    BOOST_TEST(!net->areConnected(1, 3));
    BOOST_TEST(net->getBranch(1, 2)->getRkm() == 0.1, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getXkm() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getBshkm() == 0.3, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getA() == 0.4, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(1, 2)->getPhi() == 0.5, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getRkm() == 0.1, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getXkm() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getBshkm() == 0.3, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getA() == 0.4, tt::tolerance(0.001));
    BOOST_TEST(net->getBranch(2, 3)->getPhi() == 0.5, tt::tolerance(0.001));
}
