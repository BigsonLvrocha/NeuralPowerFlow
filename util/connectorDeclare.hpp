/**
 * @brief defines 
 * 
 * @file connector.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef UTIL_CONNECTORDECLARE_HPP_
#define UTIL_CONNECTORDECLARE_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "connectionPoint.hpp"
#include "connection.hpp"


namespace neuralFlux{

/**
*   Class that can make connection between classes
*   U is the child class type
*   abstract class
*/

template <typename T, typename U>
class Connector: public ConnectionPoint<T>, public Connection<U>, public boost::noncopyable {
 public:
    static void connect(boost::shared_ptr<T> m, boost::shared_ptr<U> k) {
        m->connect(k);
        k->connect(m);
    }
    virtual ~Connector() = 0;
};

Connector::~Connector() {
}

}  // namespace neuralFlux
#endif  // UTIL_CONNECTORDECLARE_HPP_
