/**
 * @brief implementation of barInfo
 * 
 * @file barInfo.cpp
 * @author your name
 * @date 2018-09-22
 * @copyright 2018
 */

#include "barInfo.hpp"

namespace neuralFlux {
//  construtor padrão
BarInfo::BarInfo(
    uint64_t id  ,
    Bar::barType type,
    double v,
    double teta,
    double p,
    double q,
    double bsh
):
    m_id(id),
    m_type(type),
    m_v(v),
    m_teta(teta),
    m_p(p),
    m_q(q),
    m_bsh(bsh)
{}


// construtor a partir de uma barra
BarInfo::BarInfo(const BarPtr& bar):
    m_id(bar->getId()),
    m_type(bar->getType()),
    m_v(bar->getV()),
    m_teta(bar->getTeta()),
    m_p(bar->getP()),
    m_q(bar->getQ()),
    m_bsh(bar->getBsh()
) {
}

// getters
uint64_t BarInfo::getId() const {
    return m_id;
}
Bar::barType BarInfo::getType() const {
    return m_type;
}
double BarInfo::getV() const {
    return m_v;
}
double BarInfo::getTeta() const {
    return m_teta;
}
double BarInfo::getP() const {
    return m_p;
}
double BarInfo::getQ() const {
    return m_q;
}
double BarInfo::getBsh() const {
    return m_bsh;
}

void BarInfo::setId(uint64_t id) {
    m_id = id;
}
void BarInfo::setType(Bar::barType type) {
    m_type = type;
}
void BarInfo::setV(double v) {
    m_v = v;
}
void BarInfo::setTeta(double teta) {
    m_teta = teta;
}
void BarInfo::setP(double p) {
    m_p = p;
}
void BarInfo::setQ(double q) {
    m_q = q;
}
void BarInfo::setBsh(double bsh) {
    m_bsh = bsh;
}
}  // namespace neuralFlux
