/**
 * @brief defines power netork class
 * 
 * @file powerNet.hpp
 * @author Luiz Victor Linhares Rocha
 * @date 2018-09-22
 * @copyright 2018
 */

#ifndef REDE_POWERNET_HPP_
#define REDE_POWERNET_HPP_

#include <vector>
#include <boost/shared_ptr.hpp>
#include "../libraries/Eigen/Eigen"
#include "neuralFluxDeclare.hpp"
#include "barra.hpp"
#include "../util/identifiable.hpp"

namespace neuralFlux {

class PowerNet : public Identifiable {
 public:
    explicit PowerNet(uint64_t id = 0);
    virtual ~PowerNet();
    PowerNet(const PowerNet& net);
    PowerNet& operator=(const PowerNet&);
    // network setup methods
    void addSlackBar(
        const size_t id,
        const double V,
        const double teta,
        const double p = 0,
        const double q = 0,
        const double bsh = 0);
    void addPQBar(
        const size_t id,
        const double p,
        const double q,
        const double v = 1,
        const double teta = 0,
        const double bsh = 0);
    void addPVBar(
        const size_t id,
        const double p,
        const double v,
        const double teta = 0,
        const double q = 0,
        const double bsh = 0);
    void addBar(
        const Bar::barType type,
        const size_t id,
        const double v = 1,
        const double teta = 0,
        const double p = 0,
        const double q = 0,
        const double bsh = 0);
    /** connect bars using the bars ids, hence the bars insertion order*/
    void connect(
        size_t k,
        size_t m,
        const double rkm,
        const double xkm,
        const double bshkm = 0,
        const double a = 1,
        const double phi = 0);
    /** connect bars using the sorted vector*/
    void connectByIndex(
        size_t k,
        size_t m,
        const double rkm,
        const double xkm,
        const double bshkm = 0,
        const double a = 1,
        const double phi = 0);
    // network tear down methods
    void deleteBranch(uint64_t id);
    void deleteBranch(uint64_t kId, uint64_t mId);
    void deleteBranchByIndex(uint64_t i);
    void deleteBranchByBarIndex(uint64_t kI, uint64_t mI);
    void deleteBar(size_t i);
    void deleteBarByIndex(size_t i);
    void cleanBranches();
    void clean();
    // network reset to standard
    void setFlatStart();
    // caracteristics getters
    size_t getNBars() const;
    size_t getNBranches() const;
    size_t getNSlack() const;
    size_t getNPQ() const;
    size_t getNPV() const;
    size_t getBarIndex(const BarPtr bar) const;
    size_t getBarIndex(const size_t id) const;
    size_t getBranchIndex(const BranchPtr branch) const;
    // gets branch index by branch id
    size_t getBranchIndex(uint64_t id) const;
    // gets branch index by bars id
    size_t getBranchIndex(uint64_t kId, uint64_t mId) const;
    // gets branch index by index of bars
    size_t getBranchIndexByIndex(size_t kId, size_t mId) const;
    // gets bar by id, using addition sequence
    const BarPtr getBar(uint64_t i) const;
    // gets bar by index
    const BarPtr getBarByIndex(size_t i) const;
    // gets branch by id
    const BranchPtr getBranch(uint64_t i) const;
    // gets branch by index
    const BranchPtr getBranchByIndex(size_t i) const;
    // gets branch by id of two bars
    const BranchPtr getBranch(uint64_t kId, uint64_t mId) const;
    // gets branch by index of two bars
    const BranchPtr getBranchByIndex(size_t kId, size_t mId) const;
    void updateY();
    const complexo getY(size_t i, size_t j);
    void completeNetPowers();
    // calcula a potência demandada de uma barra, calculada a partir das admitancias
    complexo getSk(size_t i);
    double getTetaKm(uint64_t kId, uint64_t mId) const;
    double getTetaKmByIndex(size_t kI, size_t mI) const;
    // checks if 2 bars are connected
    bool areConnected(uint64_t i, uint64_t j);
    bool areConnectedByIndex(size_t i, size_t j);

 private:
    std::vector<BarPtr> m_busBars;
    std::vector<BranchPtr> m_branches;
    Eigen::MatrixXcd m_Y;

    size_t m_nPQ;
    size_t m_nPV;
    size_t m_nSlack;

    /**
    *   @brief called when a bar is deleted, decrement the count for the type the bar was
    */
    void discontBarCount(Bar::barType type);
    bool checkBarIdExists(uint64_t id);
    void sortBackBranch();
    void sortBackBar();
    bool checkChanged();
};
}  // namespace neuralFlux
#endif  // REDE_POWERNET_HPP_
