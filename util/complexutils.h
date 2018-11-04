/**
 * @brief implements the functions for complex datatypes
 * 
 * @file complexutils.h
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef UTIL_COMPLEXUTILS_H_
#define UTIL_COMPLEXUTILS_H_

#include "complexutilsDeclare.h"

/*
*   Resumo:
*
*   const complexo complexConjugado(const complexo& a)
*   double complexModulo2(const complexo& a)
*   double complexFase(const complexo &a)
*   const complexo complexCis(const double a)
*   const complexo complexPolar(const double magnetude, const double angle)
*/

namespace neuralFlux {
/**
 * @brief conjugado retorna o conjugado do numero complexo
 * @param a o numero complexo
 * @return  o conjugado do numero passado
 */
inline const complexo complexConjugado(const complexo& a) {
    return complexo(a.real(), -a.imag());
}

/**
 * @brief modulo    calcula o módulo quadrático do numero complexos
 * @param a o numero complexo
 * @return  o módulo do número passado
 */
inline double complexModulo2(const complexo& a) {
    return a.real()*a.real() + a.imag()*a.imag();
}

/**
 * @brief complexFase   calcula o angulo de fase do angulo complexo
 * @param a o numero complexo a ser calculado
 * @return  o angulo de fase do numero complexo em [-pi,pi] radianos
 */
inline double complexFase(const complexo &a) {
    return atan2(a.imag(), a.real());
}

/**
 * @brief cis   retorna cosseno + i*seno do angulo passado
 * @param a o angulo
 * @return cosseno(a) + i*seno(a)
 */
inline const complexo complexCis(const double a) {
    return complexo(cos(a), sin(a));
}

/**
 * @brief complexPolar retorna o numero complexo construido da forma polar
 * @param magnetude o módulo do numero
 * @param angle o angulo de fase do numero
 * @return  o numero complexo em forma retangular
 */
inline const complexo complexPolar(const double magnetude, const double angle) {
    return magnetude*complexCis(angle);
}
}  // namespace neuralFlux
#endif  // UTIL_COMPLEXUTILS_H_
