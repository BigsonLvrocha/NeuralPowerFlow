/**
 * @brief 
 * 
 * @file powerNetAssertionTest.cpp
 * @author your name
 * @date 2018-09-30
 * @copyright 2018
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include "../rede/powerNetValidator.hpp"
#include "../rede/powerNet.hpp"

using neuralFlux::PowerNetPtr;
using neuralFlux::PowerNetValidator;
using neuralFlux::NetHasNoSlack;
using neuralFlux::PowerNet;
using neuralFlux::NetHasIsolatedBar;
using neuralFlux::NetHasIslands;


BOOST_AUTO_TEST_CASE(NonSlack) {
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addPQBar(1, 1., 0.);
    net->addPVBar(2, 1., 1.);
    PowerNetValidator validator(net);
    BOOST_CHECK_THROW(validator.completeCheck(), NetHasNoSlack);
}

BOOST_AUTO_TEST_CASE(IsolatedBar) {
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(0, 1., 0.);
    net->addPQBar(1, 1., 0.);
    net->addPVBar(2, 1., 1.);
    PowerNetValidator validator(net);
    BOOST_CHECK_THROW(validator.completeCheck(), NetHasIsolatedBar);
}

BOOST_AUTO_TEST_CASE(Islanded) {
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(0, 1., 0.);
    net->addPQBar(1, 1., 0.);
    net->addPQBar(2, 1., 0.);
    net->addPVBar(3, 1., 1.);
    net->connect(0, 1, 1., 1.);
    net->connect(2, 3, 1., 1.);
    PowerNetValidator validator(net);
    BOOST_CHECK_THROW(validator.completeCheck(), NetHasIslands);
    BOOST_TEST(validator.hasIslandWithoutSlack());
}
