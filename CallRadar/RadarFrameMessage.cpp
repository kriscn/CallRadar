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
    std::uint16_t Value1;
    std::uint16_t Value2;
    std::uint16_t Value3;
};

struct RadarFrameMessage {
    RadarFrameMessagePoint Header;
    RadarFrameMessagePoint Points[1000];

    std::uint16_t BinNubA;
    std::uint16_t BinNubB;
    std::uint16_t Spare1;

    std::int32_t Temperature;
    std::uint16_t Spare2;

    std::uint32_t Id;
    std::uint16_t Spare3;

    std::uint32_t Version;
    std::uint16_t Spare4;

    std::uint16_t Spare5;
    std::uint16_t Speed;
    std::uint16_t Spare6;

    std::uint16_t Spare7;
    std::uint16_t Arg;
    std::uint16_t Spare8;

    std::uint16_t ConfigId;
    std::uint16_t LP;
    std::uint16_t Spare9;

    std::uint16_t S1;
    std::uint16_t S2;
    std::uint16_t Spare10;

    std::uint16_t NSPD;
    std::uint16_t Type;
    std::uint16_t Spare11;

    std::uint16_t S3;
    std::uint16_t S4;
    std::uint16_t Spare12;

    RadarFrameMessagePoint SpareGroups[8];
    RadarFrameMessagePoint Tail;

    RadarRunningState GetRunningState() const {
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
