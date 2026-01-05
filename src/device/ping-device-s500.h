#pragma once

#include "ping-device.h"

class S500 : public PingDevice
{
public:
    /**
     *  @brief Constructor
     *
     *  @param ser: The device I/O
     */
    S500(PingPort& port) : PingDevice(port) {}

    /**
     * @brief Destructor
     */
    ~S500();

    /**
     *  @brief Establish communications with the device, and initialize the update interval
     *
     *  @param pingIntervalMs: The interval (in milliseconds) between acoustic measurements
     *
     *  @return true if the device was initialized successfully
     */
    bool initialize();

    /**
     * @brief Set parameters
     *
     * @param start_mm - Units: mm; Start of ping range, normally set to 0.
     * @param length_mm - Units: mm; Length of the returned profile. End of range = start_mm + length_mm. Set to 0 for auto_range.
     * @param gain_index - Set to -1 for auto gain, otherwise 0-13 sets gain for manual gain.
     * @param msec_per_ping - Units: msec; Set to -1 to start a single ping. Otherwise sets minimum ping interval.
     * @param pulse_len_usec - 0 for auto mode. Currently ignored and auto duration is always used.
     * @param report_id - The ID of the packet type that you would like in response. Options are distance2 (1223), profile6 (1308), or zero. Zero disables pinging.
     * @param reserved - Set to 0
     * @param chirp - Set to 1 for chirp, 0 for monotone ping.
     * @param decimation - Set to 0 for auto range resolution in chirp mode
     *
     * @return when verify = false: true if a valid reply is received from the device.
     * @return when verify = true: true if a valid reply is received from the
     * device, and the values in the reply match the values that we applied
     */
    bool set_ping_params(uint32_t start_mm, uint32_t length_mm, int16_t gain_index, int16_t msec_per_ping, uint16_t pulse_len_usec, uint16_t report_id, uint16_t reserved, uint8_t chirp, uint8_t decimation, bool verify = true);

    /**
     * @brief Set the speed of sound
     *
     * @param sos_mm_per_sec - Units: mm/s; The speed of sound in the measurement medium, ~1,500,000 mm/s for water.
     *
     * @return when verify = false: true if a valid reply is received from the device.
     * @return when verify = true: true if a valid reply is received from the
     * device, and the values in the reply match the values that we applied
     */
    bool set_speed_of_sound(uint32_t sos_mm_per_sec, bool verify = true);


    //! Set parameters
    struct {
        // Start of ping range, normally set to 0.
        uint32_t start_mm;
        // Length of the returned profile. End of range = start_mm + length_mm. Set to 0 for auto_range.
        uint32_t length_mm;
        // Set to -1 for auto gain, otherwise 0-13 sets gain for manual gain.
        int16_t gain_index;
        // Set to -1 to start a single ping. Otherwise sets minimum ping interval.
        int16_t msec_per_ping;
        // 0 for auto mode. Currently ignored and auto duration is always used.
        uint16_t pulse_len_usec;
        // The ID of the packet type that you would like in response. Options are distance2 (1223), profile6 (1308), or zero. Zero disables pinging.
        uint16_t report_id;
        // Set to 0
        uint16_t reserved;
        // Set to 1 for chirp, 0 for monotone ping.
        uint8_t chirp;
        // Set to 0 for auto range resolution in chirp mode
        uint8_t decimation;
    } set_ping_params_data;

    //! Set the speed of sound
    struct {
        // The speed of sound in the measurement medium, ~1,500,000 mm/s for water.
        uint32_t sos_mm_per_sec;
    } set_speed_of_sound_data;


private:
    /**
     *  @brief Handle an incoming message from the device. Internal values are updated according to the device data.
     *
     *  @param message: A pointer to the message received from the device
     */
    void _handleMessage(const ping_message* message) override;
};