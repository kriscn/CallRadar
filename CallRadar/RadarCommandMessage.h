#pragma once

#include <cstdint>

enum class RadarCommandMessageType {
    StartRadar,
    StopRadar,
    ScanContinous,
    ScanSingle,
    SelectGroupESingle,
    DownloadGroupE
};

struct RadarCommandMessageContentDownloadE {
    std::uint16_t StartAngle;
    std::uint16_t EndAngle;
    std::uint8_t BufferPos;
    std::uint16_t Speed;
};

struct RadarCommandMessageContent {
    RadarCommandMessageContentDownloadE DownloadE;
};

class RadarCommandMessage {
public:
    RadarCommandMessage(RadarCommandMessageType type, RadarCommandMessageContent content = RadarCommandMessageContent())
        : Type(type), Content(content)
    {}

    RadarCommandMessageType Type;
    RadarCommandMessageContent Content;

    static const RadarCommandMessage Start;
    static const RadarCommandMessage Stop;
    static const RadarCommandMessage ScanContinous;
    static const RadarCommandMessage ScanSingle;
    static const RadarCommandMessage SelectGroupESingle;

    static RadarCommandMessage DownloadGroupE(std::uint16_t startAngle, std::uint16_t endAngle, std::uint8_t bufferPos, std::uint16_t speed) {
        RadarCommandMessageContentDownloadE content{ startAngle, endAngle, bufferPos, speed };
        return RadarCommandMessage(RadarCommandMessageType::DownloadGroupE, RadarCommandMessageContent{ content });
    }
};

const RadarCommandMessage RadarCommandMessage::Start(RadarCommandMessageType::StartRadar);
const RadarCommandMessage RadarCommandMessage::Stop(RadarCommandMessageType::StopRadar);
const RadarCommandMessage RadarCommandMessage::ScanContinous(RadarCommandMessageType::ScanContinous);
const RadarCommandMessage RadarCommandMessage::ScanSingle(RadarCommandMessageType::ScanSingle);
const RadarCommandMessage RadarCommandMessage::SelectGroupESingle(RadarCommandMessageType::SelectGroupESingle);
