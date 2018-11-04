/**
 * @brief definition of non covergence of flux
 * 
 * @file fluxonaoconvergiuexception.h
 * @author your name
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_FLUXONAOCONVERGIUEXCEPTION_H_
#define REDE_FLUXONAOCONVERGIUEXCEPTION_H_

#include <stdexcept>
#include <string>

/**
 * @brief Excessão a ser jogada quando um fluxo não converge
 */
class FluxoNaoConvergiuException: public std::logic_error {
 public:
    explicit FluxoNaoConvergiuException(const std::string& msg):
        std::logic_error(msg) {}
    FluxoNaoConvergiuException():
        logic_error(std::string("O calculo de fluxo não convergiu")) {
    }
};

#endif  // REDE_FLUXONAOCONVERGIUEXCEPTION_H_
