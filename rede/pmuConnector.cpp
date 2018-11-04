/**
 * @file pmuConnector.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief implements class for connecting pmus in a power net
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <random>
#include <algorithm>
#include "pmuConnector.hpp"
#include "powerNet.hpp"
#include "barra.hpp"
#include "../util/tree.hpp"

using std::max;
using std::min;
using std::vector;
using std::default_random_engine;
using std::uniform_int_distribution;

namespace neuralFlux {

PmuConnector::PmuConnector(PowerNetPtr net) :
    net(net),
    bars() {
}

PmuConnector::PmuConnector() :
    net(),
    bars() {
}

const std::vector<int> PmuConnector::insertBestPmu(std::default_random_engine *eng, uint32_t maxItNotFound) {
    int itNow = 0;
    int minScore = 0;
    int scoreNow = 0;
    std::vector<int> winner;
    while (itNow < maxItNotFound) {
        this->insertPmus(eng);
        scoreNow = this->getScore();
        if (scoreNow < minScore || minScore == 0) {
            winner = this->getPmuBars(this->bars);
            minScore = scoreNow;
            itNow = 0;
        } else {
            itNow += 1;
        }
    }
    return winner;
}

const std::vector<int> PmuConnector::getPmuBars(const std::vector<BarConnectionInfoPtr> &infos) {
    vector<int> pmuBars;
    for (int i = 0 ; i < infos.size(); i++) {
        if (infos[i]->getLevel() == 0) {
            pmuBars.push_back(infos[i]->getBar()->getId());
        }
    }
    return pmuBars;
}

void PmuConnector::setNet(PowerNetPtr net) {
    this->net = net;
}

PowerNetPtr PmuConnector::getNet() {
    return this->net;
}

const vector<int> PmuConnector::insertBestPmusDeterministic() {
    Tree<int> opcoes;
    vector<vector<int>> solutions;
    vector<int> solutionsScore;
    vector<int> currentSequence;
    size_t level = 0;
    this->parseNetBars();

    vector<int> bestBars = this->getMaxConnectionIndexes();
    for (auto it = bestBars.begin(); it != bestBars.end(); it++) {
        opcoes.addNode(level, 0, *it);
    }
    while (solutions.size() == 0) {
        for (int i = 0, n = opcoes.getLevelSize(level); i < n; i++) {
            currentSequence = opcoes.getBranch(level, i);
            this->resetState();
            this->insertPmusInSequence(currentSequence);
            if (this->isStopCondition()) {
                solutions.push_back(currentSequence);
                solutionsScore.push_back(this->getScore());
            }
            if (solutions.size() > 0) {
                continue;
            }
            bestBars = this->getMaxConnectionIndexes();
            for (auto it = bestBars.begin(); it != bestBars.end(); it++) {
                opcoes.addNode(level + 1, i, *it);
            }
        }
        level += 1;
    }
    int bestIndex = -1;
    int bestCount = 0;
    for (int i = 0, n = solutionsScore.size(); i < n; i++) {
        if (solutionsScore[i] < bestCount || bestIndex == -1) {
            bestCount = solutionsScore[i];
            bestIndex = i;
        }
    }
    vector<int> bestSolution = solutions[bestIndex];
    for (int i = 0, n = bestSolution.size(); i < n; i++) {
        bestSolution[i] = this->bars[bestSolution[i]]->getBar()->getId();
    }
    return bestSolution;
}

const std::vector<BarConnectionInfoPtr>& PmuConnector::insertPmus(std::default_random_engine *gen) {
    this->parseNetBars();
    uniform_int_distribution<int> dist(0, 10);
    while (!this->isStopCondition()) {
        int maxConn = this->getMaxConnectionCount();
        std::vector<int> indexes = this->getMaxConnectionInfoIndexes(maxConn);
        int chosenOne = dist(*gen, uniform_int_distribution<int>::param_type(0, indexes.size() - 1));
        this->bars[indexes[chosenOne]]->setLevel(0);
        for (int i = 0, n = this->bars.size() ; i < n ; i++) {
            this->bars[i]->applyLevelToNeighbours();
        }
    }
    return this->bars;
}

int PmuConnector::getScore() {
    int score = 0;
    int pmus = this->countPmus();
    int level2 = this->countLevel2();
    int level2Branches = this->countLevel2Branches();
    return pmus*1024 + level2*32 + level2Branches;
}

void PmuConnector::parseNetBars() {
    this->bars = std::vector<BarConnectionInfoPtr>();
    for (int i = 0, n = this->net->getNBars() ; i < n ; i++) {
        BarPtr bar = this->net->getBarByIndex(i);
        BarConnectionInfoPtr info(new BarConnectionInfo(bar));
        this->bars.push_back(info);
    }
    for (int i = 0, n = this->bars.size() ; i < n ; i++) {
        uint64_t idI = this->bars[i]->getBar()->getId();
        for (int j = 0, m = this->bars.size() ; j < m ; j++) {
            if (i == j) {
                continue;
            }
            uint64_t idJ = this->bars[j]->getBar()->getId();
            if (this->net->areConnected(idI, idJ)) {
                this->bars[i]->setBarConnection(this->bars[j]);
            }
        }
    }
}

bool PmuConnector::isStopCondition() {
    for (int i = 0, n = this->bars.size() ; i < n ; i++) {
        if (this->bars[i]->getLevel() > 1 && this->bars[i]->getConnectionsWithLevel(2) > 0) {
            return false;
        }
    }
    return true;
}

std::vector<int> PmuConnector::getMaxConnectionIndexes() {
    int count = 0;
    int countNow = 0;
    std::vector<int> answer;
    for (int i = 0, n = this->bars.size() ; i < n; i++) {
        if (this->bars[i]->getLevel() > 1) {
            countNow = this->bars[i]->getConnectionsWithLevel(2);
            if (countNow > count) {
                answer.clear();
                answer.push_back(i);
                count = countNow;
            } else if (countNow == count) {
                answer.push_back(i);
            }
        }
    }
    return answer;
}

int PmuConnector::getMaxConnectionCount() {
    int count = 0;
    for (int i = 0, n = this->bars.size() ; i < n; i++) {
        if (this->bars[i]->getLevel() > 1) {
            count = max(this->bars[i]->getConnectionsWithLevel(2), count);
        }
    }
    return count;
}

std::vector<int> PmuConnector::getMaxConnectionInfoIndexes(int count) {
    std::vector<int> indexes;
    for (int i = 0, n = this->bars.size() ; i < n; i++) {
        if (this->bars[i]->getConnectionsWithLevel(2) == count) {
            indexes.push_back(i);
        }
    }
    return indexes;
}

int PmuConnector::countPmus() {
    int count = 0;
    for (int i = 0, n = this->bars.size() ; i < n ; i++) {
        if (this->bars[i]->getLevel() == 0) {
            count += 1;
        }
    }
    return count;
}

int PmuConnector::countLevel2() {
    int count = 0;
    for (int i= 0, n = this->bars.size(); i < n; i++) {
        if (this->bars[i]->getLevel() > 1) {
            count += 1;
        }
    }
    return count;
}

int PmuConnector::countLevel2Branches() {
    int count = 0;
    for (int i= 0, n = this->bars.size(); i < n; i++) {
        if (this->bars[i]->getLevel() == 2) {
            count += this->bars[i]->getConnectionsWithLevel();
        }
    }
    return count/2;
}


void PmuConnector::insertPmusInSequence(std::vector<int> sequence) {
    for (auto it = sequence.begin(); it != sequence.end(); it++) {
        this->insertPmuInBar(*it);
    }
}

void PmuConnector::insertPmuInBar(int index) {
    this->bars[index]->setLevel(0);
    for (int j = 0; j < 2 ; j++) {
        for (int i = 0, n = this->bars.size() ; i < n ; i++) {
            this->bars[i]->applyLevelToNeighbours();
        }
    }
}

void PmuConnector::resetState() {
    for (int i = 0, n = this->bars.size() ; i < n ; i++) {
        this->bars[i]->setLevel(3);
    }
}
}  // namespace neuralFlux
