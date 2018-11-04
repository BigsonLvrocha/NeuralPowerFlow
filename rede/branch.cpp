/**
 * @brief implements branch class type
 * 
 * @file branch.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <cmath>
#include <iostream>
#include "../util/complexutils.h"
#include "barra.hpp"
#include "branch.hpp"

namespace neuralFlux {


BranchPtr Branch::connectBranch(
    boost::shared_ptr<Bar> barK,
    boost::shared_ptr<Bar> barM,
    const double rkm,
    const double xkm,
    const double bshkm,
    const double a,
    const double phi
) {
    BranchPtr branch(new Branch(rkm, xkm, bshkm, a, phi));
    Connector<Bar, Branch>::makeConnection(barK, branch);
    Connector<Bar, Branch>::makeConnection(barM, branch);
    return branch;
}

Branch::Branch(
    const double rkm,
    const double xkm,
    const double bshkm,
    const double a,
    const double phi):
    m_zkm(complexo(rkm, xkm)),
    m_tap(complexPolar(a, phi)),
    m_bshkm(bshkm),
    m_branchOn(true) {
}

Branch::~Branch() {
}

const BarPtr Branch::getK() const {
    return getConnection(0);
}

const BarPtr Branch::getM() const {
    return getConnection(1);
}

double Branch::getTetaKm() const {
    return getK()->getTeta() - getM()->getTeta();
}

const complexo Branch::getZkm() const {
    return m_zkm;
}

double Branch::getRkm() const {
    return m_zkm.real();
}

double Branch::getXkm() const {
    return m_zkm.imag();
}

complexo Branch::getYkm() const {
    return complexo(getGkm(), getBkm());
}

double Branch::getGkm() const {
    return getRkm()/(getRkm()*getRkm() + getXkm()*getXkm());
}

double Branch::getBkm() const {
    return -getXkm()/(getRkm()*getRkm()+getXkm()*getXkm());
}

double Branch::getBshkm() const {
    return m_bshkm;
}

const complexo Branch::getTrafo() const {
    return m_tap;
}

double Branch::getA() const {
    return std::sqrt(complexModulo2(m_tap));
}


double Branch::getPhi() const {
    return complexFase(m_tap);
}

void Branch::setZkm(const complexo zkm) {
    if (std::abs(zkm-m_zkm) > tolerance) {
        m_zkm = zkm;
        setChanged();
    }
}
void Branch::setRkm(const double rkm) {
    setZkm(complexo(rkm, m_zkm.imag()));
}
void Branch::setXkm(const double xkm) {
    setZkm(complexo(m_zkm.real(), xkm));
}

void Branch::setYkm(const complexo ykm) {
    const double gkm = ykm.real();
    const double bkm = ykm.imag();

    setZkm(complexo(gkm/(gkm*gkm+bkm*bkm), -bkm/(gkm*gkm+bkm*bkm)));
}

void Branch::setGkm(const double gkm) {
    setYkm(complexo(gkm, getBkm()));
}

void Branch::setBkm(const double bkm) {
    setYkm(complexo(getGkm(), bkm));
}

void Branch::setBshkm(const double bshkm) {
    if (std::abs(m_bshkm - bshkm) > tolerance) {
        m_bshkm = bshkm;
        setChanged();
    }
}

void Branch::setTrafo(const complexo trafo) {
    if (std::abs(trafo - m_tap) > tolerance) {
        m_tap = trafo;
        setChanged();
    }
}

void Branch::setA(const double a) {
    setTrafo(complexPolar(a, complexFase(m_tap)));
}

void Branch::setPhi(const double phi) {
    setTrafo(complexPolar(sqrt(complexModulo2(m_tap)), phi));
}

uint64_t Branch::getId() const {
    return Branch::calculateId( getK()->getId(), getM()->getId() );
}

uint64_t Branch::calculateId(uint64_t kId, uint64_t mId) {
    if (kId < mId) {
        return (kId << (sizeof(uint64_t)*4)) | (mId);
    }
    return (mId << (sizeof(uint64_t)*4)) | (kId);
}

bool Branch::operator < (const Branch& branch) const {
    return this->getId() < branch.getId();
}
bool Branch::operator > (const Branch& branch) const {
    return this->getId() > branch.getId();
}

bool Branch::operator == (const Branch& branch) const {
    return this->getK() == branch.getM();
}

void Branch::setBranchOn(bool branchOn) {
    if (m_branchOn != branchOn) {
        setChanged();
        m_branchOn = branchOn;
    }
}

bool Branch::isBranchOn() {
    return m_branchOn;
}

complexo Branch::getIkm() {
    const double T = complexModulo2(getTrafo());
    return (T*this->getYkm() + complexo(0, this->getBshkm())) * this->getK()->getE()
        - (complexConjugado(this->getTrafo()) * this->getYkm()) * this->getM()->getE();
}

complexo Branch::getImk() {
    return (-this->getTrafo() * this->getYkm()) * this->getK()->getE()
        + (this->getYkm() + complexo(0, this->getBshkm())) * this->getM()->getE();
}
}  // namespace neuralFlux
