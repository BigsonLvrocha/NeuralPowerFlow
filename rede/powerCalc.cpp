/**
 * @brief implementation of class for calculating power flow
 * 
 * @file powerCalc.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include "barra.hpp"
#include "branch.hpp"
#include "powerCalc.hpp"
#include "admitanceCalc.hpp"
#include "../util/complexutils.h"

namespace neuralFlux {

complexo PowerCalc::getSkm(const BranchPtr& branch) {
    // equação S* = Ek*Ikm
    const complexo Ykm = branch->getYkm();
    const complexo Ek = branch->getK()->getE();
    const complexo Em = branch->getM()->getE();
    const complexo t = branch->getTrafo();

    const complexo S = complexConjugado(Ek)*
        ((complexModulo2(t)*Ykm+complexo(0, branch->getBshkm()))*Ek-(complexConjugado(t)*Ykm)*Em);
    return complexConjugado(S);
}

double PowerCalc::getPkm(const BranchPtr& branch) {
    return getSkm(branch).real();
}

double PowerCalc::getQkm(const BranchPtr& branch) {
    return getSkm(branch).imag();
}

complexo PowerCalc::getSmk(const BranchPtr& branch) {
    return complexo(getPmk(branch), getQmk(branch));
}


double PowerCalc::getPmk(const BranchPtr& branch) {
    const double gkm = branch->getGkm();
    const double bkm = branch->getBkm();
    const double vk = branch->getK()->getV();
    const double vm = branch->getM()->getV();
    const double a = branch->getA();
    const double phi = branch->getPhi();
    return vm*vm*gkm - (a*vk)*vm*(gkm*cos(branch->getTetaKm() + phi) - bkm*sin(branch->getTetaKm()+phi));
}

double PowerCalc::getQmk(const BranchPtr& branch) {
    const double gkm = branch->getGkm();
    const double bkm = branch->getBkm();
    const double vm = branch->getM()->getV();
    const double vk = branch->getK()->getV();
    const double a = branch->getA();
    const double phi = branch->getPhi();
    const double bshkm = branch->getBshkm();
    return -vm*vm*(bkm+bshkm)+(a*vk)*vm*(gkm*sin(branch->getTetaKm()+phi)+bkm*cos(branch->getTetaKm()+phi));
}

complexo PowerCalc::getLoss(const BranchPtr& branch) {
    return complexo(getActiveLoss(branch), getReactiveLoss(branch));
}

double PowerCalc::getActiveLoss(const BranchPtr& branch) {
    return getPkm(branch) + getPmk(branch);
}

double PowerCalc::getReactiveLoss(const BranchPtr& branch) {
    return getQkm(branch)+getQmk(branch);
}


complexo PowerCalc::getSk(const BarPtr& bar) {
    complexo sum = 0;
    for (size_t i = 0, n = bar->getNConnections(); i < n; i++) {
        if (bar->getConnection(i)->getK() == bar) {
            sum += AdmitanceCalc::getAdmitanceKm(bar->getConnection(i))*bar->getConnection(i)->getM()->getE();
        } else {
            sum += AdmitanceCalc::getAdmitanceMk(bar->getConnection(i))*bar->getConnection(i)->getK()->getE();
        }
    }
    return complexConjugado(complexConjugado(bar->getE())*(sum+AdmitanceCalc::getAdmitanceKk(bar)*bar->getE()));
}

double PowerCalc::getPk(const BarPtr& bar) {
    return getSk(bar).real();
}

double PowerCalc::getQk(const BarPtr& bar) {
    return getSk(bar).imag();
}
}  // namespace neuralFlux
