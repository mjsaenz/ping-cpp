#include <link/desktop/abstract-link.h>
#include <ping-device-s500.h>
#include <ping-message-all.h>

#include "command-line/command-line.h"

#include "helper.h"

#include <cstdio>
#include <iostream>

/**
 * @brief Test s500 devices
 */
int main(int argc, char* argv[])
{
    if (!CommandLine::self()->run(argc, argv)) {
        return -1;
    }

    auto port = AbstractLink::openUrl(CommandLine::self()->connectionString);
    if (!port) {
        std::cerr << "Failed to open communication link with device" << std::endl;
        return -1;
    }
    S500 device = S500(*port.get());

    // Basic information
    PingDeviceTest::test("Device initialization", device.initialize(100));
    PingDeviceTest::test("Device firmware major version read", device.device_information.firmware_version_major > 0);
    PingDeviceTest::test("Ping protocol version",
        device.protocol_version.version_major >= 1);

    // Test control commands
    PingDeviceTest::test("Set speed of sound", device.set_speed_of_sound(1550000));

    // Set various ping parameters
    PingDeviceTest::test("Set ping parameters", device.set_ping_params(
        0,      // start_mm: Start of ping range, normally set to 0.
        5000,   // length_mm: Length of the returned profile.
        1,      // gain_index: Manual gain setting.
        200,    // msec_per_ping: Minimum ping interval.
        0,      // pulse_len_usec: Auto pulse duration.
        1223,   // report_id: Request a 'distance2' report.
        0,      // reserved: Set to 0.
        0,      // chirp: Monotone ping.
        0       // decimation: Auto range resolution.
    ));

    // Test request
    auto requestIDs = {
        S500Id::FW_VERSION,
        S500Id::SPEED_OF_SOUND,
        S500Id::RANGE,
        S500Id::PING_RATE_MSEC,
        S500Id::GAIN_INDEX,
        S500Id::PROCESSOR_DEGC,
        S500Id::ALTITUDE,
        S500Id::DISTANCE2,
        S500Id::PROFILE6_T,
    };

    for (auto id : requestIDs) {
        std::string title = "Requesting message [";
        title.append(PingHelper::nameFromMessageId(static_cast<PingEnumNamespace::PingMessageId>(id)));
        title.append("]");

        PingDeviceTest::test(title, device.request(id, 1000));
    }

    return 0;
}
