/**
 * @brief definition of branchinfo class
 * 
 * @file branchInfo.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_BRANCHINFO_HPP_
#define REDE_BRANCHINFO_HPP_

#include "neuralFluxDeclare.hpp"

namespace neuralFlux {
class BranchInfo{
 public:
    BranchInfo(
        uint64_t idk,
        uint64_t idm,
        double rkm,
        double xkm,
        double bshkm = 0,
        double a = 0,
        double phi = 0,
        bool on = true);
    explicit BranchInfo(const BranchPtr& branch);
    uint64_t getIdk() const;
    uint64_t getIdm() const;
    uint64_t getId() const;
    double getRkm() const;
    double getXkm() const;
    double getBshkm() const;
    double getA() const;
    double getPhi() const;
    bool isBranchOn() const;
    void setIdk(uint64_t idk);
    void setIdm(uint64_t idm);
    void setRkm(double rkm);
    void setXkm(double xkm);
    void setBshkm(double bshkm);
    void setA(double a);
    void setPhi(double phi);
    void setBranchOn(bool on);

 private:
    uint64_t m_idk;
    uint64_t m_idm;
    double m_rkm;
    double m_xkm;
    double m_bshkm;
    double m_a;
    double m_phi;
    bool m_on;
};
}  // namespace neuralFlux
#endif  // REDE_BRANCHINFO_HPP_
