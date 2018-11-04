/**
 * @brief defines 
 * 
 * @file connectionDense.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright
 */

#ifndef UTIL_CONNECTIONDENSE_HPP_
#define UTIL_CONNECTIONDENSE_HPP_

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace neuralFlux {

/**
*   class defining a point in a connection, has higher hierarchy than Connections
*   can have multiple connections
*   only Conector can connect it to another Connection
*/
template <typename T>
class ConnectionPoint : public boost::noncopyable{
 public:
    template<typename V, typename X> friend class Connector;
    virtual ~ConnectionPoint() = 0;
    size_t getNConnections();
    const boost::shared_ptr<T> getConnection(const size_t i);
    void setCheckWeakness();
 protected:
    void connect(const boost::shared_ptr<T>& con);
 private:
    std::vector <boost::weak_ptr<T> > m_connections;
    bool m_weak;
    void checkWeakness();
    void deleteConection(size_t i);
    size_t getConnectionIndex(const boost::shared_ptr<T>& con);
};

template <typename T>
ConnectionPoint<T>::~ConnectionPoint() {}

template <typename T>
class Connection : boost::noncopyable{
 public:
    template<typename V, typename X>
    friend class Connector;
    Connection();
    virtual ~Connection() = 0;
    boost::shared_ptr<T>& getConnection(const size_t i);
    const boost::shared_ptr<T>& getConnection(const size_t i) const;
    size_t getNConnections();
 protected:
    void connect(const boost::shared_ptr<T>& con);
 private:
    std::vector<boost::shared_ptr<T> > m_connection;
    size_t getConnectionIndex(const boost::shared_ptr<T>&  con);
};

template <typename T, typename U>
class Connector : public boost::noncopyable{
 public:
    static void makeConnection(boost::shared_ptr<T> k, boost::shared_ptr<U> m){
        k->connect(m);
        m->connect(k);
    }
};
}  // namespace neuralFlux
#endif  // UTIL_CONNECTIONDENSE_HPP_
