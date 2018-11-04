/**
 * @brief defines a connection point
 * 
 * @file connectionPoint.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef UTIL_CONNECTIONPOINT_HPP_
#define UTIL_CONNECTIONPOINT_HPP_

#include <exception>
#include <stdexcept>
#include "connectionDense.hpp"

namespace neuralFlux {
template <typename T>
size_t ConnectionPoint<T>::getNConnections() {
    if (m_weak) {
        checkWeakness();
    }
    return m_connections.size();
}

template <typename T>
const boost::shared_ptr<T> ConnectionPoint<T>::getConnection(const size_t i) {
    if (m_weak) {
        checkWeakness();
    }
    if (m_connections.size() <= i) {
        throw std::out_of_range("connection does not exist");
    }
    return m_connections[i].lock();
}

template <typename T>
void ConnectionPoint<T>::setCheckWeakness() {
    m_weak = true;
}

template <typename T>
void ConnectionPoint<T>::connect(const boost::shared_ptr<T>& con) {
    if (m_weak) {
        checkWeakness();
    }
    try {
        getConnectionIndex(con);
    } catch(std::runtime_error e) {
        m_connections.push_back(con);
        return;
    }
    throw std::runtime_error("cannot connect same objects twice");
}

template <typename T>
void ConnectionPoint<T>::checkWeakness() {
    for (size_t i =0; i < m_connections.size() ; i++) {
        if (m_connections[i].expired()) {
            deleteConection(i);
            --i;
        }
    }
    m_weak = false;
}

template <typename T>
void ConnectionPoint<T>::deleteConection(size_t i) {
    if (m_connections.size() <= i) {
        throw std::out_of_range("connection doesn't exist");
    }
    for (size_t j = i+1, n = m_connections.size() ; j < n ; j++) {
        m_connections[j-1] = m_connections[j];
    }
    m_connections.pop_back();
}

template <typename T>
size_t ConnectionPoint<T>::getConnectionIndex(const boost::shared_ptr<T>& con) {
    for (size_t i = 0, n = m_connections.size() ; i < n ; i++) {
        if (m_connections[i].lock() == con) {
            return i;
        }
    }
    throw std::runtime_error("connection not found");
}
}  // namespace neuralFlux
#endif  // UTIL_CONNECTIONPOINT_HPP_
