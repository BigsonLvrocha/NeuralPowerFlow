/**
 * @brief definition of a gaussiedel powerflow solver
 * 
 * @file gausSiedelSolver.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_GAUSSIEDELSOLVER_HPP_
#define REDE_GAUSSIEDELSOLVER_HPP_

#include "neuralFluxDeclare.hpp"
#include "../util/complexutilsDeclare.h"
#include "powerFlowSolver.hpp"

namespace neuralFlux {

class GausSeidelSolver : public PowerFlowSolver{
 public:
    GausSeidelSolver();
    explicit GausSeidelSolver(PowerNetPtr net);
    virtual ~GausSeidelSolver();
 protected:
    virtual void solve();
    double stepPQ(size_t iBar);
    double stepPV(size_t iBar);
    double stepNet();
 private:
    complexo getE(size_t iBar);
};
}  // namespace neuralFlux
#endif  // REDE_GAUSSIEDELSOLVER_HPP_
