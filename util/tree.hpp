/**
 * @file tree.hpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief definition file for tree structures
 * @version 0.1
 * @date 2018-10-13
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef UTIL_TREE_HPP_
#define UTIL_TREE_HPP_

#include <vector>
#include "treeDeclare.hpp"

namespace neuralFlux {

template <typename T>
TreeNode<T>::TreeNode(T value, size_t parent) :
    value(value),
    parent(parent) {
}

template <typename T>
T TreeNode<T>::get() {
    return this->value;
}

template <typename T>
T TreeNode<T>::getParent() {
    return this->parent;
}

template <typename T>
Tree<T>::Tree() {
}

template <typename T>
Tree<T>& Tree<T>::addNode(size_t level, size_t parent, T value) {
    while (this->trunk.size() <= level) {
        this->trunk.push_back(vector<TreeNode<T>>());
    }
    this->trunk[level].push_back(TreeNode<T>(value, parent));
    return *this;
}

template <typename T>
vector<T> Tree<T>::getBranch(size_t level, size_t nodeNo) {
    vector<T> answer(level + 1);
    for (size_t i = 0; i <= level; i++) {
        answer[level-i] = this->trunk[level-i][nodeNo].get();
        nodeNo = this->trunk[level-i][nodeNo].getParent();
    }
    return answer;
}

template <typename T>
size_t Tree<T>::getLevelSize(size_t level) {
    return this->trunk[level].size();
}

template <typename T>
T Tree<T>::getValue(size_t level, size_t nodeNo) {
    return this->trunk[level][nodeNo];
}

template <typename T>
size_t Tree<T>::getSize() {
    return this->trunk.size();
}


}  // namespace neuralFlux


#endif  // UTIL_TREE_HPP_
