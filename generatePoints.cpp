/**
 * @file generatePoints.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief main file to generate a new net
 * @version 0.1
 * @date 2018-10-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <ctime>
#include <string>
#include <iostream>
#include "rede/netDatasetGenerator.hpp"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
    if (argc < 4) {
        cout << "Program usage: bin [netFile] [inputDataFile] [solutionDataFile]" << endl;
        return 2;
    }
    int points = 10000;
    double dS   = 0.03;
    double dS_  = 0.30;
    double dP   = 0.01;
    double dP_  = 0.02;
    double dQ   = 0.005;
    double dQ_  = 0.01;
    double dPV  = 0.01;
    double dPV_ = 0.04;
    uint64_t seed = (uint64_t) std::time(NULL);
    string inputNetFile(argv[1]);
    string outInputFile(argv[2]);
    string outSolutionFile(argv[3]);
    for (int i = 4; i + 1 < argc ; i += 2) {
        string option(argv[i]);
        string value(argv[i + 1]);
        if (option == "--points") {
            points = std::stoi(value);
            continue;
        } else if (option == "--ds") {
            dS = std::stof(value);
            continue;
        } else if (option == "--ds-") {
            dS_ = std::stof(value);
            continue;
        } else if (option == "--dp") {
            dP = std::stof(value);
        } else if (option == "--dp-") {
            dP_ = std::stof(value);
        } else if (option == "--dq") {
            dQ = std::stof(value);
        } else if (option == "--dq-") {
            dQ_ = std::stof(value);
        } else if (option == "--dpv") {
            dPV = std::stof(value);
        } else if (option == "--dpv-") {
            dPV_ = std::stof(value);
        } else if (option == "--seed") {
            seed = std::stoi(value);
        }
    }
    ifstream net(inputNetFile.c_str());
    ofstream input(outInputFile.c_str());
    ofstream solution(outSolutionFile.c_str());
    std::default_random_engine eng(seed);
    neuralFlux::NetDatasetGenerator gen;
    std::vector<int> pmus = gen.generateFromFile(
        net,
        input,
        solution,
        &eng,
        points,
        dS,
        dS_,
        dP,
        dP_,
        dQ,
        dQ_,
        dPV,
        dPV_,
        std::cout);
    cout << points << " pontos gerados com pmus nas barras: " << endl;
    for (int i = 0 ; i < pmus.size() ; i++) {
        cout << pmus[i] << ",";
    }
    cout << endl;
    return 0;
}
