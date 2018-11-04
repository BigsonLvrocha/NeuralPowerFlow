/**
 * @file tree.hpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief simple tree structure
 * @version 0.1
 * @date 2018-10-13
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef UTIL_TREEDECLARE_HPP_
#define UTIL_TREEDECLARE_HPP_

#include <cstdlib>
#include <vector>

namespace neuralFlux {

using std::vector;

template<typename T>
class TreeNode {
 private:
    T value;
    size_t parent;

 public:
    TreeNode(T value, size_t parent);
    T get();
    T getParent();
};


template<typename T>
class Tree {
 private:
    vector< vector<TreeNode<T> > > trunk;
 public:
    Tree();
    Tree<T>& addNode(size_t level, size_t parent, T value);
    vector<T> getBranch(size_t level, size_t nodeNo);
    size_t getLevelSize(size_t level);
    T getValue(size_t level, size_t nodeNo);
    size_t getSize();
};

}  // namespace neuralFlux
#endif  // UTIL_TREEDECLARE_HPP_
