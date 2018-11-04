/**
 * @brief implementation of network for state saving
 * 
 * @file netStateHandler.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <random>

#include <limits>
#include "../util/complexutils.h"
#include "netStateHandler.hpp"
#include "powerNet.hpp"
#include "branch.hpp"

using std::default_random_engine;

namespace neuralFlux {

NetStateHandler::NetStateHandler() {
}

NetStateHandler::NetStateHandler(const PowerNetPtr& net) {
    m_net = net;
    saveState();
}

void NetStateHandler::setNet(const PowerNetPtr& net) {
    m_net = net;
    saveState();
}

void NetStateHandler::resetState() {
    if (netStructureOk()) {
        resetBarsState();
        resetBranchesState();
    } else {
        m_net->clean();
        recreateNetBars();
        recreateNetBranches();
    }
}

void NetStateHandler::saveState() {
    saveBars();
    saveBranches();
}


bool NetStateHandler::netStructureOk() {
    if (m_net->getNBars() != m_bar.size()) {
        return false;
    }
    if (m_net->getNBranches() != m_branch.size()) {
        return false;
    }
    for (size_t i = 0, n = m_bar.size(); i < n ; i++) {
        if (m_net->getBar(m_bar[i].getId())->getType() != m_bar[i].getType()) {
            return false;
        }
    }
    for (size_t i = 0, n = m_branch.size() ; i < n ; i++) {
        if (!m_net->areConnected(m_branch[i].getIdk(), m_branch[i].getIdm())) {
            return false;
        }
    }
    return true;
}

void NetStateHandler::saveBars() {
    m_bar.clear();
    for (size_t i = 0, n = m_net->getNBars() ; i < n ; i++) {
        m_bar.push_back(BarInfo(m_net->getBarByIndex(i)));
    }
}

void NetStateHandler::saveBranches() {
    m_branch.clear();
    for (size_t i = 0, n = m_net->getNBranches(); i < n ; i++) {
        m_branch.push_back(BranchInfo(m_net->getBranchByIndex(i)));
    }
}

void NetStateHandler::recreateNetBranches() {
    m_net->cleanBranches();
    for (size_t i = 0, n = m_branch.size() ; i < n ; i++) {
        m_net->connect(
            m_branch[i].getIdk(),
            m_branch[i].getIdm(),
            m_branch[i].getRkm(),
            m_branch[i].getXkm(),
            m_branch[i].getBshkm(),
            m_branch[i].getA(),
            m_branch[i].getPhi());

        m_net->getBranch(
            m_branch[i].getIdk(),
            m_branch[i].getIdm())
            ->setBranchOn(m_branch[i].isBranchOn());
    }
}

void NetStateHandler::recreateNetBars() {
    m_net->clean();
    for (size_t i = 0, n = m_bar.size() ; i < n ; i++) {
        m_net->addBar(
            m_bar[i].getType(),
            m_bar[i].getId(),
            m_bar[i].getV(),
            m_bar[i].getTeta(),
            m_bar[i].getP(),
            m_bar[i].getQ(),
            m_bar[i].getBsh());
    }
}

void NetStateHandler::resetBarsState() {
    for (size_t i = 0 , n = m_bar.size() ; i < n ; i++) {
        BarPtr bar = m_net->getBar(m_bar[i].getId());
        bar->setE(complexPolar(m_bar[i].getV(), m_bar[i].getTeta()));
        bar->setS(complexo(m_bar[i].getP(), m_bar[i].getQ()));
        bar->setBsh(m_bar[i].getBsh());
    }
}

void NetStateHandler::resetBranchesState() {
    for (size_t i = 0 , n = m_branch.size() ; i < n ; i++) {
        BranchPtr branch = m_net->getBranch(m_branch[i].getIdk(), m_branch[i].getIdm());
        branch->setZkm(complexo(m_branch[i].getRkm(), m_branch[i].getXkm()));
        branch->setBshkm(m_branch[i].getBshkm());
        branch->setTrafo(complexPolar(m_branch[i].getA(), m_branch[i].getPhi()));
        branch->setBranchOn(m_branch[i].isBranchOn());
    }
}

void NetStateHandler::shuffleNet(
    double deltaS,
    double deltaS_,
    double deltaP,
    double deltaP_,
    double deltaQ,
    double deltaQ_,
    double deltaPV,
    double deltaPV_,
    std::default_random_engine *gen
) {
    double rangeS = deltaS + deltaS_;
    double rangeP = deltaP + deltaP_;
    double rangeQ = deltaQ + deltaQ_;
    double rangePV = deltaPV + deltaPV_;
    double rand = static_cast<double>(
        std::generate_canonical<double, std::numeric_limits<double>::digits>(*gen));
    double prodS = 1.0 + rand * rangeS - deltaS_;
    for (size_t i = 0, n = this->m_net->getNBars() ; i < n ; i++) {
        BarPtr bar = this->m_net->getBarByIndex(i);
        switch (bar->getType()) {
            case Bar::Slack: {
                break;
            }
            case Bar::PQ: {
                rand = static_cast<double>(
                    std::generate_canonical<double, std::numeric_limits<double>::digits>(*gen));
                double prodP = 1.0 + rand*rangeP - deltaP_;
                double prodQ = 1.0 + rand*rangeQ - deltaQ_;
                bar->setP(bar->getP()*prodP*prodS);
                bar->setQ(bar->getQ()*prodQ*prodS);
                break;
            }
            case Bar::PV: {
                rand = static_cast<double>(
                    std::generate_canonical<double, std::numeric_limits<double>::digits>(*gen));
                double prodPV = 1.0 + rand*rangePV - deltaPV_;
                bar->setP(bar->getP()*prodPV*prodS);
                break;
            }
        }
    }
}
}  // namespace neuralFlux
