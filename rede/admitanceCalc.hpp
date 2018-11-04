/**
 * @brief header file for admitance calculation class
 * 
 * @file admitanceCalc.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018 Luiz Victor Linhares Rocha
 */

#ifndef REDE_ADMITANCECALC_HPP_
#define REDE_ADMITANCECALC_HPP_

#include <complex>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "../libraries/Eigen/Eigen"
#include "neuralFluxDeclare.hpp"

namespace neuralFlux {
/**
*   @brief class to calculate branch's admitance factor in a network admitance matrix
*/
class AdmitanceCalc : boost::noncopyable {
 public:
    static complexo getAdmitanceKkPartial(const BranchPtr& branch);
    static complexo getAdmitanceMmPartial(const BranchPtr& branch);
    static complexo getAdmitanceKm(const BranchPtr& branch);
    static complexo getAdmitanceMk(const BranchPtr& branch);
    static complexo getAdmitanceKk(const BarPtr& bar);
    static Eigen::MatrixXcd getMatrix(const PowerNetPtr& net);
    static Eigen::MatrixXcd getMatrix(const PowerNet * const net);
};
}  // namespace neuralFlux
#endif  // REDE_ADMITANCECALC_HPP_
