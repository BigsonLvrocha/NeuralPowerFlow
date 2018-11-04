/**
 * @brief 
 * 
 * @file barra.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_BARRA_HPP_
#define REDE_BARRA_HPP_

#define USE_MATH_DEFINES
#include <complex>
#include <cmath>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "neuralFluxDeclare.hpp"
#include "../util/complexutilsDeclare.h"
#include "../util/connection.hpp"
#include "../util/connector.hpp"
#include "../util/identifiable.hpp"
#include "../util/changeTrackable.hpp"

namespace neuralFlux {

class Bar : public ConnectionPoint<Branch>, public Identifiable, public ChangeTrackable {
 public:
    virtual ~Bar();
    enum barType{ Slack = 1, PV = 2 , PQ = 3 };
    // bar factories with standard values
    static const BarPtr newSlack(
        const double v,
        const double teta,
        const uint64_t id = 0,
        const double p = 0,
        const double q = 0,
        const double bsh = 0);

    static const BarPtr newPV(
        const double p,
        const double v,
        const uint64_t id = 0,
        const double teta = 0,
        const double q = 0,
        const double bsh = 0);

    static const BarPtr newPQ(
        const double p,
        const double q,
        const uint64_t id = 0,
        const double v = 1,
        const double teta = 0,
        const double bsh = 0);

    static const BarPtr newBar(
        barType type,
        uint64_t id,
        double v = 1,
        double teta = 0,
        double p = 0,
        double q = 0,
        double bsh = 0);

    ///* getters and setters*/
    const complexo getE() const;
    const complexo getS() const;
    double getP() const;
    double getQ() const;
    double getV() const;
    double getTeta() const;
    barType getType() const;
    double getBsh() const;

    void setE(const complexo e);
    void setS(const complexo s);
    void setP(const double p);
    void setQ(const double q);
    void setV(const double v);
    void setTeta(const double teta);
    void setBsh(const double bsh);
    void setType(barType type);

 private:
    complexo m_e;
    complexo m_s;
    double m_bsh;
    barType m_type;
    Bar(const barType type,
        const double v,
        const double teta,
        const double p,
        const double q,
        const uint64_t id = 0,
        const double bsh = 0);
};
}  // namespace neuralFlux


#endif  // REDE_BARRA_HPP_
