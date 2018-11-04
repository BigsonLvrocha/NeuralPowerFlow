/**
 * @brief defines branch class
 * 
 * @file branch.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_BRANCH_HPP_
#define REDE_BRANCH_HPP_

#include <complex>
#include <boost/shared_ptr.hpp>
#include "neuralFluxDeclare.hpp"
#include "../util/complexutilsDeclare.h"
#include "../util/connection.hpp"
#include "../util/connector.hpp"
#include "../util/identifiable.hpp"
#include "../util/changeTrackable.hpp"


namespace neuralFlux {

class Branch : public Connection<Bar>, public ChangeTrackable {
 public:
    virtual ~Branch();
    static BranchPtr connectBranch(
        boost::shared_ptr<Bar> barK,
        boost::shared_ptr<Bar> barM,
        const double rkm,
        const double xkm,
        const double bshkm = 0,
        const double a = 1,
        const double phi = 0);

    const BarPtr getK() const;
    const BarPtr getM() const;
    double getTetaKm() const;

    // Getters
    const complexo getZkm() const;
    double getRkm() const;
    double getXkm() const;


    complexo getYkm() const;
    double getGkm() const;
    double getBkm() const;
    double getBshkm() const;

    const complexo getTrafo() const;
    double getA() const;
    double getPhi() const;

    // setters, set changed
    void setZkm(const complexo zkm);
    void setRkm(const double rkm);
    void setXkm(const double xkm);

    void setYkm(const complexo zkm);
    void setGkm(const double rkm);
    void setBkm(const double xkm);
    void setBshkm(const double bshkm);

    void setTrafo(const complexo trafo);
    void setA(const double a);
    void setPhi(const double phi);

    // calculators
    complexo getIkm();
    complexo getImk();

    uint64_t getId() const;
    static uint64_t calculateId(uint64_t kId, uint64_t mId);
    // operator, branch order defined by the bar's ids, being k the mos significant
    bool operator<(const Branch& branch) const;
    bool operator>(const Branch& branch) const;
    bool operator==(const Branch& branch) const;

    void setBranchOn(bool branchOn = true);
    bool isBranchOn();

 private:
    complexo m_zkm;   // impedancia da linha
    complexo m_tap;  // dados de possivel trafo
    double m_bshkm;
    bool m_branchOn;
    const double tolerance = 0.000001;

    Branch(
        const double rkm,
        const double xkm,
        const double bshkm = 0,
        const double a = 1,
        const double phi = 0);
};
}  // namespace neuralFlux

#endif  // REDE_BRANCH_HPP_
