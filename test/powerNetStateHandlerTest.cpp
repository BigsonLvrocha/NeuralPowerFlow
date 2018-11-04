/**
 * @brief defines test methods for the powerNetStateHandler Calss
 * 
 * @file powerNetStateHandlerTest.cpp
 * @author your name
 * @date 2018-09-30
 * @copyright 2018
 */
#define BOOST_TEST_DYN_LINK

#include <random>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "../rede/netStateHandler.hpp"
#include "../rede/powerNet.hpp"
#include "../rede/branch.hpp"


namespace tt = boost::test_tools;
using neuralFlux::PowerNetPtr;
using neuralFlux::PowerNet;
using neuralFlux::NetStateHandler;

struct netFixture {
    netFixture():
        net(new PowerNet()),
        handler(net),
        eng(0) {
        net->addSlackBar(1, 1, 0);
        net->addPQBar(2, 0.2, 0);
        net->addPQBar(3, 0.2, 0);
        net->connect(1, 2, 0.1, 1.0);
        net->connect(2, 3, 0.1, 1.0);
        handler.setNet(net);
    }

    PowerNetPtr net;
    NetStateHandler handler;
    std::default_random_engine eng;
};


/***
    Testa mudança sobre uma barra
*/
BOOST_FIXTURE_TEST_CASE(saveBar, netFixture) {
    net->getBar(1)->setV(2.0);
    BOOST_TEST(net->getBar(1)->getV() == 2.0, tt::tolerance(0.001) );
    handler.resetState();
    BOOST_TEST(net->getBar(1)->getV() == 1.0, tt::tolerance(0.001));
}

/**
    Testa mudança sobre um ramo
*/
BOOST_FIXTURE_TEST_CASE(saveBranch, netFixture) {
    net->getBranch(1, 2)->setRkm(1.0);
    BOOST_TEST(net->getBranch(1, 2)->getRkm() == 1.0, tt::tolerance(0.001));
    handler.resetState();
    BOOST_TEST(net->getBranch(1, 2)->getRkm() == 0.1, tt::tolerance(0.001));
}

/**
    Testa mudanca com adição de barra
*/
BOOST_FIXTURE_TEST_CASE(saveAddBar, netFixture) {
    net->addPVBar(4, 0.2, 1);
    BOOST_TEST(net->getNBars() == 4u);
    handler.resetState();
    BOOST_TEST(net->getNBars() == 3u);
    BOOST_CHECK_THROW(net->getBar(4), std::runtime_error);
}

/**
    Testa mudança com adição de ramo
*/

BOOST_FIXTURE_TEST_CASE(saveAddBranch, netFixture) {
    net->connect(1, 3, 0.1, 0.2);
    BOOST_TEST(net->getNBranches() == 3u);
    BOOST_TEST(net->areConnected(1, 3) == true);
    handler.resetState();
    BOOST_TEST(net->getNBranches() == 2u);
    BOOST_TEST(net->areConnected(1, 3) == false);
}

BOOST_FIXTURE_TEST_CASE(shuffleStateSame, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0, 0, 0, 0, 0, 0, &eng);
    BOOST_TEST(net->getBar(2)->getP() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.001));
    BOOST_TEST(net->getBar(3)->getP() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.001));
    BOOST_TEST(net->getBar(4)->getP() == 0.2, tt::tolerance(0.001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.001));
}

BOOST_FIXTURE_TEST_CASE(shuffleStateP, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0.01, 0, 0, 0, 0, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() == 0.20043791837242497, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() == 0.20135772943348137, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}


BOOST_FIXTURE_TEST_CASE(shuffleStateP_, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0.0, 0.01, 0, 0, 0, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() < 0.20);
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() < 0.20);
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}

BOOST_FIXTURE_TEST_CASE(shuffleStateS, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0.01, 0, 0, 0, 0, 0, 0, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() > 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() > 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() > 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}


BOOST_FIXTURE_TEST_CASE(shuffleStateS_, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0.01, 0, 0, 0, 0, 0, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() < 0.20);
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() < 0.20);
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() < 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}

BOOST_FIXTURE_TEST_CASE(shuffleStateQ, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0, 0, 0.1, 0, 0, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}


BOOST_FIXTURE_TEST_CASE(shuffleStateQ_, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0, 0, 0, 0.1, 0, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() == 0.20);
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() == 0.20);
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}

BOOST_FIXTURE_TEST_CASE(shuffleStatePV, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0, 0, 0, 0, 0.1, 0, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() == 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() > 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}


BOOST_FIXTURE_TEST_CASE(shuffleStatePV_, netFixture) {
    net->addPVBar(4, 0.2, 1.0);
    handler.shuffleNet(0, 0, 0, 0, 0, 0, 0, 0.1, &eng);
    BOOST_TEST(net->getBar(1)->getP() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(1)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(2)->getP() == 0.20);
    BOOST_TEST(net->getBar(2)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(3)->getP() == 0.20);
    BOOST_TEST(net->getBar(3)->getQ() == 0.0, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getP() < 0.2, tt::tolerance(0.0001));
    BOOST_TEST(net->getBar(4)->getQ() == 0.0, tt::tolerance(0.0001));
}
