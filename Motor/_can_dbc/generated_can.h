/// DBC file: ../../snf.dbc    Self node: 'MOTOR'  (ALL = 0)
/// This file can be included by a source file, for example: #include "generated.h"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/// Extern function needed for dbc_encode_and_send()
extern bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8]);

/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} dbc_mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} dbc_msg_hdr_t; 

// static const dbc_msg_hdr_t BLE_COMM_CMD_HDR =                     {   10, 1 };
static const dbc_msg_hdr_t MASTER_SYSTEM_CMD_HDR =                {  100, 1 };
static const dbc_msg_hdr_t MASTER_MOTOR_CMD_HDR =                 {  151, 2 };
// static const dbc_msg_hdr_t MASTER_SYSTEM_STATUS_HDR =             {  162, 3 };
// static const dbc_msg_hdr_t SENSOR_ULTRASONIC_HDR =                {  211, 1 };
// static const dbc_msg_hdr_t SENSOR_HEARTBEAT_HDR =                 {  214, 4 };
// static const dbc_msg_hdr_t SENSOR_BATT_HDR =                      {  263, 1 };
// static const dbc_msg_hdr_t BLE_HEARTBEAT_HDR =                    {  314, 4 };
// static const dbc_msg_hdr_t BLE_CHCK_PT_HDR =                      {  341, 8 };
// static const dbc_msg_hdr_t GEO_DIRECTION_HDR =                    {  411, 1 };
// static const dbc_msg_hdr_t GEO_DEST_RCHD_HDR =                    {  413, 1 };
// static const dbc_msg_hdr_t GEO_HEARTBEAT_HDR =                    {  414, 4 };
// static const dbc_msg_hdr_t GEO_LOCATION_HDR =                     {  431, 8 };
// static const dbc_msg_hdr_t GEO_COMPASS_HDR =                      {  461, 2 };
static const dbc_msg_hdr_t MOTOR_HEARTBEAT_HDR =                  {  514, 4 };
static const dbc_msg_hdr_t MOTOR_SPEED_HDR =                      {  561, 2 };
// static const dbc_msg_hdr_t IO_HEARTBEAT_HDR =                     {  614, 4 };

/// Enumeration(s) for Message: 'MASTER_SYSTEM_CMD' from 'MASTER'
typedef enum {
    SYSTEM_STOP = 0,
    SYSTEM_START = 1,
} MASTER_SYSTEM_CMD_enum_E ;

/// Enumeration(s) for Message: 'MASTER_MOTOR_CMD' from 'MASTER'
typedef enum {
    STEER_REVERSE = 5,
    STEER_HALF_RIGHT = 3,
    STEER_STRAIGHT = 4,
    STEER_RIGHT = 2,
    STEER_LEFT = 0,
    STEER_HALF_LEFT = 1,
} MASTER_MOTOR_CMD_steer_E ;

typedef enum {
    STOP = 0,
    START = 1,
} MASTER_MOTOR_CMD_drive_E ;




/// Message: MASTER_SYSTEM_CMD from 'MASTER', DLC: 1 byte(s), MID: 100
typedef struct {
    MASTER_SYSTEM_CMD_enum_E MASTER_SYSTEM_CMD_enum; ///< B7:0   Destination: BLE,GEO,IO,MOTOR,SENSOR

    dbc_mia_info_t mia_info;
} MASTER_SYSTEM_CMD_t;


/// Message: MASTER_MOTOR_CMD from 'MASTER', DLC: 2 byte(s), MID: 151
typedef struct {
    MASTER_MOTOR_CMD_steer_E MASTER_MOTOR_CMD_steer; ///< B7:0   Destination: MOTOR,IO
    MASTER_MOTOR_CMD_drive_E MASTER_MOTOR_CMD_drive; ///< B15:8   Destination: MOTOR,IO

    dbc_mia_info_t mia_info;
} MASTER_MOTOR_CMD_t;


/// Message: MOTOR_HEARTBEAT from 'MOTOR', DLC: 4 byte(s), MID: 514
typedef struct {
    uint16_t MOTOR_HEARTBEAT_rx_bytes;        ///< B15:0   Destination: MASTER
    uint16_t MOTOR_HEARTBEAT_tx_bytes;        ///< B31:16   Destination: MASTER

    // No dbc_mia_info_t for a message that we will send
} MOTOR_HEARTBEAT_t;


/// Message: MOTOR_SPEED from 'MOTOR', DLC: 2 byte(s), MID: 561
typedef struct {
    float MOTOR_SPEED_actual;                 ///< B13:0  Min: 0 Max: 100.00   Destination: IO,BLE

    // No dbc_mia_info_t for a message that we will send
} MOTOR_SPEED_t;


/// @{ These 'externs' need to be defined in a source file of your project
extern const uint32_t                             MASTER_SYSTEM_CMD__MIA_MS;
extern const MASTER_SYSTEM_CMD_t                  MASTER_SYSTEM_CMD__MIA_MSG;
extern const uint32_t                             MASTER_MOTOR_CMD__MIA_MS;
extern const MASTER_MOTOR_CMD_t                   MASTER_MOTOR_CMD__MIA_MSG;
/// @}


/// Not generating code for dbc_encode_BLE_COMM_CMD() since the sender is BLE and we are MOTOR

