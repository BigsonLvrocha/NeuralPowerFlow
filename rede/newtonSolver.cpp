/**
 * @brief defines newton raphman power flow solver
 * 
 * @file newtonSolver.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <cmath>
#include "newtonSolver.hpp"
#include "barra.hpp"
#include "branch.hpp"
#include "powerNet.hpp"


namespace neuralFlux {

NewtonSolver::NewtonSolver():PowerFlowSolver() {
}

NewtonSolver::NewtonSolver(PowerNetPtr net):
    PowerFlowSolver(net) {
}

NewtonSolver::~NewtonSolver() {
}

const Eigen::MatrixXd& NewtonSolver::getJacobian(const bool calculate) {
    if (calculate)
        return calculateJacobian();
    return m_jacobian;
}

const Eigen::VectorXd& NewtonSolver::getDeltas(const bool calculate) {
    if (calculate)
        return calculateDeltas();
    return m_deltas;
}

void NewtonSolver::solve() {
    cacheBars();
    calculateJacobian();
    calculateDeltas();
    while (!reachedMaxIt() && !reachMinError(getTotalError())) {
        step();
        calculateJacobian();
        calculateDeltas();
        addIt();
    }
}

const Eigen::MatrixXd& NewtonSolver::calculateJacobian() {
    calculateH();
    calculateM();
    calculateN();
    calculateL();
    return m_jacobian;
}

const Eigen::VectorXd& NewtonSolver::calculateDeltas() {
    for (size_t i = 0 ; i < m_nPQ ; i++) {
        m_deltas(i) = getDeltaP(m_PQBars[i]);
        m_deltas(i+m_nPQ+m_nPV) = getDeltaQ(m_PQBars[i]);
    }

    for (size_t i = 0 ; i < m_nPV ; i++) {
        m_deltas(i+m_nPQ) = getDeltaP(m_PVBars[i]);
    }

    return m_deltas;
}

void NewtonSolver::applyCorrection() {
    for (size_t i = 0 ; i < m_nPQ ; i++) {
        BarPtr bar = getNet()->getBarByIndex(m_PQBars[i]);
        bar->setTeta(bar->getTeta() + m_correction(i));
        bar->setV(bar->getV() + m_correction(i+m_nPQ+m_nPV));
    }
    for (size_t i = 0 ; i < m_nPV ; i++) {
        BarPtr bar = getNet()->getBarByIndex(m_PVBars[i]);
        bar->setTeta(bar->getTeta() + m_correction(i+m_nPQ));
    }
}

double NewtonSolver::getTotalError() {
    double erroMax = 0;
    for (size_t i = 0, n = m_deltas.rows() ; i < n; i++) {
        if (std::fabs(m_deltas[i]) > erroMax)
            erroMax = std::fabs(m_deltas[i]);
    }
    return erroMax;
}

void NewtonSolver::cacheBars() {
    PowerNetPtr net = getNet();
    size_t nBars = net->getNBars();
    for (size_t i = 0 ; i < nBars ; i++) {
        if (net->getBarByIndex(i)->getType() == Bar::PQ) {
            m_PQBars.push_back(i);
        } else if (net->getBarByIndex(i)->getType() == Bar::PV) {
            m_PVBars.push_back(i);
        }
    }
    m_nPQ = m_PQBars.size();
    m_nPV = m_PVBars.size();
    m_jacobian.setZero(2*m_nPQ+m_nPV, 2*m_nPQ+m_nPV);
    m_deltas.setZero(2*m_nPQ+m_nPV);
    m_correction.setZero(2*m_nPQ+m_nPV);
}

double NewtonSolver::getH(const size_t i, const size_t j) {
    PowerNetPtr net = getNet();
    if (i != j) {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Vm = net->getBarByIndex(j)->getV();
        const double Gkm = net->getY(i, j).real();
        const double Bkm = net->getY(i, j).imag();
        const double sinkm = sin(net->getTetaKmByIndex(i, j));
        const double coskm = cos(net->getTetaKmByIndex(i, j));
        return Vk*Vm*(Gkm*sinkm-Bkm*coskm);
    } else {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Bkk = net->getY(i, i).imag();
        const double soma = getSumConjugate(i);
        return -Vk*Vk*Bkk - Vk*soma;
    }
}

double NewtonSolver::getL(const size_t i, const  size_t j) {
    PowerNetPtr net = getNet();
    if (i != j) {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Gkm = net->getY(i, j).real();
        const double Bkm = net->getY(i, j).imag();
        const double sinkm = sin(net->getTetaKmByIndex(i, j));
        const double coskm = cos(net->getTetaKmByIndex(i, j));
        return Vk*(Gkm*sinkm-Bkm*coskm);
    } else {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Bkk = net->getY(i, i).imag();
        const double soma = getSumConjugate(i);
        return -2*Vk*Bkk + soma;
    }
}

double NewtonSolver::getM(const size_t i, const size_t j) {
    PowerNetPtr net = getNet();
    if (i != j) {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Vm = net->getBarByIndex(j)->getV();
        const double Gkm = net->getY(i, j).real();
        const double Bkm = net->getY(i, j).imag();
        const double sinkm = sin(net->getTetaKmByIndex(i, j));
        const double coskm = cos(net->getTetaKmByIndex(i, j));
        return -Vk*Vm*(Gkm*coskm+Bkm*sinkm);
    } else {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Gkk = net->getY(i, i).real();
        const double soma = getSum(i);
        return -(Vk*Vk)*Gkk+Vk*soma;
    }
}

double NewtonSolver::getN(const size_t i, const size_t j) {
    PowerNetPtr net = getNet();
    if (i != j) {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Gkm = net->getY(i, j).real();
        const double Bkm = net->getY(i, j).imag();
        const double sinkm = sin(net->getTetaKmByIndex(i, j));
        const double coskm = cos(net->getTetaKmByIndex(i, j));
        return Vk*(Gkm*coskm+Bkm*sinkm);
    } else {
        const double Vk = net->getBarByIndex(i)->getV();
        const double Gkk = net->getY(i, i).real();
        const double soma = getSum(i);
        return Vk*Gkk + soma;
    }
}

double NewtonSolver::getSum(const size_t i) {
    double sum = 0;
    PowerNetPtr net = getNet();
    for (size_t j = 0, n = net->getNBars() ; j < n ; j++) {
        if (i == j || net->areConnectedByIndex(i, j)) {
            const double vm = net->getBarByIndex(j)->getV();
            const double gkm = net->getY(i, j).real();
            const double bkm = net->getY(i, j).imag();
            const double sinkm = sin(net->getTetaKmByIndex(i, j));
            const double coskm = cos(net->getTetaKmByIndex(i, j));
            sum += vm*(gkm*coskm+bkm*sinkm);
        }
    }
    return sum;
}

double NewtonSolver::getSumConjugate(const size_t i) {
    double sum = 0;
    PowerNetPtr net = getNet();
    for (size_t j = 0 , n = net->getNBars() ; j < n ; j++) {
        if (i == j || net->areConnectedByIndex(i, j)) {
            const double vm = net->getBarByIndex(j)->getV();
            const double gkm = net->getY(i, j).real();
            const double bkm = net->getY(i, j).imag();
            const double sinkm = sin(net->getTetaKmByIndex(i, j));
            const double coskm = cos(net->getTetaKmByIndex(i, j));
            sum += vm*(gkm*sinkm-bkm*coskm);
        }
    }
    return sum;
}

double NewtonSolver::getDeltaP(const size_t i) {
    BarPtr bar = getNet()->getBarByIndex(i);
    return bar->getP()-bar->getV()*getSum(i);
}

double NewtonSolver::getDeltaQ(const size_t i) {
    BarPtr bar = getNet()->getBarByIndex(i);
    return bar->getQ()-bar->getV()*getSumConjugate(i);
}

void NewtonSolver::calculateH() {
    for (size_t i = 0; i < m_nPQ ; i++) {
        // primeiro, adiciona as barras pq em relação as correções de pq
        for (size_t j = 0 ; j < m_nPQ ; j++) {
            m_jacobian(i, j) = getH(m_PQBars[i], m_PQBars[j]);
        }
        // barras pq em relação a correções de pv
        for (size_t j = 0 ; j < m_nPV ; j++) {
            m_jacobian(i, j+m_nPQ) = getH(m_PQBars[i], m_PVBars[j]);
        }
    }
    for (size_t i = 0 ; i < m_nPV ; i++) {
        // barras pv em relação a correções de pq
        for (size_t j = 0 ; j < m_nPQ ; j++) {
            m_jacobian(i+m_nPQ, j) = getH(m_PVBars[i], m_PQBars[j]);
        }
        // barras pv em relação a correções de pv
        for (size_t j = 0 ; j < m_nPV ; j++) {
            m_jacobian(i+m_nPQ, j+m_nPQ) = getH(m_PVBars[i], m_PVBars[j]);
        }
    }
}

void NewtonSolver::calculateL() {
    // apenas pq com relação de correções de pq
    for (size_t i = 0 ; i < m_nPQ ; i++) {
        for (size_t j = 0; j < m_nPQ; j++) {
            m_jacobian(i + m_nPQ + m_nPV, j + m_nPQ + m_nPV) = getL(m_PQBars[i], m_PQBars[j]);
        }
    }
}

void NewtonSolver::calculateM() {
    for (size_t i = 0 ; i < m_nPQ ; i++) {
        // barras pq em relação a correções de pq
        for (size_t j = 0 ; j < m_nPQ ; j++) {
            m_jacobian(i+m_nPQ+m_nPV, j) = getM(m_PQBars[i], m_PQBars[j]);
        }
        // barras pv em relação a correções de pq
        for (size_t j = 0 ; j < m_nPV ; j++) {
            m_jacobian(i + m_nPQ + m_nPV, j + m_nPQ) = getM(m_PQBars[i], m_PVBars[j]);
        }
    }
}

void NewtonSolver::calculateN() {
    // pq com relação a correções de PQ
    for (size_t i = 0 ; i < m_nPQ ; i++) {
        for (size_t j = 0 ; j < m_nPQ ; j++) {
            m_jacobian(i, j + m_nPQ + m_nPV) = getN(m_PQBars[i], m_PQBars[j]);
        }
    }
    // pv com relação a correções de PQ
    for (size_t i = 0; i < m_nPV; i++) {
        for (size_t j = 0 ; j < m_nPQ ; j++) {
            m_jacobian(i + m_nPQ, j + m_nPQ + m_nPV) = getN(m_PVBars[i], m_PQBars[j]);
        }
    }
}

void NewtonSolver::step() {
    m_correction = m_jacobian.colPivHouseholderQr().solve(m_deltas);
    applyCorrection();
}
}  // namespace neuralFlux
