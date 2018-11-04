/**
 * @brief implenets simple class for holding id
 * 
 * @file identifiable.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 */

#include <iostream>
#include "identifiable.hpp"

namespace neuralFlux {

Identifiable::Identifiable(uint64_t id):m_id(id) {
}

Identifiable::~Identifiable() {
}

uint64_t Identifiable::getId() const {
    return m_id;
}
}  // namespace neuralFlux
