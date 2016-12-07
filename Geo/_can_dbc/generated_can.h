/// DBC file: ../../snf.dbc    Self node: 'GEO'  (ALL = 0)
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
// static const dbc_msg_hdr_t SENSOR_ULTRASONIC_HDR =                {  211, 1 };
// static const dbc_msg_hdr_t SENSOR_BATT_HDR =                      {  213, 1 };
// static const dbc_msg_hdr_t SENSOR_HEARTBEAT_HDR =                 {  214, 4 };
static const dbc_msg_hdr_t BLE_CHCK_PT_HDR =                      {  311, 8 };
// static const dbc_msg_hdr_t BLE_HEARTBEAT_HDR =                    {  314, 4 };
// static const dbc_msg_hdr_t BLE_MAP_START_DATA_HDR =               {  361, 8 };
// static const dbc_msg_hdr_t BLE_MAP_DEST_DATA_HDR =                {  362, 8 };
static const dbc_msg_hdr_t GEO_DIRECTION_HDR =                    {  411, 1 };
static const dbc_msg_hdr_t GEO_DEST_RCHD_HDR =                    {  413, 1 };
static const dbc_msg_hdr_t GEO_HEARTBEAT_HDR =                    {  414, 4 };
static const dbc_msg_hdr_t GEO_LOCATION_HDR =                     {  421, 8 };
static const dbc_msg_hdr_t GEO_COMPASS_HDR =                      {  461, 8 };
// static const dbc_msg_hdr_t MOTOR_HEARTBEAT_HDR =                  {  514, 4 };
// static const dbc_msg_hdr_t MOTOR_SPEED_HDR =                      {  561, 2 };
// static const dbc_msg_hdr_t IO_HEARTBEAT_HDR =                     {  614, 4 };

/// Enumeration(s) for Message: 'MASTER_SYSTEM_CMD' from 'MASTER'
typedef enum {
    SYSTEM_STOP = 0,
    SYSTEM_RESET = 2,
    SYSTEM_START = 1,
} MASTER_SYSTEM_CMD_enum_E ;




/// Message: MASTER_SYSTEM_CMD from 'MASTER', DLC: 1 byte(s), MID: 100
typedef struct {
    MASTER_SYSTEM_CMD_enum_E MASTER_SYSTEM_CMD_enum; ///< B7:0   Destination: BLE,GEO,IO,MOTOR,SENSOR

    dbc_mia_info_t mia_info;
} MASTER_SYSTEM_CMD_t;


/// Message: BLE_CHCK_PT from 'BLE', DLC: 8 byte(s), MID: 311
typedef struct {
    float BLE_CHCK_PT_lat;                    ///< B31:0   Destination: GEO
    float BLE_CHCK_PT_long;                   ///< B63:32   Destination: GEO

    dbc_mia_info_t mia_info;
} BLE_CHCK_PT_t;


/// Message: GEO_DIRECTION from 'GEO', DLC: 1 byte(s), MID: 411
typedef struct {
    uint8_t GEO_DIRECTION_data;               ///< B7:0   Destination: MASTER

    // No dbc_mia_info_t for a message that we will send
} GEO_DIRECTION_t;


/// Message: GEO_DEST_RCHD from 'GEO', DLC: 1 byte(s), MID: 413
typedef struct {
    uint8_t GEO_DEST_RCHD_stat : 1;           ///< B0:0  Min: 0 Max: 1   Destination: MASTER,IO

    // No dbc_mia_info_t for a message that we will send
} GEO_DEST_RCHD_t;


/// Message: GEO_HEARTBEAT from 'GEO', DLC: 4 byte(s), MID: 414
typedef struct {
    uint16_t GEO_HEARTBEAT_rx_bytes;          ///< B15:0   Destination: MASTER
    uint16_t GEO_HEARTBEAT_tx_bytes;          ///< B31:16   Destination: MASTER

    // No dbc_mia_info_t for a message that we will send
} GEO_HEARTBEAT_t;


