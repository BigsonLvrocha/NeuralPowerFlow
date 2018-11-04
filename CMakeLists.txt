cmake_minimum_required(VERSION 3.0.0)
project(NeuralPowerFlow VERSION 0.1.0)
SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include(CTest)
include_directories(libraries)

enable_testing()



add_library(
    NeuralNet
    rede/admitanceCalc.cpp
    rede/barInfo.cpp
    rede/barra.cpp
    rede/branch.cpp
    rede/branchInfo.cpp
    rede/gausSiedel.cpp
    rede/netFactory.cpp
    rede/netStateHandler.cpp
    rede/newtonSolver.cpp
    rede/powerCalc.cpp
    rede/powerFlowSolver.cpp
    rede/powerNet.cpp
    rede/powerNetValidator.cpp
    rede/cdfReader.cpp
    rede/barConnectionInfo.cpp
    rede/pmuConnector.cpp
    rede/netSolutionPrinter.cpp
    rede/netDatasetGenerator.cpp
    util/identifiable.cpp
    util/changeTrackable.cpp
)

add_executable(
    NeuralTest
    test/barConnectionInfoTest.cpp
    test/testMaster.cpp
    test/admitanceCalcTest.cpp
    test/barraTest.cpp
    test/branchTest.cpp
    test/changeTrackableTest.cpp
    test/NetFactoryTest.cpp
    test/newtonFlowTest.cpp
    test/powerCalcTest.cpp
    test/powerFlowTest.cpp
    test/powerNetAssertionTest.cpp
    test/powerNetStateHandlerTest.cpp
    test/powerNetTest.cpp
    test/cdfReaderTest.cpp
    test/pmuConnectorTest.cpp
    test/netSolutionPrinterTest.cpp
    test/treeTest.cpp
)

add_executable(
    DataGen
    generatePoints.cpp
)

target_link_libraries(NeuralTest libboost_unit_test_framework.so)
target_link_libraries(NeuralTest NeuralNet)
target_link_libraries(DataGen NeuralNet)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
