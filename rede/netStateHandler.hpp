/**
 * @brief definition of net state handler class
 * 
 * @file netStateHandler.hpp
 * @author Luiz Victor Linhares
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_NETSTATEHANDLER_HPP_
#define REDE_NETSTATEHANDLER_HPP_

#include <random>
#include <ctime>
#include <vector>
#include "neuralFluxDeclare.hpp"
#include "barInfo.hpp"
#include "branchInfo.hpp"

namespace neuralFlux {

class NetStateHandler {
 public:
    NetStateHandler();
    explicit NetStateHandler(const PowerNetPtr& net);
    void setNet(const PowerNetPtr& net);
    void resetState();
    void saveState();
    void shuffleNet(
        double deltaS,
        double deltaS_,
        double deltaP,
        double deltaP_,
        double deltaQ,
        double deltaQ_,
        double deltaPV,
        double deltaPV_,
        std::default_random_engine *gen);

 private:
    std::vector<BarInfo> m_bar;
    std::vector<BranchInfo> m_branch;
    PowerNetPtr m_net;
    bool netStructureOk();
    void saveBars();
    void saveBranches();
    void recreateNetBranches();
    void recreateNetBars();
    void resetBarsState();
    void resetBranchesState();
};
}  // namespace neuralFlux
#endif  // REDE_NETSTATEHANDLER_HPP_
