/**
 * @file barConnectionInfo.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <algorithm>
#include "barConnectionInfo.hpp"
#include "barra.hpp"
#include "branch.hpp"

using std::min;

namespace neuralFlux {

BarConnectionInfo::BarConnectionInfo(BarPtr bar, int level) :
    bar(bar),
    level(level),
    connected() {
}

BarConnectionInfo::BarConnectionInfo(BarPtr bar, const vector<BarConnectionInfoPtr> &connecteds, int level) :
    bar(bar),
    connected(connecteds),
    level(level) {
}

BarPtr BarConnectionInfo::getBar() {
    return this->bar;
}

int BarConnectionInfo::getLevel() {
    return this->level;
}

const vector<BarConnectionInfoPtr>& BarConnectionInfo::getConnectedBars() {
    return this->connected;
}
void BarConnectionInfo::setLevel(int level) {
    this->level = level;
}

void BarConnectionInfo::setBarConnection(BarConnectionInfoPtr bar) {
    this->connected.push_back(bar);
}

void BarConnectionInfo::applyLevelToNeighbours() {
    for (int i = 0, n = this->connected.size() ; i < n ; i++) {
        this->connected[i]->setLevel(min(this->level + 1, this->connected[i]->getLevel()));
    }
}

int BarConnectionInfo::getConnectionsWithLevel(int minLevel) {
    int count = 0;
    for (int i = 0, n = this->connected.size() ; i < n ; i++) {
        count += this->connected[i]->getLevel() >= minLevel ? 1 : 0;
    }
    return count;
}

}  // namespace neuralFlux
