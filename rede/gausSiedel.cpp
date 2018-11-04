/**
 * @brief implementation of gaussiedel resolver
 * 
 * @file gausSiedel.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <cmath>
#include "gausSiedelSolver.hpp"
#include "barra.hpp"
#include "branch.hpp"
#include "powerNet.hpp"
#include "admitanceCalc.hpp"
#include "powerCalc.hpp"
#include "../util/complexutils.h"

namespace neuralFlux {


GausSeidelSolver::GausSeidelSolver():PowerFlowSolver() {}
GausSeidelSolver::GausSeidelSolver(PowerNetPtr net): PowerFlowSolver(net) {}
GausSeidelSolver::~GausSeidelSolver() {}

void GausSeidelSolver::solve() {
    double error = 0;
    do {
        error = stepNet();
        addIt();
    } while (!reachedMaxIt() && !reachMinError(error));
}

double GausSeidelSolver::stepPQ(size_t iBar) {
    PowerNetPtr net = getNet();
    BarPtr bar = getNet()->getBarByIndex(iBar);
    double oldV = bar->getV();
    double oldTeta = bar->getTeta();
    bar->setE(getE(iBar));
    return std::fabs(oldV - bar->getV()) + std::fabs(oldTeta - bar->getTeta());
}

double GausSeidelSolver::stepPV(size_t iBar) {
    PowerNetPtr net = getNet();
    BarPtr bar = net->getBarByIndex(iBar);
    double oldTeta = bar->getTeta();
    bar->setQ(net->getSk(iBar).imag());
    complexo E = getE(iBar);
    bar->setTeta(complexFase(E));
    return std::fabs(oldTeta-bar->getTeta());
}

complexo GausSeidelSolver::getE(size_t iBar) {
    PowerNetPtr net = getNet();
    BarPtr bar = net->getBarByIndex(iBar);
    complexo soma = complexConjugado(bar->getS())/complexConjugado(bar->getE());
    for (size_t i=0, n=iBar ; i < n ; i++) {
        soma -= net->getY(iBar, i)*net->getBarByIndex(i)->getE();
    }
    for (size_t i = iBar+1, n = getNet()->getNBars(); i < n; i++) {
        soma -= net->getY(iBar, i)*net->getBarByIndex(i)->getE();
    }
    return soma/net->getY(iBar, iBar);
}

double GausSeidelSolver::stepNet() {
    double error = 0;

    for (size_t i = 0, n = getNet()->getNBars() ; i < n ; i++) {
        Bar::barType type = getNet()->getBarByIndex(i)->getType();
        if (type == Bar::PQ) {
            error += stepPQ(i);
        } else if (type == Bar::PV) {
            error += stepPV(i);
        }
    }
    return error;
}
}  // namespace neuralFlux
