#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../rede/barra.hpp"
#include "../rede/branch.hpp"
#include "../rede/powerNet.hpp"
#include <iostream>

using namespace neuralFlux;


BOOST_AUTO_TEST_CASE(networkCreate){

    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPQBar(2,0.3,0.);
    net->addPQBar(3,0.3,0.);
    net->addPQBar(4,0.3,0.);
    net->addPQBar(5,0.3,0.);

    net->connect(1,2,0.,0.1,0.,0.98);
    net->connect(2,3,0.,0.1,0.,1.,0.2);
    net->connect(2,5,0.1,0.1);
    net->connect(3,4,0.1,1.0);
    net->connect(4,5,0.1,1.0);

    BOOST_TEST(net->getBar(1)->getNConnections() == 1u);
    BOOST_TEST(net->getBar(2)->getNConnections() == 3u);
    BOOST_TEST(net->getBar(3)->getNConnections() == 2u);
    BOOST_TEST(net->getBar(4)->getNConnections() == 2u);
    BOOST_TEST(net->getBar(5)->getNConnections() == 2u);

    BOOST_TEST(net->getBranchByIndex(0)->getK() == net->getBar(1));
    BOOST_TEST(net->getBranchByIndex(1)->getK() == net->getBar(2));
    BOOST_TEST(net->getBranchByIndex(2)->getK() == net->getBar(2));
    BOOST_TEST(net->getBranchByIndex(3)->getK() == net->getBar(3));
    BOOST_TEST(net->getBranchByIndex(4)->getK() == net->getBar(4));

    BOOST_TEST(net->getBranchByIndex(0)->getM() == net->getBar(2));
    BOOST_TEST(net->getBranchByIndex(1)->getM() == net->getBar(3));
    BOOST_TEST(net->getBranchByIndex(2)->getM() == net->getBar(5));
    BOOST_TEST(net->getBranchByIndex(3)->getM() == net->getBar(4));
    BOOST_TEST(net->getBranchByIndex(4)->getM() == net->getBar(5));
}

BOOST_AUTO_TEST_CASE(networkDelete){

    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPQBar(2,0.3,0.);
    net->addPQBar(3,0.3,0.);
    net->addPQBar(4,0.3,0.);
    net->addPQBar(5,0.3,0.);

    net->connect(1,2,0.,0.1,0.,0.98);
    net->connect(2,3,0.,0.1,0.,1.,0.2);
    net->connect(2,5,0.1,0.1);
    net->connect(3,4,0.1,1.0);
    net->connect(4,5,0.1,1.0);

    net->clean();
    BOOST_TEST(net->getNBars() == 0u);
    BOOST_TEST(net->getNBranches() == 0u);
    BOOST_TEST(net->getNSlack() == 0u);
    BOOST_TEST(net->getNPQ() == 0u);
    BOOST_TEST(net->getNPV() == 0u);

}

BOOST_AUTO_TEST_CASE(networkCreateOrder){

    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPQBar(2,0.3,0.);
    net->addPQBar(3,0.3,0.);
    net->addPQBar(4,0.3,0.);
    net->addPQBar(5,0.3,0.);

    net->connect(1,2,0.,0.1,0.,0.98);
    net->connect(2,3,0.,0.1,0.,1.,0.2);
    net->connect(2,5,0.1,0.1);
    net->connect(3,4,0.1,1.0);
    net->connect(4,5,0.1,1.0);

    BOOST_TEST(net->getBar(1)->getNConnections() == 1u);
    BOOST_TEST(net->getBar(2)->getNConnections() == 3u);
    BOOST_TEST(net->getBar(3)->getNConnections() == 2u);
    BOOST_TEST(net->getBar(4)->getNConnections() == 2u);
    BOOST_TEST(net->getBar(5)->getNConnections() == 2u);

    BOOST_TEST(net->getBranchByIndex(0)->getK() == net->getBar(1));
    BOOST_TEST(net->getBranchByIndex(1)->getK() == net->getBar(2));
    BOOST_TEST(net->getBranchByIndex(2)->getK() == net->getBar(2));
    BOOST_TEST(net->getBranchByIndex(3)->getK() == net->getBar(3));
    BOOST_TEST(net->getBranchByIndex(4)->getK() == net->getBar(4));

    BOOST_TEST(net->getBranchByIndex(0)->getM() == net->getBar(2));
    BOOST_TEST(net->getBranchByIndex(1)->getM() == net->getBar(3));
    BOOST_TEST(net->getBranchByIndex(2)->getM() == net->getBar(5));
    BOOST_TEST(net->getBranchByIndex(3)->getM() == net->getBar(4));
    BOOST_TEST(net->getBranchByIndex(4)->getM() == net->getBar(5));
}

