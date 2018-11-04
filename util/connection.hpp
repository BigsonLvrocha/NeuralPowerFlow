/**
 * @brief defines a base connection class
 * 
 * @file connection.hpp
 * @author your name
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef UTIL_CONNECTION_HPP_
#define UTIL_CONNECTION_HPP_

#include <cstdlib>
#include <stdexcept>
#include <utility>
#include <iostream>
#include "connectionDense.hpp"

namespace neuralFlux {

template <typename T>
Connection<T>::Connection() {
}

template <typename T>
Connection<T>::~Connection() {
    for (size_t i=0, n = this->m_connection.size() ; i < n ; i++) {
        this->m_connection[i]->setCheckWeakness();
    }
}

template <typename T>
boost::shared_ptr<T>& Connection<T>::getConnection(const size_t i) {
    if (this->m_connection.size() <= i) {
        throw std::out_of_range("connection doesn't exists");
    }
    return this->m_connection[i];
}

template <typename T>
const boost::shared_ptr<T>& Connection<T>::getConnection(const size_t i) const {
    if (this->m_connection.size() <= i) {
        throw std::out_of_range("connection doesn't exists");
    }
    return this->m_connection[i];
}

template <typename T>
size_t Connection<T>::getNConnections() {
    return this->m_connection.size();
}

template <typename T>
void Connection<T>::connect(const boost::shared_ptr<T>& con) {
    try {
        getConnectionIndex(con);
    } catch(std::runtime_error e) {
        this->m_connection.push_back(con);
        return;
    }
    throw std::runtime_error("cannot connect same objects twice");
}

template <typename T>
size_t Connection<T>::getConnectionIndex(const boost::shared_ptr<T>&  con) {
    for (size_t i = 0, n = this->m_connection.size() ; i < n ; i++) {
        if (this->m_connection[i] == con) {
            return i;
        }
    }
    throw std::runtime_error("connection not found");
}

}  // namespace neuralFlux

#endif  // UTIL_CONNECTION_HPP_