/// Not generating code for dbc_encode_MASTER_SYSTEM_CMD() since the sender is MASTER and we are MOTOR

/// Not generating code for dbc_encode_MASTER_MOTOR_CMD() since the sender is MASTER and we are MOTOR

/// Not generating code for dbc_encode_MASTER_SYSTEM_STATUS() since the sender is MASTER and we are MOTOR

/// Not generating code for dbc_encode_SENSOR_ULTRASONIC() since the sender is SENSOR and we are MOTOR

/// Not generating code for dbc_encode_SENSOR_HEARTBEAT() since the sender is SENSOR and we are MOTOR

/// Not generating code for dbc_encode_SENSOR_BATT() since the sender is SENSOR and we are MOTOR

/// Not generating code for dbc_encode_BLE_HEARTBEAT() since the sender is BLE and we are MOTOR

/// Not generating code for dbc_encode_BLE_CHCK_PT() since the sender is BLE and we are MOTOR

/// Not generating code for dbc_encode_GEO_DIRECTION() since the sender is GEO and we are MOTOR

/// Not generating code for dbc_encode_GEO_DEST_RCHD() since the sender is GEO and we are MOTOR

/// Not generating code for dbc_encode_GEO_HEARTBEAT() since the sender is GEO and we are MOTOR

/// Not generating code for dbc_encode_GEO_LOCATION() since the sender is GEO and we are MOTOR

/// Not generating code for dbc_encode_GEO_COMPASS() since the sender is GEO and we are MOTOR

/// Encode MOTOR's 'MOTOR_HEARTBEAT' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_HEARTBEAT(uint8_t bytes[8], MOTOR_HEARTBEAT_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->MOTOR_HEARTBEAT_rx_bytes)))) & 0xffff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B8

    raw = ((uint32_t)(((from->MOTOR_HEARTBEAT_tx_bytes)))) & 0xffff;
    bytes[2] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B24

    return MOTOR_HEARTBEAT_HDR;
}

/// Encode and send for dbc_encode_MOTOR_HEARTBEAT() message
static inline bool dbc_encode_and_send_MOTOR_HEARTBEAT(MOTOR_HEARTBEAT_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_HEARTBEAT(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode MOTOR's 'MOTOR_SPEED' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_SPEED(uint8_t bytes[8], MOTOR_SPEED_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    if(from->MOTOR_SPEED_actual < 0) { from->MOTOR_SPEED_actual = 0; } // Min value: 0
    if(from->MOTOR_SPEED_actual > 100.00) { from->MOTOR_SPEED_actual = 100.00; } // Max value: 100.00
    raw = ((uint32_t)(((from->MOTOR_SPEED_actual) / 0.01) + 0.5)) & 0x3fff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0x3f)); ///< 6 bit(s) starting from B8

    return MOTOR_SPEED_HDR;
}

/// Encode and send for dbc_encode_MOTOR_SPEED() message
static inline bool dbc_encode_and_send_MOTOR_SPEED(MOTOR_SPEED_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_SPEED(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Not generating code for dbc_encode_IO_HEARTBEAT() since the sender is IO and we are MOTOR

/// Not generating code for dbc_decode_BLE_COMM_CMD() since 'MOTOR' is not the recipient of any of the signals

/// Decode MASTER's 'MASTER_SYSTEM_CMD' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MASTER_SYSTEM_CMD(MASTER_SYSTEM_CMD_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MASTER_SYSTEM_CMD_HDR.dlc || hdr->mid != MASTER_SYSTEM_CMD_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    to->MASTER_SYSTEM_CMD_enum = (MASTER_SYSTEM_CMD_enum_E)((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Decode MASTER's 'MASTER_MOTOR_CMD' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MASTER_MOTOR_CMD(MASTER_MOTOR_CMD_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MASTER_MOTOR_CMD_HDR.dlc || hdr->mid != MASTER_MOTOR_CMD_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    to->MASTER_MOTOR_CMD_steer = (MASTER_MOTOR_CMD_steer_E)((raw));
    raw  = ((uint32_t)((bytes[1]))); ///< 8 bit(s) from B8
    to->MASTER_MOTOR_CMD_drive = (MASTER_MOTOR_CMD_drive_E)((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Not generating code for dbc_decode_MASTER_SYSTEM_STATUS() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_ULTRASONIC() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_HEARTBEAT() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_BATT() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_HEARTBEAT() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_CHCK_PT() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_DIRECTION() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_DEST_RCHD() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_HEARTBEAT() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_LOCATION() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_COMPASS() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_HEARTBEAT() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_SPEED() since 'MOTOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_IO_HEARTBEAT() since 'MOTOR' is not the recipient of any of the signals

/// Handle the MIA for MASTER's MASTER_SYSTEM_CMD message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MASTER_SYSTEM_CMD(MASTER_SYSTEM_CMD_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MASTER_SYSTEM_CMD__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MASTER_SYSTEM_CMD__MIA_MSG;
        msg->mia_info.mia_counter_ms = MASTER_SYSTEM_CMD__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's MASTER_MOTOR_CMD message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MASTER_MOTOR_CMD(MASTER_MOTOR_CMD_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MASTER_MOTOR_CMD__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MASTER_MOTOR_CMD__MIA_MSG;
        msg->mia_info.mia_counter_ms = MASTER_MOTOR_CMD__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

#endif
