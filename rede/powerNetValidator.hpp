/**
 * @brief defines class for power network validation
 * 
 * @file powerNetValidator.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_POWERNETVALIDATOR_HPP_
#define REDE_POWERNETVALIDATOR_HPP_

#include <vector>
#include <stdexcept>
#include "neuralFluxDeclare.hpp"

namespace neuralFlux {
/**
*   class that tests for continuity problems with the net, islanded bars and
*
*/
class PowerNetValidator {
 public:
    explicit PowerNetValidator(PowerNetPtr net);
    PowerNetValidator();

    /**
    *   sets a new net to be calculated
    */
    void setNet(const PowerNetPtr& net);

    /**
    *   check the stored net for failures, including island without slack,
    *   isolated bar
    */
    void completeCheck();

    /**
    *   check the stored net for failures, including island without slack,
    *   isolated bar
    */
    void completeCheck(const PowerNetPtr& net);
    bool hasIslandWithoutSlack();

 private:
    PowerNetPtr m_net;
    std::vector<bool> m_slackConnected;
    std::vector<uint64_t> m_id;
    // put ids into indexes
    void cacheIds();
    // gets the index of the bar with the given id
    size_t getIdIndex(uint64_t id);
    void checkSlack();
    /**
    *   check the net for bars without connections
    */
    void checkIsolatedBar();
    /**
    *   check the net for islands
    */
    void checkIsland();
    /**
    *   sweep between the bars to check if it's connected to a slack
    */
    bool sweepBars();
    bool markBarsConnected(const BarPtr& bar);
    bool checkBarConnected(const BarPtr& bar);
};

class NetHasNoSlack: public std::runtime_error {
 public:
    NetHasNoSlack():runtime_error("the power network has no slack bar"){}
};

class NetHasIsolatedBar: public std::runtime_error {
 public:
    NetHasIsolatedBar():runtime_error("the power network has bar without branches"){}
};

class NetHasIslands: public std::runtime_error{
 public:
    NetHasIslands():runtime_error("the net has islands without slack bars"){}
};
}  // namespace neuralFlux
#endif  // REDE_POWERNETVALIDATOR_HPP_
