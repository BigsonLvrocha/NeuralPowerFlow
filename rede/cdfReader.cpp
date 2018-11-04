/**
 * @file cdfReader.cpp
 * @author your name (you@domain.com)
 * @brief defines implementation of cdfReader
 * @version 0.1
 * @date 2018-10-02
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <cmath>
#include <cstring>
#include <stdexcept>
#include "cdfReader.hpp"
#include "powerNet.hpp"
#include "barInfo.hpp"
#include "branchInfo.hpp"

using std::runtime_error;

namespace neuralFlux {

CdfReader::CdfReader() {
}

PowerNetPtr CdfReader::readNet(ifstream &in) {
    PowerNetPtr net = PowerNetPtr(new PowerNet());
    double base = this->readBasePower(in);
    this->readBuses(in, net, base);
    this->readBranches(in, net, base);
    return net;
}

double CdfReader::readBasePower(ifstream &input) {
    char line[128];
    input.getline(line, 130);
    string lineObj = line;
    lineObj = lineObj.substr(30, 5);
    return std::stod(lineObj);
}

void CdfReader::readBuses(ifstream &input, PowerNetPtr net, float basePower) {
    char line[128];
    input.getline(line, 130);
    for (;;) {
        input.getline(line, 130);
        if (strlen(line) < 10) {
            break;
        }
        BarInfo info = this->readBusLine(line, basePower);
        net->addBar(
            info.getType(),
            info.getId(),
            info.getV(),
            info.getTeta(),
            info.getP(),
            info.getQ(),
            info.getBsh());
    }
}

void CdfReader::readBranches(ifstream &input, PowerNetPtr net, float basePower) {
    char line[128];
    input.getline(line, 130);
    for (;;) {
        input.getline(line, 130);
        if (strlen(line) < 10) {
            break;
        }
        BranchInfo info = this->readBranchLine(line, basePower);
        net->connect(
            info.getIdk(),
            info.getIdm(),
            info.getRkm(),
            info.getXkm(),
            info.getBshkm(),
            info.getA(),
            info.getPhi());
    }
}

BarInfo CdfReader::readBusLine(char *line, float basePower) {
    char idStr[] = {line[0], line[1], line[2], line[3], 0};
    char typeStr[] = {line[24], line[25], 0};
    char vStr[] = {line[27], line[28], line[29], line[30], line[31], line[32], 0};
    char tetaStr[] = {line[33], line[34], line[35], line[36], line[37], line[38], line[39], 0};
    char plStr[] = {line[40], line[41], line[42], line[43], line[44], line[45], line[46], line[47], line[48], 0};
    char qlStr[] = {
        line[49],
        line[50],
        line[51],
        line[52],
        line[53],
        line[54],
        line[55],
        line[56],
        line[57],
        line[58],
        0
    };
    char pgStr[] = {line[59], line[60], line[61], line[62], line[63], line[64], line[65], line[66], 0};
    char qgStr[] = {line[67], line[68], line[69], line[70], line[71], line[72], line[73], line[74], 0};
    char bshStr[] = {line[114], line[115], line[116], line[117], line[118], line[119], line[120], line[121], 0};
    int id = std::stoi(idStr);
    int typeInt = std::stoi(typeStr);
    double v = std::stod(vStr);
    double teta = std::stod(tetaStr) * M_PI / 180.0;
    double pl = std::stod(plStr) / basePower;
    double ql = std::stod(qlStr) / basePower;
    double pg = std::stod(pgStr) / basePower;
    double qg = std::stod(qgStr) / basePower;
    double bsh = std::stod(bshStr);
    Bar::barType type;
    switch (typeInt) {
        case 0:
        case 1:
            type = Bar::PQ;
            break;
        case 2:
            type = Bar::PV;
            break;
        case 3:
            type = Bar::Slack;
            break;
    }
    return BarInfo (id, type, v, teta, pg - pl, qg - ql, bsh);
}

BranchInfo CdfReader::readBranchLine(char *line, float basePower) {
    char idkStr[] = {line[0], line[1], line[2], line[3], 0};
    char idmStr[] = {line[5], line[6], line[7], line[8], 0};
    char rkmStr[] = {
        line[19],
        line[20],
        line[21],
        line[22],
        line[23],
        line[24],
        line[25],
        line[26],
        line[27],
        line[28],
        0
    };
    char xkmStr[] = {
        line[29],
        line[30],
        line[31],
        line[32],
        line[33],
        line[34],
        line[35],
        line[36],
        line[37],
        line[38],
        line[39],
        0
    };
    char bshkmStr[] = {
        line[40],
        line[41],
        line[42],
        line[43],
        line[44],
        line[45],
        line[46],
        line[47],
        line[48],
        line[49],
        0
    };
    char aStr[] = {
        line[76],
        line[77],
        line[78],
        line[79],
        line[80],
        line[81],
        0
    };
    char phiStr[] = {
        line[83],
        line[84],
        line[85],
        line[86],
        line[87],
        line[88],
        line[89],
        0
    };
    int idk     = std::stoi(idkStr);
    int idm     = std::stoi(idmStr);
    double rkm  = std::stod(rkmStr);
    double xkm  = std::stod(xkmStr);
    double bshkm = std::stod(bshkmStr);
    double a = std::stod(aStr);
    double phi  = std::stod(phiStr);
    if (a < 0.0001 && a > -0.0001) {
        a = 1.0;
    }
    return BranchInfo(idk, idm, rkm, xkm, bshkm, a, phi);
}

}  // namespace neuralFlux
