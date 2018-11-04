/**
 * @brief defines newton raphman solver
 * 
 * @file newtonSolver.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_NEWTONSOLVER_HPP_
#define REDE_NEWTONSOLVER_HPP_

#include <vector>
#include "../libraries/Eigen/Eigen"
#include "neuralFluxDeclare.hpp"
#include "powerFlowSolver.hpp"

namespace neuralFlux {

class NewtonSolver : public PowerFlowSolver{
 public:
    // default constructor
    NewtonSolver();
    // constructor with defined net
    explicit NewtonSolver(PowerNetPtr net);
    // virtual destructor, as it's inherited
    virtual ~NewtonSolver();
    // jacobiana getter, can be recalculated if flag = true
    const Eigen::MatrixXd& getJacobian(const bool calculate = true);
    // delta getter, can be recalculated if flag = true
    const Eigen::VectorXd& getDeltas(const bool calculate = true);

 protected:
    Eigen::MatrixXd m_jacobian;
    Eigen::VectorXd m_deltas;
    Eigen::VectorXd m_correction;
    virtual void solve();
    const Eigen::MatrixXd& calculateJacobian();
    const Eigen::VectorXd& calculateDeltas();
    void applyCorrection();
    double getTotalError();
    void cacheBars();

 private:
    // bars subscript cache
    std::vector<size_t> m_PQBars;
    std::vector<size_t> m_PVBars;
    size_t m_nPQ;
    size_t m_nPV;

    // calulations of indiviual part of matrix
    double getH(const size_t i, const size_t j);
    double getL(const size_t i, const size_t j);
    double getM(const size_t i, const size_t j);
    double getN(const size_t i, const size_t j);


    // calculates the sum of Vm*(Gkm*cos(tetakm)+Bkm*sin(TetaKm)), where k = i and m are the bars connected to the bar i
    double getSum(const size_t i);
    // calculates the sum of Vm*(Gkm*sin(tetakm)-Bkm*cos(TetaKm)), where k = i and m are the bars connected to the bar i
    double getSumConjugate(const size_t _i);
    // calulate active power error of bar i
    double getDeltaP(const size_t i);
    // calulate reactive power error of bar i
    double getDeltaQ(const size_t i);
    // calculates submatrix H and puts into jacobian
    void calculateH();
    // calculates submatrix M and puts into jacobian
    void calculateM();
    // calculates submatrix N and puts into jacobian
    void calculateN();
    // calculates submatrix L and puts into jacobian
    void calculateL();
    // do a net step
    void step();
};
}  // namespace neuralFlux
#endif  // REDE_NEWTONSOLVER_HPP_
