#include <cstdint>

enum class RadarRunningState {
    None,
    Stopped,
    Backing,
    Completed,
    ReadyA,
    ReadyB,
    ReadyC,
    ReadyD,
    ReadyE,
    Scanning
};

#pragma pack(push, 1)
struct RadarFrameMessagePoint {
    uint16_t Value1;
    uint16_t Value2;
    uint16_t Value3;
};

struct RadarFrameMessage {
    RadarFrameMessagePoint Header;
    RadarFrameMessagePoint Points[1000];
    uint16_t BinNubA;
    uint16_t BinNubB;
    uint16_t Spare1;
    int32_t Temperature;
    uint16_t Spare2;
    uint32_t Id;
    uint16_t Spare3;
    uint32_t Version;
    uint16_t Spare4;
    uint16_t Spare5;
    uint16_t Speed;
    uint16_t Spare6;
    uint16_t Spare7;
    uint16_t Arg;
    uint16_t Spare8;
    uint16_t ConfigId;
    uint16_t LP;
    uint16_t Spare9;
    uint16_t S1;
    uint16_t S2;
    uint16_t Spare10;
    uint16_t NSPD;
    uint16_t Type;
    uint16_t Spare11;
    uint16_t S3;
    uint16_t S4;
    uint16_t Spare12;
    RadarFrameMessagePoint SpareGroups[8];
    RadarFrameMessagePoint Tail;

    RadarRunningState GetRunningState() {
        if (S3 == 0x55aa) {
            if (S4 == 0x0000) return RadarRunningState::Stopped;
            else if (S4 == 0xaa55) return RadarRunningState::Backing;
            else if (S4 == 0x002a || S4 == 0x0021) return RadarRunningState::Completed;
            else {
                switch (S4) {
                case 0x31: return RadarRunningState::ReadyA;
                case 0x32: return RadarRunningState::ReadyB;
                case 0x33: return RadarRunningState::ReadyC;
                case 0x34: return RadarRunningState::ReadyD;
                case 0x35: return RadarRunningState::ReadyE;
                default: return RadarRunningState::None;
                }
            }
        }
        else {
            return RadarRunningState::Scanning;
        }
    }
};
#pragma pack(pop)
