#include <Windows.h>
#include <stdint.h>

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

    RadarRunningState GetRunningState()
    {
        if (S3 == 0x55aa) {
            switch (S4) {
            case 0x0000:return Stopped;
            case 0xaa55:return Returning;
            case 0x0031:return ReadyA;
            case 0x0032:return ReadyB;
            case 0x0033:return ReadyC;
            case 0x0034:return ReadyD;
            case 0x0035:return ReadyE;
            case 0x002a:return Completed;
            case 0x0021:return Completed;
            default:return None;
            }
        }
        else {
            return Scanning;
        }
    }
};

#pragma pack(pop)
