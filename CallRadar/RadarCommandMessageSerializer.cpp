#pragma once

#include <stdexcept>
#include <cstdint>
#include <cstring>

enum class RadarCommandMessageType : uint16_t
{
	StartRadar = 0x7362,
	StopRadar = 0x6f70,
	DownloadGroupE = 0x59,
	ScanContinous = 0x6521,
	ScanSingle = 0x652a,
	SelectGroupASingle = 0x6531,
	SelectGroupBSingle = 0x6532,
	SelectGroupCSingle = 0x6533,
	SelectGroupDSingle = 0x6534,
	SelectGroupESingle = 0x6535,
	SelectGroupAContinous = 0x6541,
	SelectGroupBContinous = 0x6542,
	SelectGroupCContinous = 0x6543,
	SelectGroupDContinous = 0x6544,
	SelectGroupEContinous = 0x6545
};

struct RadarCommandMessageContentDownloadE
{
	uint16_t StartAngle;
	uint16_t EndAngle;
	uint8_t BufferPos;
	uint16_t Speed;
};

class RadarCommandMessage
{
public:
	RadarCommandMessage(RadarCommandMessageType type, void* content = nullptr, size_t contentSize = 0)
		: Type(type), Content(content), ContentSize(contentSize)
	{
	}
	RadarCommandMessageType Type;
	void* Content;
	size_t ContentSize;

	static RadarCommandMessage Start;
	static RadarCommandMessage Stop;
	static RadarCommandMessage ScanContinous;
	static RadarCommandMessage ScanSingle;
	static RadarCommandMessage SelectGroupESingle;

	static RadarCommandMessage DownloadGroupE(uint16_t startAngle, uint16_t endAngle, uint8_t bufferPos, uint16_t speed)
	{
		auto content = new RadarCommandMessageContentDownloadE{ startAngle, endAngle, bufferPos, speed };
		return RadarCommandMessage(RadarCommandMessageType::DownloadGroupE, content, sizeof(RadarCommandMessageContentDownloadE));
	}

	~RadarCommandMessage()
	{
		if (Content != nullptr)
		{
			delete[] static_cast<uint8_t*>(Content);
		}
	}
};

RadarCommandMessage RadarCommandMessage::Start = RadarCommandMessage(RadarCommandMessageType::StartRadar);
RadarCommandMessage RadarCommandMessage::Stop = RadarCommandMessage(RadarCommandMessageType::StopRadar);
RadarCommandMessage RadarCommandMessage::ScanContinous = RadarCommandMessage(RadarCommandMessageType::ScanContinous);
RadarCommandMessage RadarCommandMessage::ScanSingle = RadarCommandMessage(RadarCommandMessageType::ScanSingle);
RadarCommandMessage RadarCommandMessage::SelectGroupESingle = RadarCommandMessage(RadarCommandMessageType::SelectGroupESingle);

class RadarCommandMessageSerializer : public IMessageSerializer<RadarCommandMessage>
{
public:
	RadarCommandMessage Deserialize(const ReadOnlySequence<uint8_t>& buffer) override
	{
		throw std::logic_error("Not implemented");
	}
	void Serialize(const RadarCommandMessage& message, IBufferWriter<uint8_t>& writer) override
	{
		switch (message.Type)
		{
		case RadarCommandMessageType::StartRadar:
			WriteUInt16(writer, static_cast<uint16_t>(RadarCommandMessageType::StartRadar), false);
			break;
		case RadarCommandMessageType::StopRadar:
			WriteUInt16(writer, static_cast<uint16_t>(RadarCommandMessageType::StopRadar), false);
			break;
		case RadarCommandMessageType::DownloadGroupE:
		{
			auto content = static_cast<RadarCommandMessage
