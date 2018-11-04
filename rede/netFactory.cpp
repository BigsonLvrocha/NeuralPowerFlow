/**
 * @brief implementation of power net object factory
 * 
 * @file netFactory.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <stdexcept>
#include "netFactory.hpp"
#include "powerNet.hpp"
#include "branch.hpp"

namespace neuralFlux {

NetFactory::NetFactory() {
}

NetFactory::NetFactory(const PowerNetPtr& net) {
    setNet(net);
}

void NetFactory::setNet(const PowerNetPtr& net) {
    addBarsFrom(net);
    addBranchesFrom(net);
}

PowerNetPtr NetFactory::newNet() {
    PowerNetPtr net(new PowerNet());
    addBarsTo(net);
    addBranchesTo(net);
    return net;
}

void NetFactory::addSlack(
    uint64_t id,
    double v,
    double teta,
    double p,
    double q,
    double bsh
) {
    m_busBar.push_back(BarInfo(id, Bar::Slack, v, teta, p, q, bsh));
}

void NetFactory::addPQ(
    uint64_t id,
    double p,
    double q,
    double v,
    double teta,
    double bsh
) {
    m_busBar.push_back(BarInfo(id, Bar::PQ, v, teta, p, q, bsh));
}

void NetFactory::addPV(
    uint64_t id,
    double p,
    double v,
    double teta,
    double q,
    double bsh
) {
    this->m_busBar.push_back(BarInfo(id, Bar::PV, v, teta, p, q, bsh));
}

void NetFactory::addBranch(
    uint64_t kId,
    uint64_t mId,
    double rkm,
    double xkm,
    double bshkm,
    double a,
    double phi
) {
    m_branch.push_back(BranchInfo(kId, mId, rkm, xkm, bshkm, a, phi));
}

void NetFactory::removeBar(size_t i) {
    for (size_t j=i+1, n = m_busBar.size() ; j < n ; j++) {
        m_busBar[j-1] = m_busBar[j];
    }
    m_busBar.pop_back();
}

void NetFactory::removeBranch(size_t i) {
    for (size_t j = i+1, n = m_branch.size() ; j < n ; j++) {
        m_branch[j-1] = m_branch[j];
    }
    m_branch.pop_back();
}

void NetFactory::clearNet() {
    clearBranches();
    clearBars();
}
void NetFactory::clearBranches() {
    m_branch.clear();
}
void NetFactory::clearBars() {
    m_busBar.clear();
}

void NetFactory::addBarsFrom(const PowerNetPtr& net) {
    BarPtr bar;
    for (size_t i=0, n=net->getNBars(); i < n ; i++) {
        bar = net->getBarByIndex(i);
        switch (bar->getType()) {
        case Bar::Slack:
            addSlack(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
            break;
        case Bar::PQ:
            addPQ(bar->getId(), bar->getP(), bar->getQ(), bar->getV(), bar->getTeta(), bar->getBsh());
            break;
        case Bar::PV:
            addPV(bar->getId(), bar->getP(), bar->getV(), bar->getTeta(), bar->getQ(), bar->getBsh());
            break;
        default:
            throw std::runtime_error("invalid bar added");
        }
    }
}

void NetFactory::addBranchesFrom(const PowerNetPtr& net) {
    BranchPtr branch;
    for (size_t i=0, n = net->getNBranches(); i < n ; i++) {
        branch = net->getBranchByIndex(i);
        addBranch(
            branch->getK()->getId(),
            branch->getM()->getId(),
            branch->getRkm(),
            branch->getXkm(),
            branch->getBshkm(),
            branch->getA(),
            branch->getPhi());
    }
}

void NetFactory::addBarsTo(const PowerNetPtr& net) {
    for (size_t i = 0, n = m_busBar.size() ; i < n; i++) {
        switch (m_busBar[i].getType()) {
        case Bar::Slack:
            net->addSlackBar(
                m_busBar[i].getId(),
                m_busBar[i].getV(),
                m_busBar[i].getTeta(),
                m_busBar[i].getP(),
                m_busBar[i].getQ(),
                m_busBar[i].getBsh());
            break;

        case Bar::PV:
            net->addPVBar(
                m_busBar[i].getId(),
                m_busBar[i].getP(),
                m_busBar[i].getV(),
                m_busBar[i].getTeta(),
                m_busBar[i].getQ(),
                m_busBar[i].getBsh());
            break;
        case Bar::PQ:
            net->addPQBar(
                m_busBar[i].getId(),
                m_busBar[i].getP(),
                m_busBar[i].getQ(),
                m_busBar[i].getV(),
                m_busBar[i].getTeta(),
                m_busBar[i].getBsh());
            break;
        default:
            throw std::runtime_error("unsuported bar type");
        }
    }
}

void NetFactory::addBranchesTo(const PowerNetPtr& net) {
    for (size_t i = 0, n = m_branch.size() ; i < n; i++) {
        net->connect(
            m_branch[i].getIdk(),
            m_branch[i].getIdm(),
            m_branch[i].getRkm(),
            m_branch[i].getXkm(),
            m_branch[i].getBshkm(),
            m_branch[i].getA(),
            m_branch[i].getPhi());
    }
}
}  // namespace neuralFlux
