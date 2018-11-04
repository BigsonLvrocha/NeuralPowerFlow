/**
 * @brief defines utility functions for dealing with complex data types
 * 
 * @file complexutilsDeclare.h
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef UTIL_COMPLEXUTILSDECLARE_H_
#define UTIL_COMPLEXUTILSDECLARE_H_

#include <complex>

namespace neuralFlux {

typedef std::complex<double> complexo;

const complexo complexConjugado(const complexo& a);
double complexModulo2(const complexo& a);
double complexFase(const complexo &a);
const complexo complexCis(const double a);
const complexo complexPolar(const double magnetude, const double angle);
}  // namespace neuralFlux

#endif  // UTIL_COMPLEXUTILSDECLARE_H_