/// Message: GEO_LOCATION from 'GEO', DLC: 8 byte(s), MID: 421
typedef struct {
    float GEO_LOCATION_lat;                   ///< B31:0   Destination: IO,BLE
    float GEO_LOCATION_long;                  ///< B63:32   Destination: IO,BLE

    // No dbc_mia_info_t for a message that we will send
} GEO_LOCATION_t;


/// Message: GEO_COMPASS from 'GEO', DLC: 8 byte(s), MID: 461
typedef struct {
    uint16_t GEO_COMPASS_mag;                 ///< B8:0   Destination: IO

    // No dbc_mia_info_t for a message that we will send
} GEO_COMPASS_t;


/// @{ These 'externs' need to be defined in a source file of your project
extern const uint32_t                             MASTER_SYSTEM_CMD__MIA_MS;
extern const MASTER_SYSTEM_CMD_t                  MASTER_SYSTEM_CMD__MIA_MSG;
extern const uint32_t                             BLE_CHCK_PT__MIA_MS;
extern const BLE_CHCK_PT_t                        BLE_CHCK_PT__MIA_MSG;
/// @}


/// Not generating code for dbc_encode_BLE_COMM_CMD() since the sender is BLE and we are GEO

/// Not generating code for dbc_encode_MASTER_SYSTEM_CMD() since the sender is MASTER and we are GEO

/// Not generating code for dbc_encode_MASTER_MOTOR_CMD() since the sender is MASTER and we are GEO

/// Not generating code for dbc_encode_MASTER_SYSTEM_STATUS() since the sender is MASTER and we are GEO

/// Not generating code for dbc_encode_SENSOR_ULTRASONIC() since the sender is SENSOR and we are GEO

/// Not generating code for dbc_encode_SENSOR_BATT() since the sender is SENSOR and we are GEO

/// Not generating code for dbc_encode_SENSOR_HEARTBEAT() since the sender is SENSOR and we are GEO

/// Not generating code for dbc_encode_BLE_CHCK_PT() since the sender is BLE and we are GEO

/// Not generating code for dbc_encode_BLE_HEARTBEAT() since the sender is BLE and we are GEO

/// Not generating code for dbc_encode_BLE_MAP_START_DATA() since the sender is BLE and we are GEO

/// Not generating code for dbc_encode_BLE_MAP_DEST_DATA() since the sender is BLE and we are GEO

/// Encode GEO's 'GEO_DIRECTION' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_DIRECTION(uint8_t bytes[8], GEO_DIRECTION_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->GEO_DIRECTION_data)))) & 0xff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0

    return GEO_DIRECTION_HDR;
}

