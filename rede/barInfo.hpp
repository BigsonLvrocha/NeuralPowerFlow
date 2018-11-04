/**
 * @brief header file for information for a bar
 * 
 * @file barInfo.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright Luiz Victor Linhares Rocha
 */

#ifndef REDE_BARINFO_HPP_
#define REDE_BARINFO_HPP_

#include <cstdint>
#include "barra.hpp"


namespace neuralFlux {

// armazena as informações de uma barra
class BarInfo{
 public:
    // construtor padrão
    BarInfo(
        uint64_t id = 0,
        Bar::barType type = Bar::Slack,
        double v = 1,
        double teta = 0,
        double p = 0,
        double q = 0,
        double  bsh = 0.0);
    explicit BarInfo(const BarPtr& bar);

    // getters
    uint64_t getId() const;
    Bar::barType getType() const;
    double getV() const;
    double getTeta() const;
    double getP() const;
    double getQ() const;
    double getBsh() const;

    /// setters
    void setId(uint64_t id);
    void setType(Bar::barType type);
    void setV(double v);
    void setTeta(double teta);
    void setP(double p);
    void setQ(double q);
    void setBsh(double bsh);

 private:
    uint64_t m_id;
    Bar::barType m_type;
    double m_v;
    double m_teta;
    double m_p;
    double m_q;
    double m_bsh;
};

}  // namespace neuralFlux
#endif  // REDE_BARINFO_HPP_
