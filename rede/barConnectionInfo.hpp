/**
 * @file barConnectionInfo.hpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief defines class for storing levels of connection in a bar
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef REDE_BARCONNECTIONINFO_HPP_
#define REDE_BARCONNECTIONINFO_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include "neuralFluxDeclare.hpp"


using std::vector;
using boost::shared_ptr;

namespace neuralFlux {

class BarConnectionInfo;
typedef shared_ptr<BarConnectionInfo> BarConnectionInfoPtr;

class BarConnectionInfo {
 public:
    explicit BarConnectionInfo(BarPtr bar, int level = 3);
    BarConnectionInfo(BarPtr bar, const vector<BarConnectionInfoPtr> &connecteds, int level = 3);
    BarPtr getBar();
    int getLevel();
    const vector<BarConnectionInfoPtr> &getConnectedBars();
    void setLevel(int level);
    void setBarConnection(BarConnectionInfoPtr bar);
    void applyLevelToNeighbours();
    int getConnectionsWithLevel(int minLevel = 2);

 private:
    vector<BarConnectionInfoPtr> connected;
    int level;
    BarPtr bar;

    void printResult(BarPtr bar);
    void printInput(BarPtr bar);
};
}  // namespace neuralFlux

#endif  // REDE_BARCONNECTIONINFO_HPP_