/// Encode and send for dbc_encode_GEO_DIRECTION() message
static inline bool dbc_encode_and_send_GEO_DIRECTION(GEO_DIRECTION_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_DIRECTION(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO's 'GEO_DEST_RCHD' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_DEST_RCHD(uint8_t bytes[8], GEO_DEST_RCHD_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    // Not doing min value check since the signal is unsigned already
    if(from->GEO_DEST_RCHD_stat > 1) { from->GEO_DEST_RCHD_stat = 1; } // Max value: 1
    raw = ((uint32_t)(((from->GEO_DEST_RCHD_stat)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01)); ///< 1 bit(s) starting from B0

    return GEO_DEST_RCHD_HDR;
}

/// Encode and send for dbc_encode_GEO_DEST_RCHD() message
static inline bool dbc_encode_and_send_GEO_DEST_RCHD(GEO_DEST_RCHD_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_DEST_RCHD(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO's 'GEO_HEARTBEAT' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_HEARTBEAT(uint8_t bytes[8], GEO_HEARTBEAT_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->GEO_HEARTBEAT_rx_bytes)))) & 0xffff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B8

    raw = ((uint32_t)(((from->GEO_HEARTBEAT_tx_bytes)))) & 0xffff;
    bytes[2] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B24

    return GEO_HEARTBEAT_HDR;
}

/// Encode and send for dbc_encode_GEO_HEARTBEAT() message
static inline bool dbc_encode_and_send_GEO_HEARTBEAT(GEO_HEARTBEAT_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_HEARTBEAT(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO's 'GEO_LOCATION' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_LOCATION(uint8_t bytes[8], GEO_LOCATION_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->GEO_LOCATION_lat) / 1e-06) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B8
    bytes[2] |= (((uint8_t)(raw >> 16) & 0xff)); ///< 8 bit(s) starting from B16
    bytes[3] |= (((uint8_t)(raw >> 24) & 0xff)); ///< 8 bit(s) starting from B24

    raw = ((uint32_t)(((from->GEO_LOCATION_long) / 1e-06) + 0.5)) & 0xffffffff;
    bytes[4] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B32
    bytes[5] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B40
    bytes[6] |= (((uint8_t)(raw >> 16) & 0xff)); ///< 8 bit(s) starting from B48
    bytes[7] |= (((uint8_t)(raw >> 24) & 0xff)); ///< 8 bit(s) starting from B56

    return GEO_LOCATION_HDR;
}

/// Encode and send for dbc_encode_GEO_LOCATION() message
static inline bool dbc_encode_and_send_GEO_LOCATION(GEO_LOCATION_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_LOCATION(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode GEO's 'GEO_COMPASS' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_GEO_COMPASS(uint8_t bytes[8], GEO_COMPASS_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->GEO_COMPASS_mag)))) & 0x1ff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0
    bytes[1] |= (((uint8_t)(raw >> 8) & 0x01)); ///< 1 bit(s) starting from B8

    return GEO_COMPASS_HDR;
}

/// Encode and send for dbc_encode_GEO_COMPASS() message
static inline bool dbc_encode_and_send_GEO_COMPASS(GEO_COMPASS_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_GEO_COMPASS(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Not generating code for dbc_encode_MOTOR_HEARTBEAT() since the sender is MOTOR and we are GEO

/// Not generating code for dbc_encode_MOTOR_SPEED() since the sender is MOTOR and we are GEO

/// Not generating code for dbc_encode_IO_HEARTBEAT() since the sender is IO and we are GEO

/// Not generating code for dbc_decode_BLE_COMM_CMD() since 'GEO' is not the recipient of any of the signals

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


/// Not generating code for dbc_decode_MASTER_MOTOR_CMD() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MASTER_SYSTEM_STATUS() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_ULTRASONIC() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_BATT() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_HEARTBEAT() since 'GEO' is not the recipient of any of the signals

/// Decode BLE's 'BLE_CHCK_PT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_BLE_CHCK_PT(BLE_CHCK_PT_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != BLE_CHCK_PT_HDR.dlc || hdr->mid != BLE_CHCK_PT_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    raw |= ((uint32_t)((bytes[1]))) << 8; ///< 8 bit(s) from B8
    raw |= ((uint32_t)((bytes[2]))) << 16; ///< 8 bit(s) from B16
    raw |= ((uint32_t)((bytes[3]))) << 24; ///< 8 bit(s) from B24
    to->BLE_CHCK_PT_lat = ((raw * 1e-06));
    raw  = ((uint32_t)((bytes[4]))); ///< 8 bit(s) from B32
    raw |= ((uint32_t)((bytes[5]))) << 8; ///< 8 bit(s) from B40
    raw |= ((uint32_t)((bytes[6]))) << 16; ///< 8 bit(s) from B48
    raw |= ((uint32_t)((bytes[7]))) << 24; ///< 8 bit(s) from B56
    to->BLE_CHCK_PT_long = ((raw * 1e-06));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Not generating code for dbc_decode_BLE_HEARTBEAT() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_MAP_START_DATA() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLE_MAP_DEST_DATA() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_DIRECTION() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_DEST_RCHD() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_HEARTBEAT() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_LOCATION() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GEO_COMPASS() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_HEARTBEAT() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_SPEED() since 'GEO' is not the recipient of any of the signals

/// Not generating code for dbc_decode_IO_HEARTBEAT() since 'GEO' is not the recipient of any of the signals

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

/// Handle the MIA for BLE's BLE_CHCK_PT message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_BLE_CHCK_PT(BLE_CHCK_PT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BLE_CHCK_PT__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BLE_CHCK_PT__MIA_MSG;
        msg->mia_info.mia_counter_ms = BLE_CHCK_PT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

#endif
