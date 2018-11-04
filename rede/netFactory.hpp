/**
 * @brief definition of powernetwork factory
 * 
 * @file netFactory.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_NETFACTORY_HPP_
#define REDE_NETFACTORY_HPP_

#include <vector>
#include "neuralFluxDeclare.hpp"
#include "branchInfo.hpp"
#include "barInfo.hpp"

namespace neuralFlux {

/**
*   Class that can be set to output a single type of net
*   Net information are kept by add order
*/
class NetFactory{
 public:
    NetFactory();
    explicit NetFactory(const PowerNetPtr& net);

    void setNet(const PowerNetPtr& net);
    PowerNetPtr newNet();
    void addSlack(uint64_t id, double v = 1, double teta = 0, double p = 0, double q = 0, double bsh = 0);
    void addPQ(uint64_t id, double p = 0, double q = 0, double v = 1, double teta = 0, double bsh = 0);
    void addPV(uint64_t id, double p = 0, double v = 1, double teta = 0, double q = 0, double bsh = 0);
    void addBranch(uint64_t kId, uint64_t mId, double rkm, double xkm, double bshkm = 1, double a = 1, double phi = 0);
    // removes a bar, by index
    void removeBar(size_t i);
    void removeBranch(size_t i);
    void clearNet();
    void clearBranches();
    void clearBars();

 private:
    std::vector<BarInfo> m_busBar;
    std::vector<BranchInfo> m_branch;
    void addBarsFrom(const PowerNetPtr& net);
    void addBranchesFrom(const PowerNetPtr& net);
    void addBarsTo(const PowerNetPtr& net);
    void addBranchesTo(const PowerNetPtr& net);
};
}  // namespace neuralFlux
#endif  // REDE_NETFACTORY_HPP_
