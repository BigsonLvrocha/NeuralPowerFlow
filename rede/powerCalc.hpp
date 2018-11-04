/**
 * @brief defines class for calculating powerFlow
 * 
 * @file powerCalc.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_POWERCALC_HPP_
#define REDE_POWERCALC_HPP_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "neuralFluxDeclare.hpp"
#include "../util/complexutilsDeclare.h"

namespace neuralFlux {
/**
*   Class to calculate power flow and power loss in branches
*   Static class, no instance can be made
*/
class PowerCalc : boost::noncopyable {
 public:
    static complexo getSkm(const BranchPtr& branch);
    static double getPkm(const BranchPtr& branch);
    static double getQkm(const BranchPtr& branch);
    static complexo getSmk(const BranchPtr& branch);
    static double getPmk(const BranchPtr& branch);
    static double getQmk(const BranchPtr& branch);
    static complexo getLoss(const BranchPtr& branch);
    static double getActiveLoss(const BranchPtr& branch);
    static double getReactiveLoss(const BranchPtr& branch);
    static complexo getSk(const BarPtr& bar);
    static double getPk(const BarPtr& bar);
    static double getQk(const BarPtr& bar);
};
}  // namespace neuralFlux
#endif  // REDE_POWERCALC_HPP_
