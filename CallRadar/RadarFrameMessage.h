#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include "Logger.h"

class RadarRunningCls {
public:
    enum RadarRunningState
    {
        None = 0,
        Stopped = 1,
        Returning = 2,
        Completed = 3,
        ReadyA = 4,
        ReadyB = 5,
        ReadyC = 6,
        ReadyD = 7,
        ReadyE = 8,
        Scanning = 9
    };
};

#pragma pack(push, 1)

struct RadarFrameMessagePoint
{
    uint16_t Value1;
    uint16_t Value2;
    uint16_t Value3;
};

struct RadarFrameMessage
{
    RadarFrameMessagePoint Header;
    RadarFrameMessagePoint Points[1000];

    // NUM(1001)
    uint16_t BinNubA;
    uint16_t BinNubB;
    uint16_t Field1;
    // TMP(1002)
    int32_t Temperature;
    uint16_t Field2;
    // ID(1003)
    uint32_t Id;
    uint16_t Field3;
    // INF(1004)
    uint32_t INF;
    uint16_t Field4;
    // SPD(1005)
    uint16_t Field51;
    uint16_t Speed;
    uint16_t Field52;
    // ARG(1006)
    uint16_t Field61;
    uint16_t Arg;
    uint16_t Field62;
    // CFGID(1007)
    uint16_t ConfigId;
    uint16_t LP;
    uint16_t Field7;
    // S1-S2(1008)
    uint16_t S1;
    uint16_t S2;
    uint16_t Field8;
    // NSPD(1009)
    uint16_t NSPD;
    uint16_t Type;
    uint16_t Field9;
    // S3-S4(1010)
    uint16_t S3;
    uint16_t S4;
    uint16_t Field10;

    RadarFrameMessagePoint Footer[9];

    RadarFrameMessagePoint SpareGroups[8];
    RadarFrameMessagePoint Tail;

    RadarRunningCls::RadarRunningState GetRunningState()
    {
        //log4c->debug("................GetRunningState:S3:" + std::to_string(S3));
        switch (S3) {
        case 0x55aa:
            //log4c->debug("................0x55aa");
            switch (S4) {
            case 0x0000:
                //std::cout << "Stopped"  << std::endl;
                //log4c->debug("................Stopped");
                return RadarRunningCls::Stopped;
            case 0xaa55:
                //std::cout << "Returning" << std::endl;
                //log4c->debug("................Returning");
                return RadarRunningCls::Returning;
            case 0x31:
                //std::cout << "ReadyA" << std::endl;
                //log4c->debug("................ReadyA");
                return RadarRunningCls::ReadyA;
            case 0x32:
                //std::cout << "ReadyB" << std::endl;
                //log4c->debug("................ReadyB");
                return RadarRunningCls::ReadyB;
            case 0x33:
                //std::cout << "ReadyC" << std::endl;
                //log4c->debug("................ReadyC");
                return RadarRunningCls::ReadyC;
            case 0x34:
                //std::cout << "ReadyD" << std::endl;
                //log4c->debug("................ReadyD");
                return RadarRunningCls::ReadyD;
            case 0x35:
                //std::cout << "ReadyE" << std::endl;
                //log4c->debug("................ReadyE");
                return RadarRunningCls::ReadyE;
            case 0x002a:
                //std::cout << "Completed Single" << std::endl;
                //log4c->debug("................Completed1");
                return RadarRunningCls::Completed;
            case 0x0021:
                //std::cout << "Completed Mult" << std::endl;
                //log4c->debug("................Completed2");
                return RadarRunningCls::Completed;
            default:
                //std::cout << "None,S4:" << S4 << std::endl;
                //log4c->debug("................None1");
                return RadarRunningCls::None;
            }
        case 0x0000:
            //std::cout << "None,S4:" << S4 << std::endl;
            //log4c->debug("................None2");
            return RadarRunningCls::None;
        default:
            //log4c->debug("................Scanning  S3:"+std::to_string(S3) + ";S4:" + std::to_string(S4)+";Header:" + std::to_string(Header.Value1) + ";Footer:" + std::to_string(Footer->Value1));
            return RadarRunningCls::Scanning;
        }
    }
};

#pragma pack(pop)
