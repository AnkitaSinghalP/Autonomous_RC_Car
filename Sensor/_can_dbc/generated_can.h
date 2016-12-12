/// DBC file: ../../snf.dbc    Self node: 'SENSOR'  (ALL = 0)
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
// static const dbc_msg_hdr_t MASTER_MOTOR_CMD_HDR =                 {  151, 3 };
// static const dbc_msg_hdr_t MASTER_SYSTEM_STATUS_HDR =             {  162, 3 };
static const dbc_msg_hdr_t SENSOR_ULTRASONIC_HDR =                {  211, 1 };
static const dbc_msg_hdr_t SENSOR_BATT_HDR =                      {  213, 1 };
static const dbc_msg_hdr_t SENSOR_HEARTBEAT_HDR =                 {  214, 4 };
// static const dbc_msg_hdr_t BLE_CHCK_PT_HDR =                      {  311, 8 };
// static const dbc_msg_hdr_t BLE_HEARTBEAT_HDR =                    {  314, 4 };
// static const dbc_msg_hdr_t BLE_MAP_START_DATA_HDR =               {  361, 8 };
// static const dbc_msg_hdr_t BLE_MAP_DEST_DATA_HDR =                {  362, 8 };
// static const dbc_msg_hdr_t GEO_DIRECTION_HDR =                    {  411, 1 };
// static const dbc_msg_hdr_t GEO_DEST_RCHD_HDR =                    {  413, 1 };
// static const dbc_msg_hdr_t GEO_HEARTBEAT_HDR =                    {  414, 4 };
// static const dbc_msg_hdr_t GEO_LOCATION_HDR =                     {  421, 8 };
// static const dbc_msg_hdr_t GEO_COMPASS_HDR =                      {  461, 2 };
// static const dbc_msg_hdr_t MOTOR_HEARTBEAT_HDR =                  {  514, 4 };
// static const dbc_msg_hdr_t MOTOR_SPEED_HDR =                      {  561, 2 };
// static const dbc_msg_hdr_t IO_HEARTBEAT_HDR =                     {  614, 4 };

/// Enumeration(s) for Message: 'MASTER_SYSTEM_CMD' from 'MASTER'
typedef enum {
    SYSTEM_START = 1,
    SYSTEM_RESET = 2,
    SYSTEM_STOP = 0,
} MASTER_SYSTEM_CMD_enum_E ;




/// Message: MASTER_SYSTEM_CMD from 'MASTER', DLC: 1 byte(s), MID: 100
typedef struct {
    MASTER_SYSTEM_CMD_enum_E MASTER_SYSTEM_CMD_enum; ///< B7:0   Destination: BLE,GEO,IO,MOTOR,SENSOR

    dbc_mia_info_t mia_info;
} MASTER_SYSTEM_CMD_t;


/// Message: SENSOR_ULTRASONIC from 'SENSOR', DLC: 1 byte(s), MID: 211
typedef struct {
    uint8_t SENSOR_ULTRASONIC_left : 1;       ///< B0:0   Destination: MASTER,IO
    uint8_t SENSOR_ULTRASONIC_middle : 1;     ///< B1:1   Destination: MASTER,IO
    uint8_t SENSOR_ULTRASONIC_right : 1;      ///< B2:2   Destination: MASTER,IO
    uint8_t SENSOR_ULTRASONIC_rear : 1;       ///< B3:3   Destination: MASTER,IO
    uint8_t SENSOR_ULTRASONIC_critical : 1;   ///< B4:4   Destination: MASTER,IO

    // No dbc_mia_info_t for a message that we will send
} SENSOR_ULTRASONIC_t;


/// Message: SENSOR_BATT from 'SENSOR', DLC: 1 byte(s), MID: 213
typedef struct {
    uint8_t SENSOR_BATT_stat;                 ///< B6:0  Min: 0 Max: 100   Destination: MASTER,IO

    // No dbc_mia_info_t for a message that we will send
} SENSOR_BATT_t;


/// Message: SENSOR_HEARTBEAT from 'SENSOR', DLC: 4 byte(s), MID: 214
typedef struct {
    uint16_t SENSOR_HEARTBEAT_rx_bytes;       ///< B15:0   Destination: MASTER
    uint16_t SENSOR_HEARTBEAT_tx_bytes;       ///< B31:16   Destination: MASTER

    // No dbc_mia_info_t for a message that we will send
} SENSOR_HEARTBEAT_t;


/// @{ These 'externs' need to be defined in a source file of your project
extern const uint32_t                             MASTER_SYSTEM_CMD__MIA_MS;
extern const MASTER_SYSTEM_CMD_t                  MASTER_SYSTEM_CMD__MIA_MSG;
/// @}


/// Not generating code for dbc_encode_BLE_COMM_CMD() since the sender is BLE and we are SENSOR

/// Not generating code for dbc_encode_MASTER_SYSTEM_CMD() since the sender is MASTER and we are SENSOR

