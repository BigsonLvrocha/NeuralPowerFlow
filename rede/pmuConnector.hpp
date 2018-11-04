/**
 * @file pmuConnector.hpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief defines class for giving pmu 
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <random>
#include <vector>
#include <ctime>
#include "neuralFluxDeclare.hpp"
#include "barConnectionInfo.hpp"

#ifndef REDE_PMUCONNECTOR_HPP_
#define REDE_PMUCONNECTOR_HPP_

using std::vector;

namespace neuralFlux {

class PmuConnector {
 public:
    explicit PmuConnector(PowerNetPtr net);
    PmuConnector();
    void setNet(PowerNetPtr net);
    PowerNetPtr getNet();
    const vector<int> insertBestPmusDeterministic();
    const std::vector<BarConnectionInfoPtr>& insertPmus(std::default_random_engine *gen);
    const std::vector<int> insertBestPmu(std::default_random_engine *eng, uint32_t maxItNotFound = 1000);
    const std::vector<int> getPmuBars(const std::vector<BarConnectionInfoPtr> &infos);
    int getScore();
    void parseNetBars();

 private:
    PowerNetPtr net;
    std::vector<BarConnectionInfoPtr> bars;

    bool isStopCondition();
    std::vector<int> getMaxConnectionIndexes();
    int getMaxConnectionCount();
    std::vector<int> getMaxConnectionInfoIndexes(int count);
    int countPmus();
    int countLevel2();
    int countLevel2Branches();
    void insertPmusInSequence(std::vector<int> sequence);
    void insertPmuInBar(int index);
    void resetState();
};
}  // namespace neuralFlux

#endif  // REDE_PMUCONNECTOR_HPP_
