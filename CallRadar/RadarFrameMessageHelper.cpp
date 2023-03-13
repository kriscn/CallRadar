#include "main.h"

#define M_PI 3.14159265358979323846

class RadarFrameMessageHelper {
public:
    static void ExecuteTailFilter(const std::vector<RadarFrameMessage>& messages) {

    }

    static std::vector<CloudPoint> ConvertToPoints(const std::vector<RadarFrameMessage>& messages) {
        std::vector<CloudPoint> points;
        points.reserve(messages.size() * 1000);

        const double _radarDistance = 0.057;
        const double _distanceFactor = 0.001;
        const double _scanAngleFactor = 0.0625 * M_PI / 180;
        const double _pitchAngleFactor = M_PI * 2 / 65535;

        for (size_t i = 0; i < messages.size() / 3; i++) {
            const RadarFrameMessage& startMessage = messages[i * 3];
            const double pitchAngleStart = startMessage.S3;
            const double pitchAngleEnd = startMessage.S4;
            
            const double startPointAngle = startMessage.Points[sizeof(startMessage.Points)/6 - 1].Value2;
            const double factorPitch = (pitchAngleEnd - pitchAngleStart) /
                (double)(messages[i * 3 + 2].Points[sizeof(startMessage.Points)/6 - 1].Value2 - startPointAngle);

            /*auto fp = [&](const RadarFrameMessagePoint& p) {
                if (p.Value1 != 0) {
                    const double distance = p.Value1 * _distanceFactor;
                    const double scanAngle = p.Value2 * _scanAngleFactor;
                    const double pitchAngle = (pitchAngleStart + (scanAngle - startPointAngle) * factorPitch) * _pitchAngleFactor;
                    const double x0 = distance * sin(scanAngle);
                    const double pitchAngleSin = sin(pitchAngle);
                    const double pitchAngleCos = cos(pitchAngle);
                    points.emplace_back(CloudPoint{
                        (float)(_radarDistance * pitchAngleSin + x0 * pitchAngleCos),
                        (float)(-cos(scanAngle) * distance),
                        (float)(_radarDistance * pitchAngleCos - x0 * pitchAngleSin)
                        });
                }
            };

            for (const auto& p : startMessage.Points) {
                fp(p);
            }
            for (const auto& p : messages[i * 3 + 1].Points) {
                fp(p);
            }
            for (const auto& p : messages[i * 3 + 2].Points) {
                fp(p);
            }*/
        }

        return points;
    }
};