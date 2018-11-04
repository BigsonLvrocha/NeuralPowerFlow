/**
 * @brief implementation of branch info class
 * 
 * @file branchInfo.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include "branchInfo.hpp"
#include "barra.hpp"
#include "branch.hpp"

namespace neuralFlux {

BranchInfo::BranchInfo(
    uint64_t idk,
    uint64_t idm,
    double rkm,
    double xkm,
    double bshkm,
    double a,
    double phi,
    bool on
):
    m_idk(idk),
    m_idm(idm),
    m_rkm(rkm),
    m_xkm(xkm),
    m_bshkm(bshkm),
    m_a(a),
    m_phi(phi),
    m_on(on) {
}

BranchInfo::BranchInfo(const BranchPtr& branch):
    m_idk(branch->getK()->getId()),
    m_idm(branch->getM()->getId()),
    m_rkm(branch->getRkm()),
    m_xkm(branch->getXkm()),
    m_bshkm(branch->getBshkm()),
    m_a(branch->getA()),
    m_phi(branch->getPhi()),
    m_on(branch->isBranchOn()) {
}

uint64_t BranchInfo::getIdk() const {
    return m_idk;
}

uint64_t BranchInfo::getIdm() const {
    return m_idm;
}

uint64_t BranchInfo::getId() const {
    return Branch::calculateId(m_idk, m_idm);
}

double BranchInfo::getRkm() const {
    return m_rkm;
}

double BranchInfo::getXkm() const {
    return m_xkm;
}

double BranchInfo::getBshkm() const {
    return m_bshkm;
}

double BranchInfo::getA() const {
    return m_a;
}

double BranchInfo::getPhi() const {
    return m_phi;
}

bool BranchInfo::isBranchOn() const {
    return m_on;
}

void BranchInfo::setIdk(uint64_t idk) {
    m_idk = idk;
}

void BranchInfo::setIdm(uint64_t idm) {
    m_idm = idm;
}

void BranchInfo::setRkm(double rkm) {
    m_rkm = rkm;
}

void BranchInfo::setXkm(double xkm) {
    m_xkm = xkm;
}

void BranchInfo::setBshkm(double bshkm) {
    m_bshkm = bshkm;
}
void BranchInfo::setA(double a) {
    m_a = a;
}
void BranchInfo::setPhi(double phi) {
    m_phi = phi;
}

void BranchInfo::setBranchOn(bool on) {
    m_on = on;
}
}  // namespace neuralFlux
