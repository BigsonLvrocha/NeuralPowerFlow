/**
 * @brief implements classes that are change trackable
 * 
 * @file changeTrackable.cpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#include "changeTrackable.hpp"


namespace neuralFlux {

ChangeTrackable::ChangeTrackable():m_changed(true) {}

void ChangeTrackable::clearChanged() {
    m_changed = false;
}

void ChangeTrackable::setChanged() {
    m_changed = true;
}

bool ChangeTrackable::isChanged() {
    return m_changed;
}
}  // namespace neuralFlux
