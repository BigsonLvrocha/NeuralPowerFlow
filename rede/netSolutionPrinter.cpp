/**
 * @file netSolutionPrinter.cpp
 * @author Luiz Victor Linhares Rocha (luizvictorlrocha@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-05
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <cmath>
#include <algorithm>
#include "netSolutionPrinter.hpp"
#include "powerNet.hpp"
#include "barra.hpp"
#include "branch.hpp"
#include "../util/complexutils.h"

using neuralFlux::complexo;

namespace neuralFlux {
NetSolutionPrinter::NetSolutionPrinter(PowerNetPtr net) :
    net(net) {
}

NetSolutionPrinter::NetSolutionPrinter():
    net() {
}

NetSolutionPrinter& NetSolutionPrinter::setNet(PowerNetPtr net) {
    this->net = net;
    return *this;
}

PowerNetPtr NetSolutionPrinter::getNet() {
    return this->net;
}

NetSolutionPrinter& NetSolutionPrinter::printHeaderWithPmu(
    ostream &streamInput,
    ostream &streamOutput,
    vector<int> pmus) {
    for (int i = 0, n = this->net->getNBars(); i < n; i++) {
        BarPtr bar = this->net->getBarByIndex(i);
        int id = bar->getId();
        std::vector<int>::iterator it = std::find(pmus.begin(), pmus.end(), id);
        if (it != pmus.end()) {
            streamInput
                << "P" << id << ","
                << "Q" << id << ","
                << "V" << id << ","
                << "Teta" << id << ",";
            for (int j = 0, m = this->net->getNBars() ; j < m ; j++) {
                if (i == j) {
                    continue;
                }
                if (!this->net->areConnectedByIndex(i, j)) {
                    continue;
                }
                BarPtr barJ = this->net->getBarByIndex(j);
                BranchPtr branch = this->net->getBranchByIndex(i, j);
                complexo ej = barJ->getE();
                complexo ei = bar->getE();
                complexo z = branch->getZkm();
                complexo I = (ej - ei) / z;
                streamInput <<
                    "I" << id << "." << barJ->getId() << "," <<
                    "tetaI" << id << "." << barJ->getId() << ",";
            }
        } else {
            switch (bar->getType()) {
                case Bar::Slack: {
                    streamInput
                        << "P" << id << ","
                        << "V" << id << ",";
                    break;
                }
                case Bar::PQ: {
                    streamOutput
                        << "V" << id << ","
                        << "Teta" << id << ",";
                    break;
                }
                case Bar::PV: {
                    streamInput
                        << "P" << id << ","
                        << "V" << id << ",";
                    streamOutput
                        << "Teta" << id << ",";
                }
            }
        }
    }
    streamInput << std::endl;
    streamOutput << std::endl;
    return *this;
}
NetSolutionPrinter& NetSolutionPrinter::printSolutionsWithPmu(
    ostream &streamInput,
    ostream &streamOutput,
    vector<int> pmus
) {
    for (int i = 0, n = this->net->getNBars(); i < n; i++) {
        BarPtr bar = this->net->getBarByIndex(i);
        int id = bar->getId();
        auto it = std::find(pmus.begin(), pmus.end(), id);
        if (it != pmus.end()) {
            streamInput
                << bar->getP() << ","
                << bar->getQ() << ","
                << bar->getV() << ","
                << bar->getTeta() << ",";
            for (int j = 0, m = this->net->getNBars() ; j < m ; j++) {
                if (i == j) {
                    continue;
                }
                if (!this->net->areConnectedByIndex(i, j)) {
                    continue;
                }
                BarPtr barJ = this->net->getBarByIndex(j);
                BranchPtr branch = this->net->getBranchByIndex(i, j);
                complexo I;
                if (bar == branch->getK()) {
                    I = branch->getIkm();
                } else {
                    I = branch->getImk();
                }
                streamInput <<
                    std::sqrt(complexModulo2(I)) << "," <<
                    complexFase(I) << ",";
            }
        } else {
            switch (bar->getType()) {
                case Bar::Slack: {
                    streamInput
                        << bar->getP() << ","
                        << bar->getV() << ",";
                    break;
                }
                case Bar::PQ: {
                    streamOutput
                        << bar->getV() << ","
                        << bar->getTeta() << ",";
                    break;
                }
                case Bar::PV: {
                    streamInput
                        << bar->getP() << ","
                        << bar->getV() << ",";
                    streamOutput
                        << bar->getTeta() << ",";
                }
            }
        }
    }
    streamInput << std::endl;
    streamOutput << std::endl;
    return *this;
}

}  // namespace neuralFlux
