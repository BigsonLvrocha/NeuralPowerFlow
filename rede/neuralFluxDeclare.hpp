/**
 * @brief defines common classes for easy import in other files, avoid highly header dependencies
 * 
 * @file neuralFluxDeclare.hpp
 * @author Luiz victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */
#ifndef REDE_NEURALFLUXDECLARE_HPP_
#define REDE_NEURALFLUXDECLARE_HPP_

#include <boost/shared_ptr.hpp>

namespace neuralFlux {
class Bar;
class Branch;
class BarInfo;
class BranchInfo;
class PowerNet;
class BranchPowerCalc;
class BranchAdmitanceCalc;

typedef boost::shared_ptr<Bar> BarPtr;
typedef boost::shared_ptr<Branch> BranchPtr;
typedef boost::shared_ptr<PowerNet> PowerNetPtr;
}  // namespace neuralFlux

#endif  // REDE_NEURALFLUXDECLARE_HPP_
