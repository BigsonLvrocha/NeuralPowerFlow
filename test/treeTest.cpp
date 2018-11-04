/**
 * @file treeTest.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief tests the 
 * @version 0.1
 * @date 2018-10-13
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include <vector>
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../util/tree.hpp"

using neuralFlux::Tree;
using std::vector;

BOOST_AUTO_TEST_CASE(treeCreationTest) {
    Tree<int> testTree;
    testTree.addNode(0, 0, 1);
    testTree.addNode(1, 0, 2);
    testTree.addNode(1, 0, 3);
    testTree.addNode(2, 0, 4);
    testTree.addNode(2, 1, 5);
    BOOST_TEST(testTree.getLevelSize(0) == 1);
    BOOST_TEST(testTree.getLevelSize(1) == 2);
    BOOST_TEST(testTree.getLevelSize(2) == 2);
}

BOOST_AUTO_TEST_CASE(treeBranchTest) {
    Tree<int> testTree;
    testTree.addNode(0, 0, 1);
    testTree.addNode(1, 0, 2);
    testTree.addNode(1, 0, 3);
    testTree.addNode(2, 0, 4);
    testTree.addNode(2, 1, 5);
    vector<int> branch = testTree.getBranch(2, 0);
    BOOST_TEST(branch.size() == 3);
    BOOST_TEST(branch[0] == 1);
    BOOST_TEST(branch[1] == 2);
    BOOST_TEST(branch[2] == 4);
}