/// Not generating code for dbc_encode_MASTER_MOTOR_CMD() since the sender is MASTER and we are SENSOR

/// Not generating code for dbc_encode_MASTER_SYSTEM_STATUS() since the sender is MASTER and we are SENSOR

/// Encode SENSOR's 'SENSOR_ULTRASONIC' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_SENSOR_ULTRASONIC(uint8_t bytes[8], SENSOR_ULTRASONIC_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->SENSOR_ULTRASONIC_left)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01)); ///< 1 bit(s) starting from B0

    raw = ((uint32_t)(((from->SENSOR_ULTRASONIC_middle)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01) << 1); ///< 1 bit(s) starting from B1

    raw = ((uint32_t)(((from->SENSOR_ULTRASONIC_right)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01) << 2); ///< 1 bit(s) starting from B2

    raw = ((uint32_t)(((from->SENSOR_ULTRASONIC_rear)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01) << 3); ///< 1 bit(s) starting from B3

    raw = ((uint32_t)(((from->SENSOR_ULTRASONIC_critical)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01) << 4); ///< 1 bit(s) starting from B4

    return SENSOR_ULTRASONIC_HDR;
}

/// Encode and send for dbc_encode_SENSOR_ULTRASONIC() message
static inline bool dbc_encode_and_send_SENSOR_ULTRASONIC(SENSOR_ULTRASONIC_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_SENSOR_ULTRASONIC(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode SENSOR's 'SENSOR_BATT' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_SENSOR_BATT(uint8_t bytes[8], SENSOR_BATT_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    // Not doing min value check since the signal is unsigned already
    if(from->SENSOR_BATT_stat > 100) { from->SENSOR_BATT_stat = 100; } // Max value: 100
    raw = ((uint32_t)(((from->SENSOR_BATT_stat)))) & 0x7f;
    bytes[0] |= (((uint8_t)(raw) & 0x7f)); ///< 7 bit(s) starting from B0

    return SENSOR_BATT_HDR;
}

/// Encode and send for dbc_encode_SENSOR_BATT() message
static inline bool dbc_encode_and_send_SENSOR_BATT(SENSOR_BATT_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_SENSOR_BATT(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode SENSOR's 'SENSOR_HEARTBEAT' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_SENSOR_HEARTBEAT(uint8_t bytes[8], SENSOR_HEARTBEAT_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->SENSOR_HEARTBEAT_rx_bytes)))) & 0xffff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B8

    raw = ((uint32_t)(((from->SENSOR_HEARTBEAT_tx_bytes)))) & 0xffff;
    bytes[2] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B24

    return SENSOR_HEARTBEAT_HDR;
}

/// Encode and send for dbc_encode_SENSOR_HEARTBEAT() message
static inline bool dbc_encode_and_send_SENSOR_HEARTBEAT(SENSOR_HEARTBEAT_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_SENSOR_HEARTBEAT(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Not generating code for dbc_encode_BLE_CHCK_PT() since the sender is BLE and we are SENSOR

/// Not generating code for dbc_encode_BLE_HEARTBEAT() since the sender is BLE and we are SENSOR

/// Not generating code for dbc_encode_BLE_MAP_START_DATA() since the sender is BLE and we are SENSOR

/// Not generating code for dbc_encode_BLE_MAP_DEST_DATA() since the sender is BLE and we are SENSOR

/// Not generating code for dbc_encode_GEO_DIRECTION() since the sender is GEO and we are SENSOR

/// Not generating code for dbc_encode_GEO_DEST_RCHD() since the sender is GEO and we are SENSOR

/// Not generating code for dbc_encode_GEO_HEARTBEAT() since the sender is GEO and we are SENSOR

/// Not generating code for dbc_encode_GEO_LOCATION() since the sender is GEO and we are SENSOR

/// Not generating code for dbc_encode_GEO_COMPASS() since the sender is GEO and we are SENSOR

/// Not generating code for dbc_encode_MOTOR_HEARTBEAT() since the sender is MOTOR and we are SENSOR

/// Not generating code for dbc_encode_MOTOR_SPEED() since the sender is MOTOR and we are SENSOR

/// Not generating code for dbc_encode_IO_HEARTBEAT() since the sender is IO and we are SENSOR

/// Not generating code for dbc_decode_BLE_COMM_CMD() since 'SENSOR' is not the recipient of any of the signals

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


/// Not generating code for dbc_decode_MASTER_MOTOR_CMD() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MASTER_SYSTEM_STATUS() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_ULTRASONIC() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_BATT() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_HEARTBEAT() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_CHCK_PT() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_HEARTBEAT() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_MAP_START_DATA() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_MAP_DEST_DATA() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_DIRECTION() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_DEST_RCHD() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_HEARTBEAT() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_LOCATION() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_COMPASS() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_HEARTBEAT() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_SPEED() since 'SENSOR' is not the recipient of any of the signals

/// Not generating code for dbc_decode_IO_HEARTBEAT() since 'SENSOR' is not the recipient of any of the signals

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

#endif
