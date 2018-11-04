/**
 * @brief definition of class for holding ids
 * 
 * @file identifiable.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef UTIL_IDENTIFIABLE_HPP_
#define UTIL_IDENTIFIABLE_HPP_

#include <cstdlib>
#include <cstdint>

namespace neuralFlux {

class Identifiable {
 public:
    explicit Identifiable(uint64_t id);
    virtual ~Identifiable();
    uint64_t getId() const;

 private:
    const uint64_t m_id;
};
}  // namespace neuralFlux
#endif  // UTIL_IDENTIFIABLE_HPP_
