/**
 * @brief defines validator class for power network
 * 
 * @file powerNetValidator.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include "powerNetValidator.hpp"
#include "powerNet.hpp"
#include "branch.hpp"
#include "barra.hpp"

namespace neuralFlux {
PowerNetValidator::PowerNetValidator() {
}

PowerNetValidator::PowerNetValidator(PowerNetPtr net):m_net(net) {
}

void PowerNetValidator::setNet(const PowerNetPtr& net) {
    m_net = net;
}

void PowerNetValidator::completeCheck() {
    if (m_net.use_count() == 0) {
        throw std::runtime_error("network was not set");
    }
    checkSlack();
    checkIsolatedBar();
    checkIsland();
}

void PowerNetValidator::completeCheck(const PowerNetPtr& net) {
    setNet(net);
    completeCheck();
}

bool PowerNetValidator::hasIslandWithoutSlack() {
    cacheIds();
    while (sweepBars()) {
    }
    for (size_t i = 0, n = m_slackConnected.size(); i < n; i++) {
        if (!m_slackConnected[i]) {
            return true;
        }
    }
    return false;
}

void PowerNetValidator::cacheIds() {
    m_slackConnected = std::vector<bool>(m_net->getNBars(), false);
    m_id = std::vector<uint64_t>(m_net->getNBars(), 0);
    for (size_t i = 0, n = m_net->getNBars(); i < n; i++) {
        m_id[i] = m_net->getBarByIndex(i)->getId();
    }
}

size_t PowerNetValidator::getIdIndex(uint64_t id) {
    for (size_t i = 0, n = m_id.size(); i < n; i++) {
        if (m_id[i] == id) {
            return i;
        }
    }
    throw std::runtime_error("bar not found or not cached");
}

void PowerNetValidator::checkSlack() {
    if (m_net->getNSlack() == 0) {
        throw NetHasNoSlack();
    }
}

void PowerNetValidator::checkIsolatedBar() {
    for (size_t i = 0, n = m_net->getNBars(); i < n; i++) {
        if (m_net->getBarByIndex(i)->getNConnections() == 0) {
            throw NetHasIsolatedBar();
        }
    }
}

void PowerNetValidator::checkIsland() {
    if (hasIslandWithoutSlack()) {
        throw NetHasIslands();
    }
}

bool PowerNetValidator::sweepBars() {
    bool changed = false;
    for (size_t i = 0, n = m_slackConnected.size() ; i < n ; i++) {
        BarPtr bar = m_net->getBarByIndex(i);
        if (m_slackConnected[i]) {
            changed |= markBarsConnected(bar);
        } else {
            if (bar->getType() == Bar::Slack) {
                m_slackConnected[i] = true;
            }
            changed |= checkBarConnected(bar);
        }
    }
    return changed;
}

bool PowerNetValidator::markBarsConnected(const BarPtr& bar) {
    bool changed = false;
    for (size_t i = 0, n = bar->getNConnections(); i < n; i++) {
        BranchPtr branch = bar->getConnection(i);
        if (bar == branch->getK()) {
            if (!m_slackConnected[getIdIndex(branch->getM()->getId())]) {
                m_slackConnected[getIdIndex(branch->getM()->getId())] = true;
                changed = true;
            }
        } else {
            if (!m_slackConnected[getIdIndex(branch->getK()->getId())]) {
                m_slackConnected[getIdIndex(branch->getK()->getId())] = true;
                changed = true;
            }
        }
    }
    return changed;
}

bool PowerNetValidator::checkBarConnected(const BarPtr& bar) {
    for (size_t i = 0, n = bar->getNConnections(); i < n; i++) {
        BranchPtr branch = bar->getConnection(i);
        if (bar == branch->getK()) {
            if (m_slackConnected[getIdIndex(branch->getM()->getId())]) {
                m_slackConnected[getIdIndex(bar->getId())] = true;
                return true;
            }
        } else {
            if (m_slackConnected[getIdIndex(branch->getK()->getId())]) {
                m_slackConnected[getIdIndex(bar->getId())] = true;
                return true;
            }
        }
    }
    return false;
}
}  // namespace neuralFlux
