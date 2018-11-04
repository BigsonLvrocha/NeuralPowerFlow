/**
 * @brief implementation of abstract super class defining power flow solvers
 * 
 * @file powerFlowSolver.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include "powerFlowSolver.hpp"
#include "powerNet.hpp"
#include "powerCalc.hpp"

namespace neuralFlux {

PowerFlowSolver::PowerFlowSolver() {
}

PowerFlowSolver::PowerFlowSolver(PowerNetPtr net):
    m_net(net),
    m_validator(net) {
}

PowerFlowSolver::~PowerFlowSolver() {
}

void PowerFlowSolver::setNet(PowerNetPtr net) {
    m_net = net;
    m_validator.setNet(net);
}

PowerNetPtr PowerFlowSolver::getNet() {
    return m_net;
}

size_t PowerFlowSolver::solveNet(const bool reset, double error, unsigned int maxIt) {
    m_maxIt = maxIt;
    m_minError = error;
    m_it = 0;
    checkNet();
    if (reset) {
        m_net->setFlatStart();
    }
    m_net->updateY();
    solve();
    m_net->completeNetPowers();
    return m_it;
}

size_t PowerFlowSolver::setNetAndSolve(PowerNetPtr net, const bool reset, double error, unsigned int maxIt) {
    setNet(net);
    return solveNet(reset, error, maxIt);
}

void PowerFlowSolver::addIt() {
    ++m_it;
}

unsigned int PowerFlowSolver::getIt() {
    return m_it;
}

bool PowerFlowSolver::reachedMaxIt() {
    return m_it > m_maxIt;
}

bool PowerFlowSolver::reachMinError(const double error) {
    return m_minError > error;
}

void PowerFlowSolver::completeBars() {
    for (size_t i = 0, n = m_net->getNBars(); i < n; i++) {
        BarPtr bar = m_net->getBarByIndex(i);
        if (bar->getType() == Bar::Slack) {
            bar->setS(m_net->getSk(i));
        } else if (bar->getType() == Bar::PV) {
            bar->setQ(m_net->getSk(i).imag());
        }
    }
}

void PowerFlowSolver::checkNet() {
    if (m_net.use_count() == 0) {
        throw std::runtime_error("Power net was not set");
    }
    m_validator.completeCheck();
}
}  // namespace neuralFlux
