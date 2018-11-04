/**
 * @brief implementation of a power network used in calculations
 * 
 * @file powerNet.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <utility>
#include <iostream>
#include "powerNet.hpp"
#include "barra.hpp"
#include "branch.hpp"
#include "admitanceCalc.hpp"
#include "../util/complexutils.h"

namespace neuralFlux {

PowerNet::PowerNet(uint64_t id):
    Identifiable(id),
    m_nPQ(0),
    m_nPV(0),
    m_nSlack(0) {
}

PowerNet::~PowerNet() {
    m_branches.clear();  // branches must be deleted first
    m_busBars.clear();
}

PowerNet::PowerNet(const PowerNet& net):
    Identifiable(net.getId()),
    m_nPQ(0),
    m_nPV(0),
    m_nSlack(0) {
    for (size_t i = 0, n = net.getNBars() ; i < n ; i++) {
        const BarPtr bar = net.getBarByIndex(i);
        if (bar->getType() == Bar::Slack) {
            this->addSlackBar(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
        } else if (bar->getType() == Bar::PV) {
            this->addPVBar(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
        } else {
            this->addPQBar(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
        }
    }
    for (size_t i = 0, n = net.getNBranches(); i < n; i++) {
        const BranchPtr branch = net.getBranchByIndex(i);
        this->connect(
            branch->getK()->getId(),
            branch->getM()->getId(),
            branch->getRkm(),
            branch->getXkm(),
            branch->getBshkm(),
            branch->getA(),
            branch->getPhi());
    }
}

PowerNet& PowerNet::operator=(const PowerNet& net) {
    for (size_t i = 0, n = net.getNBars(); i < n; i++) {
        const BarPtr bar = net.getBarByIndex(i);
        if (bar->getType() == Bar::Slack) {
            this->addSlackBar(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
        } else if (bar->getType() == Bar::PV) {
            this->addPVBar(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
        } else {
            this->addPQBar(bar->getId(), bar->getV(), bar->getTeta(), bar->getP(), bar->getQ(), bar->getBsh());
        }
    }
    for (size_t i = 0, n = net.getNBranches(); i < n; i++) {
        const BranchPtr branch = net.getBranchByIndex(i);
        this->connect(
            branch->getK()->getId(),
            branch->getM()->getId(),
            branch->getRkm(),
            branch->getXkm(),
            branch->getBshkm(),
            branch->getA(),
            branch->getPhi());
    }
    return *this;
}

void PowerNet::addSlackBar(
    const size_t id,
    const double v,
    const double teta,
    const double p,
    const double q,
    const double bsh
) {
    if (checkBarIdExists(id)) {
        throw std::runtime_error("cannot create same id bar in the net");
    }
    m_busBars.push_back(Bar::newSlack(v, teta, id, p, q, bsh));
    ++m_nSlack;
    sortBackBar();
}

void PowerNet::addPQBar(
    const size_t id,
    const double p,
    const double q,
    const double v,
    const double teta,
    const double bsh
) {
    if (checkBarIdExists(id)) {
        throw std::runtime_error("cannot create same id bar in the net");
    }
    m_busBars.push_back(Bar::newPQ(p, q, id, v, teta, bsh));
    ++m_nPQ;
    sortBackBar();
}

void PowerNet::addBar(
    const Bar::barType type,
    const size_t id,
    const double v,
    const double teta,
    const double p,
    const double q,
    const double bsh
) {
    switch (type) {
    case Bar::Slack:
        addSlackBar(id, v, teta, p, q, bsh);
        break;
    case Bar::PV:
        addPVBar(id, p, v, teta, q, bsh);
        break;
    case Bar::PQ:
        addPQBar(id, p, q, v, teta, bsh);
        break;
    default:
        throw std::runtime_error("unsuported bar type");
    }
}

void PowerNet::addPVBar(
    const size_t id,
    const double p,
    const double v,
    const double teta,
    const double q,
    const double bsh
) {
    if (checkBarIdExists(id)) {
        throw std::runtime_error("cannot create same id bar in the net");
    }
    m_busBars.push_back(Bar::newPV(p, v, id, teta, q, bsh));
    ++m_nPV;
    sortBackBar();
}

void PowerNet::connect(
    size_t k,
    size_t m,
    const double rkm,
    const double xkm,
    const double bshkm,
    const double a,
    const double phi
) {
    connectByIndex(
        getBarIndex(k),
        getBarIndex(m),
        rkm,
        xkm,
        bshkm,
        a,
        phi);
}

void PowerNet::connectByIndex(
    size_t k,
    size_t m,
    const double rkm,
    const double xkm,
    const double bshkm,
    const double a,
    const double phi
) {
    if (k == m) {
        throw std::invalid_argument("cannot connect same bars");
    }
    if (k > m) {
        std::swap<size_t>(k, m);
    }
    try {  // TODO(lvrocha): do this more eficiently
        getBranchIndexByIndex(k, m);
        throw std::invalid_argument("cannot connect same bars twice");
    } catch(std::runtime_error e) {
        m_branches.push_back(Branch::connectBranch(m_busBars[k], m_busBars[m], rkm, xkm, bshkm, a, phi));
        sortBackBranch();
    }
}

void PowerNet::deleteBranch(uint64_t id) {
    deleteBranchByIndex(getBranchIndex(id));
}

void PowerNet::deleteBranchByIndex(size_t i) {
    if (m_branches.size() <= i) {
        throw std::runtime_error("branch does not exist");
    }
    for (size_t _i = i+1, n = m_branches.size() ; _i < n ; _i++) {
        m_branches[_i-1] = m_branches[_i];
    }
    m_branches.pop_back();
}

void PowerNet::deleteBranch(uint64_t kId, uint64_t mId) {
    deleteBranchByIndex(getBranchIndex(kId, mId));
}

void PowerNet::deleteBranchByBarIndex(size_t kI, size_t mI) {
    deleteBranchByIndex(getBranchIndexByIndex(kI, mI));
}

void PowerNet::deleteBar(size_t id) {
    deleteBarByIndex(getBarIndex(id));
}

void PowerNet::deleteBarByIndex(size_t i) {
    if (m_busBars.size() <= i) {
        throw std::runtime_error("Bar does not exists");
    }
    if (m_busBars[i]->getNConnections() != 0) {
        throw std::runtime_error("cannot delete bar with live connections");
    }
    // does the deletion, done this way because busBars do not change much over time so optimization is not important
    discontBarCount(m_busBars[i]->getType());
    for (size_t _i = i+1, n=m_busBars.size() ; _i < n ; _i++) {
        m_busBars[_i-1] = m_busBars[_i];
    }
    m_busBars.pop_back();
}


void PowerNet::cleanBranches() {
    m_branches.clear();
}

void PowerNet::clean() {
    m_branches.clear();
    m_busBars.clear();
    m_nPQ = 0;
    m_nPV = 0;
    m_nSlack = 0;
}

void PowerNet::setFlatStart() {
    for (size_t i = 0, n = m_busBars.size(); i < n; i++) {
        if (m_busBars[i]->getType() == Bar::Slack) {
            m_busBars[i]->setP(0);
            m_busBars[i]->setQ(0);
        } else if (m_busBars[i]->getType() == Bar::PV) {
            m_busBars[i]->setTeta(0);
            m_busBars[i]->setQ(0);
        } else {
            m_busBars[i]->setV(1);
            m_busBars[i]->setTeta(0);
        }
    }
}

size_t PowerNet::getNBars() const {
    return m_busBars.size();
}

size_t PowerNet::getNBranches() const {
    return m_branches.size();
}

size_t PowerNet::getNSlack() const {
    return m_nSlack;
}

size_t PowerNet::getNPQ() const {
    return m_nPQ;
}

size_t PowerNet::getNPV() const {
    return m_nPV;
}

size_t PowerNet::getBarIndex(const BarPtr bar) const {
    for (size_t i = 0, n = m_busBars.size(); i < n; i++) {
        if (m_busBars[i] == bar) {
            return i;
        }
    }
    throw std::runtime_error("Bar could not be found");
}

size_t PowerNet::getBarIndex(const size_t id) const {
    for (size_t i = 0, n = m_busBars.size(); i < n; i++) {
        if (m_busBars[i]->getId() == id) {
            return i;
        }
    }
    throw std::runtime_error("Bar could not be found");
}

size_t PowerNet::getBranchIndex(const BranchPtr branch) const {
    for (size_t i = 0, n = m_branches.size(); i < n; i++) {
        if (m_branches[i] == branch) {
            return i;
        }
    }
    throw std::runtime_error("Branch could not be found");
}

size_t PowerNet::getBranchIndex(uint64_t id) const {
    for (size_t i = 0, n = m_branches.size(); i < n ; i++) {
        uint64_t id1 = m_branches[i]->getId();
        if (id1 == id) {
            return i;
        } else if (id1 > id) {
            throw std::runtime_error("could not find branch");
        }
    }
    throw std::runtime_error("could not find branch");
}

size_t PowerNet::getBranchIndex(size_t kId, size_t mId) const {
    return getBranchIndex(Branch::calculateId(kId, mId));
}

size_t PowerNet::getBranchIndexByIndex(size_t kId, size_t mId) const {
    if (m_busBars.size() <= kId || m_busBars.size() <= mId) {
        throw std::runtime_error("bar does not exists");
    }
    return getBranchIndex(m_busBars[kId]->getId(), m_busBars[mId]->getId());
}

const BarPtr PowerNet::getBar(uint64_t i) const {
    return m_busBars[getBarIndex(i)];
}

const BarPtr PowerNet::getBarByIndex(size_t i) const {
    if (m_busBars.size() <= i) {
        throw std::runtime_error("bar does not exists");
    }
    return m_busBars[i];
}

const BranchPtr PowerNet::getBranch(uint64_t i) const {
    return m_branches[getBranchIndex(i)];
}

const BranchPtr PowerNet::getBranchByIndex(size_t i) const {
    if (m_branches.size() <= i) {
        throw std::runtime_error("branch does not exists");
    }
    return m_branches[i];
}

const BranchPtr PowerNet::getBranch(uint64_t kId, uint64_t mId) const {
    return m_branches[getBranchIndex(kId, mId)];
}

const BranchPtr PowerNet::getBranchByIndex(uint64_t kId, uint64_t mId) const {
    return m_branches[getBranchIndexByIndex(kId, mId)];
}

void PowerNet::updateY() {
    const size_t nBars = getNBars();
    m_Y.setZero(nBars, nBars);
    for (size_t i = 0; i < nBars; i++) {
        BarPtr bar = getBarByIndex(i);
        m_Y(i, i) = AdmitanceCalc::getAdmitanceKk(bar);
        for (size_t j = i+1 ; j < nBars ; j++) {
            try {
                BranchPtr branch = getBranchByIndex(i, j);
                if (branch->getK() == bar) {
                    m_Y(i, j) = AdmitanceCalc::getAdmitanceKm(branch);
                    m_Y(j, i) = AdmitanceCalc::getAdmitanceMk(branch);
                } else {
                    m_Y(i, j) = AdmitanceCalc::getAdmitanceMk(branch);
                    m_Y(j, i) = AdmitanceCalc::getAdmitanceKm(branch);
                }
            } catch(std::exception e) {
            }
        }
    }
}

const complexo PowerNet::getY(size_t i, size_t j) {
    if (checkChanged()) {
        updateY();
    }
    return m_Y(i, j);
}

void PowerNet::completeNetPowers() {
    for (int i = 0, n = m_nSlack; i < n; i++) {
        m_busBars[i]->setS(getSk(i));
    }
    for (int i = m_nSlack, n = m_nSlack + m_nPV; i < n; i++) {
        m_busBars[i]->setQ(getSk(i).imag());
    }
}

bool PowerNet::areConnected(uint64_t i, uint64_t j) {
    try {
        size_t id = getBranchIndex(i, j);
        return m_branches[id]->isBranchOn();
    } catch(std::exception e) {
        return false;
    }
}

bool PowerNet::areConnectedByIndex(size_t i, size_t j) {
    try {
        size_t id = getBranchIndexByIndex(i, j);
        return m_branches[id]->isBranchOn();
    } catch(std::exception e) {
        return false;
    }
}


void PowerNet::discontBarCount(Bar::barType type) {
    if (type == Bar::PQ) {
        --m_nPQ;
    } else if (type == Bar::PV) {
        --m_nPV;
    } else {
        --m_nSlack;
    }
}

void PowerNet::sortBackBranch() {
    const uint64_t id = m_branches[m_branches.size()-1]->getId();
    for (size_t i = m_branches.size()-1 ; i > 0 ; i--) {
        if (m_branches[i-1]->getId() >  id) {
            std::swap<BranchPtr>(m_branches[i], m_branches[i-1]);
        } else {
            return;
        }
    }
}

void PowerNet::sortBackBar() {
    const Bar::barType type = m_busBars[m_busBars.size()-1]->getType();
    for (size_t i = m_busBars.size()-1; i > 0; i--) {
        if (m_busBars[i-1]->getType() > type) {
            std::swap<BarPtr>(m_busBars[i], m_busBars[i-1]);
        } else {
            break;
        }
    }
}

bool PowerNet::checkChanged() {
    bool changed = false;
    for (size_t i = 0, m = m_busBars.size(); i < m; i++) {
        if (m_busBars[i]->isChanged()) {
            changed = true;
            m_busBars[i]->clearChanged();
        }
    }
    for (size_t i = 0, m = m_branches.size(); i < m; i++) {
        if (m_branches[i]->isChanged()) {
            changed = true;
            m_branches[i]->clearChanged();
        }
    }
    return changed;
}

complexo PowerNet::getSk(size_t bar) {
    complexo sum = 0;
    for (size_t i = 0, n = m_busBars.size() ; i < n ; i++) {
        sum += m_Y(bar, i)*m_busBars[i]->getE();
    }
    return complexConjugado(complexConjugado(m_busBars[bar]->getE())*sum);
}

double PowerNet::getTetaKm(uint64_t kId, uint64_t mId) const {
    return getTetaKm(getBarIndex(kId), getBarIndex(mId));
}

double PowerNet::getTetaKmByIndex(size_t kI, size_t mI) const {
    return m_busBars[kI]->getTeta() - m_busBars[mI]->getTeta();
}

bool PowerNet::checkBarIdExists(uint64_t id) {
    for (size_t i = 0, n = m_busBars.size(); i < n; i++) {
        if (m_busBars[i]->getId() == id) {
            return true;
        }
    }
    return false;
}
}  // namespace neuralFlux
