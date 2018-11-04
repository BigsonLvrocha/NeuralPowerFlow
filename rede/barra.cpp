/**
 * @brief implementation of bar
 * 
 * @file barra.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include <stdexcept>
#include "../util/complexutils.h"
#include "barra.hpp"
#include "branch.hpp"

namespace neuralFlux {

Bar::Bar(
    const barType type,
    const double v,
    const double teta,
    const double p,
    const double q,
    const uint64_t id,
    const double bsh
):
    Identifiable(id),
    ChangeTrackable(),
    m_e(complexPolar(v, teta)),
    m_s(complexo(p, q)),
    m_bsh(bsh),
    m_type(type) {
}

Bar::~Bar() {
}


const BarPtr Bar::newSlack(
    const double v,
    const double teta,
    const uint64_t id,
    const double p,
    const double q,
    const double bsh
) {
    return BarPtr(new Bar(Bar::Slack, v, teta, p, q, id, bsh));
}

const BarPtr Bar::newPV(
    const double p,
    const double v,
    const uint64_t id,
    const double teta,
    const double q,
    const double bsh
) {
    return BarPtr(new Bar(Bar::PV, v, teta, p, q, id, bsh));
}

const BarPtr Bar::newPQ(
    const double p,
    const double q,
    const uint64_t id,
    const double v,
    const double teta,
    const double bsh
) {
    return BarPtr(new Bar(Bar::PQ, v, teta, p, q, id, bsh));
}

const BarPtr Bar::newBar(
    barType type,
    uint64_t id,
    double v,
    double teta,
    double p,
    double q,
    double bsh
) {
    return BarPtr(new Bar(type, v, teta, p, q, id, bsh));
}

const complexo Bar::getE() const {
    return m_e;
}

const complexo Bar::getS() const {
    return m_s;
}

double Bar::getP() const {
    return m_s.real();
}

double Bar::getQ() const {
    return m_s.imag();
}

double Bar::getV() const {
    return std::sqrt(complexModulo2(m_e));
}

double Bar::getTeta() const {
    return complexFase(m_e);
}

Bar::barType Bar::getType() const {
    return m_type;
}

double Bar::getBsh() const {
    return m_bsh;
}

void Bar::setE(const complexo e) {
    m_e = e;
}

void Bar::setS(const complexo s) {
    m_s = s;
}

void Bar::setP(const double p) {
    double q(m_s.imag());
    m_s = complexo(p, q);
}

void Bar::setQ(const double q) {
    double p(m_s.real());
    m_s = complexo(p, q);
}

void Bar::setV(const double v) {
    m_e = complexPolar(v, getTeta());
}

void Bar::setTeta(const double teta) {
    m_e = complexPolar(getV(), teta);
}

void Bar::setBsh(const double bsh) {
    if (std::abs(bsh-m_bsh) > 0.000001) {
        m_bsh = bsh;
        setChanged();
    }
}

void Bar::setType(barType type) {
    if (m_type != type) {
        m_type = type;
        setChanged();
    }
}
}  // namespace neuralFlux