BOOST_AUTO_TEST_CASE(testInsertionSequence){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addPQBar(1,0,0);
    net->addSlackBar(2,1.,0.);
    net->addPVBar(3,1.,0.);

    BarPtr pq = net->getBar(1);
    BarPtr slackBar = net->getBar(2);
    BarPtr pv = net->getBar(3);

    BOOST_TEST(slackBar == net->getBarByIndex(0));
    BOOST_TEST(pv == net->getBarByIndex(1));
    BOOST_TEST(pq == net->getBarByIndex(2));
}

BOOST_AUTO_TEST_CASE(EmptyNetExcept){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    BOOST_CHECK_THROW(net->getBranchByIndex(0),std::runtime_error);
    BOOST_CHECK_THROW(net->getBar(0), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(branchOrderTest){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPQBar(2,0,0);
    net->connect(2,1,0.,1.);
    BOOST_TEST(net->getBranchByIndex(0)->getId() == Branch::calculateId(2,1));
}


BOOST_AUTO_TEST_CASE(BranchConnectionSortTest){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPVBar(2,1.,0.);
    net->addPQBar(3,0.,0.);

    net->connect(3,2,1.,1.);
    BOOST_TEST(net->getBranchByIndex(0)->getId() == Branch::calculateId(3,2));
}

BOOST_AUTO_TEST_CASE(BranchConnectionSortTest2){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPQBar(2,0.,0.);
    net->addPVBar(3,1.,0.);

    net->connect(3,2,1.,1.);
    BOOST_TEST(net->getBranchByIndex(0)->getId() == Branch::calculateId(3,2));
}

BOOST_AUTO_TEST_CASE(deleteBar){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPVBar(2,1.,0.);
    net->addPQBar(3,0,0);

    net->connect(3,2,1.,1.);
    BOOST_TEST(net->getNPQ() == 1u);
    BOOST_TEST(net->getNPV() == 1u);
    BOOST_TEST(net->getNSlack() == 1u);
    BOOST_TEST(net->getBar(2)->getNConnections() == 1u);
    BOOST_TEST(net->getBar(3)->getNConnections() == 1u);
    net->deleteBar(1);
    BOOST_TEST(net->getBar(2)->getNConnections() == 1u);
    BOOST_TEST(net->getBar(3)->getNConnections() == 1u);
    BOOST_CHECK_THROW(net->getBar(1), std::runtime_error);
    BOOST_TEST(net->getNSlack() == 0u);
    BOOST_TEST(net->getNPQ() == 1u);
    BOOST_TEST(net->getNPV() == 1u);
    BOOST_CHECK_THROW(net->deleteBar(1),std::runtime_error);
}

BOOST_AUTO_TEST_CASE(deleteAllBar){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPVBar(2,1.,0.);
    net->addPQBar(3,0,0);

    net->connect(3,2,1.,1.);
    net->deleteBar(1);
    net->deleteBranch(3,2);
    BOOST_TEST(net->getNBranches() == 0u);
    net->deleteBar(2);
    net->deleteBar(3);
}

BOOST_AUTO_TEST_CASE(netAtribution){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addSlackBar(1,1.,0.);
    net->addPVBar(2,1.,0.);
    net->addPQBar(3,0,0);

    PowerNet net2(*net.get());
    PowerNet net3;
    net3 = net2;
    BOOST_TEST(net2.getBarByIndex(0) != net->getBarByIndex(0));
    BOOST_TEST(net2.getBarByIndex(0) != net3.getBarByIndex(0));
}

BOOST_AUTO_TEST_CASE(getBar){
    PowerNet net;
    net.addSlackBar(1,1.,0.);
    net.addPVBar(2,1.,0.);
    net.addPQBar(3,0,0);
    net.addPVBar(4,1.,0.);
    net.addSlackBar(5,1.,0.);
    net.addPQBar(6,0,0);


    BOOST_TEST(net.getBar(1) == net.getBarByIndex(0));
    BOOST_TEST(net.getBar(2) == net.getBarByIndex(2));
    BOOST_TEST(net.getBar(3) == net.getBarByIndex(4));
    BOOST_TEST(net.getBar(4) == net.getBarByIndex(3));
    BOOST_TEST(net.getBar(5) == net.getBarByIndex(1));
    BOOST_TEST(net.getBar(6) == net.getBarByIndex(5));
}

BOOST_AUTO_TEST_CASE(getBarNeg){
    PowerNet net;
    net.addSlackBar(1,1.,0.);
    net.addPVBar(2,1.,0.);
    net.addPQBar(3,0,0);
    net.addPVBar(4,1.,0.);
    net.addSlackBar(5,1.,0.);
    net.addPQBar(6,0,0);


    BOOST_CHECK_THROW(net.getBar(7),std::runtime_error);
    BOOST_CHECK_THROW(net.getBarByIndex(6),std::runtime_error);
}


BOOST_AUTO_TEST_CASE(getBranch){
    PowerNet net;
    net.addSlackBar(1,1.,0.);
    net.addPVBar(2,1.,0.);
    net.addPQBar(3,0,0);
    net.addPVBar(4,1.,0.);
    net.addSlackBar(5,1.,0.);
    net.addPQBar(6,0,0);

    net.connect(1, 2,1,1);
    net.connect(3, 4,1,1);
    net.connect(1, 3,1,1);
    net.connect(1, 5,1,1);

    BOOST_TEST(net.areConnected(1,3));
    BOOST_TEST(!net.areConnected(1,6));
    BOOST_TEST(net.areConnectedByIndex(0,4));
    BOOST_TEST(!net.areConnectedByIndex(0,5));


    BOOST_TEST(net.getBranch(Branch::calculateId(1,2)) == net.getBranchByIndex(0));
    BOOST_TEST(net.getBranch(Branch::calculateId(1,3)) == net.getBranchByIndex(1));
    BOOST_TEST(net.getBranch(Branch::calculateId(1,5)) == net.getBranchByIndex(2));
    BOOST_TEST(net.getBranch(Branch::calculateId(3,4)) == net.getBranchByIndex(3));

    BOOST_TEST(net.getBranch(1,2) == net.getBranchByIndex(0));
    BOOST_TEST(net.getBranch(1,3) == net.getBranchByIndex(1));
    BOOST_TEST(net.getBranch(1,5) == net.getBranchByIndex(2));
    BOOST_TEST(net.getBranch(3,4) == net.getBranchByIndex(3));

    BOOST_TEST(net.getBranchByIndex(0,2) == net.getBranchByIndex(0));
    BOOST_TEST(net.getBranchByIndex(0,4) == net.getBranchByIndex(1));
    BOOST_TEST(net.getBranchByIndex(0,1) == net.getBranchByIndex(2));
    BOOST_TEST(net.getBranchByIndex(4,3) == net.getBranchByIndex(3));
}

BOOST_AUTO_TEST_CASE(barByGeneric){
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addBar(Bar::PQ,1,0,0);
    net->addBar(Bar::Slack,2,1.,0.);
    net->addBar(Bar::PV,3,1.,0.);

    BarPtr pq = net->getBar(1);
    BarPtr slackBar = net->getBar(2);
    BarPtr pv = net->getBar(3);

    BOOST_TEST(slackBar == net->getBarByIndex(0));
    BOOST_TEST(pv == net->getBarByIndex(1));
    BOOST_TEST(pq == net->getBarByIndex(2));
}

BOOST_AUTO_TEST_CASE(sameIdBar)
{
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    net->addBar(Bar::PQ,1,0,0);
    net->addBar(Bar::Slack,2,1.,0.);
    net->addBar(Bar::PV,3,1.,0.);

    BOOST_CHECK_THROW(net->addSlackBar(1,1.,0.), std::runtime_error);

}

