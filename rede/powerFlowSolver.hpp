/**
 * @brief definition of superclass for power flow resolution
 * 
 * @file powerFlowSolver.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_POWERFLOWSOLVER_HPP_
#define REDE_POWERFLOWSOLVER_HPP_

#include <stdexcept>
#include "neuralFluxDeclare.hpp"
#include "powerNetValidator.hpp"


namespace neuralFlux {

class PowerFlowSolver {
 public:
    explicit PowerFlowSolver(PowerNetPtr net);
    PowerFlowSolver();
    virtual ~PowerFlowSolver();

    size_t solveNet(
        const bool reset = false,
        double error = 0.001,
        unsigned int maxIt = 50000);

    size_t setNetAndSolve(
        PowerNetPtr net,
        const bool reset = false,
        double error = 0.001,
        unsigned int maxIt = 50000);
    void setNet(PowerNetPtr net);
    PowerNetPtr getNet();

 protected:
    virtual void solve() = 0;
    void addIt();
    unsigned int getIt();
    bool reachedMaxIt();
    bool reachMinError(const double error);

 private:
    PowerNetPtr m_net;
    PowerNetValidator m_validator;
    unsigned int m_maxIt;
    unsigned int m_it;
    double m_minError;
    void completeBars();
    void checkNet();
};
}  // namespace neuralFlux
#endif  // REDE_POWERFLOWSOLVER_HPP_
