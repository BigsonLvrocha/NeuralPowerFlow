/**
 * @brief Implements methods for admitance calculation of power network
 * 
 * @file admitanceCalc.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018 Luiz Victor Linhares Rocha
 */

#include <iostream>
#include "barra.hpp"
#include "branch.hpp"
#include "powerNet.hpp"
#include "admitanceCalc.hpp"
#include "../util/complexutils.h"

namespace neuralFlux {

/**
 * @brief gets only on part of the sum of calculation of admitance for the same bar
 * 
 * @param branch the branch object
 * @return complexo result
 */
complexo AdmitanceCalc::getAdmitanceKkPartial(const BranchPtr& branch) {
    double modTrafo = complexModulo2(branch->getTrafo());
    if (branch->isBranchOn()) {
        return modTrafo*(branch->getYkm()+complexo(0, branch->getBshkm()));
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param branch 
 * @return complexo 
 */
complexo AdmitanceCalc::getAdmitanceMmPartial(const BranchPtr& branch) {
    if (branch->isBranchOn())
        return branch->getYkm()+complexo(0, branch->getBshkm());
    return 0;
}


complexo AdmitanceCalc::getAdmitanceKm(const BranchPtr& branch) {
    if (branch->isBranchOn())
        return -branch->getA()*branch->getYkm()*complexCis(-branch->getPhi());
    return 0;
}

complexo AdmitanceCalc::getAdmitanceMk(const BranchPtr& branch) {
    if (branch->isBranchOn())
        return -branch->getA()*branch->getYkm()*complexCis( branch->getPhi());
    return 0;
}

complexo AdmitanceCalc::getAdmitanceKk(const BarPtr& bar) {
    complexo admitance = complexo(0, bar->getBsh());
    for (size_t i = 0, n = bar->getNConnections(); i < n; i++) {
        if (bar->getConnection(i)->getK() == bar) {
            admitance += getAdmitanceKkPartial(bar->getConnection(i));
        } else {
            admitance += getAdmitanceMmPartial(bar->getConnection(i));
        }
    }
    return admitance;
}

Eigen::MatrixXcd AdmitanceCalc::getMatrix(const PowerNetPtr& net) {
    const size_t nBars = net->getNBars();
    Eigen::MatrixXcd Y;
    Y.setZero(nBars, nBars);
    for (size_t i = 0 ; i < nBars ; i++) {
        BarPtr bar = net->getBarByIndex(i);
        Y(i, i) = getAdmitanceKk(bar);
        for (size_t j = i+1 ; j < nBars ; j++) {
            try {
                BranchPtr branch = net->getBranchByIndex(i, j);
                if (branch->getK() == bar) {
                    Y(i, j) = getAdmitanceKm(branch);
                    Y(j, i) = getAdmitanceMk(branch);
                } else {
                    Y(i, j) = getAdmitanceMk(branch);
                    Y(j, i) = getAdmitanceKm(branch);
                }
            }
            catch (std::exception e) {
            }
        }
    }
    return Y;
}
}  // namespace neuralFlux
