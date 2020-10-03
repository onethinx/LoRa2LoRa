#ifndef ONETHINXEXT01_H
#define ONETHINXEXT01_H	
 
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	coreFunction_L_RX 					= 0x80,
	coreFunction_L_TX 					= 0x81,
    coreFunction_Debug                  = 0xA3,
    coreFunction_Unlock                 = 0xA4,
} coreFunctionsExt_e;

/*!
 * ============================================================================
 * Public radio settings
 * ============================================================================
 */

/*!
 * \brief Represents the possible spreading factor values in LoRa packet types
 */
typedef enum
{
    LORA_SF5                                = 0x05,
    LORA_SF6                                = 0x06,
    LORA_SF7                                = 0x07,
    LORA_SF8                                = 0x08,
    LORA_SF9                                = 0x09,
    LORA_SF10                               = 0x0A,
    LORA_SF11                               = 0x0B,
    LORA_SF12                               = 0x0C,
} RadioLoRaSpreadingFactors_e;

/*!
 * \brief Represents the bandwidth values for LoRa packet type
 */
typedef enum
{
    LORA_BW_500                             = 6,
    LORA_BW_250                             = 5,
    LORA_BW_125                             = 4,
    LORA_BW_062                             = 3,
    LORA_BW_041                             = 10,
    LORA_BW_031                             = 2,
    LORA_BW_020                             = 9,
    LORA_BW_015                             = 1,
    LORA_BW_010                             = 8,
    LORA_BW_007                             = 0,
} RadioLoRaBandwidths_e;

/*!
 * \brief Represents the coding rate values for LoRa packet type
 */
typedef enum
{
    LORA_CR_4_5                             = 0x01,
    LORA_CR_4_6                             = 0x02,
    LORA_CR_4_7                             = 0x03,
    LORA_CR_4_8                             = 0x04,
} RadioLoRaCodingRates_e;

typedef enum
{
    LORA_LOWDATARATEOPTIMIZE_OFF            = 0x00,
    LORA_LOWDATARATEOPTIMIZE_ON             = 0x01
} RadioLoraLowDataRateOptimize_e;

/*!
 * \brief Holds the Radio lengths mode for the LoRa packet type
 */
typedef enum
{
    LORA_PACKET_VARIABLE_LENGTH             = 0x00,         //!< The packet is on variable size, header included
    LORA_PACKET_FIXED_LENGTH                = 0x01,         //!< The packet is known on both sides, no header included in the packet
    LORA_PACKET_EXPLICIT                    = LORA_PACKET_VARIABLE_LENGTH,
    LORA_PACKET_IMPLICIT                    = LORA_PACKET_FIXED_LENGTH,
} RadioLoRaHeaderType_e;

/*!
 * \brief Represents the CRC mode for LoRa packet type
 */
typedef enum
{
    LORA_CRC_ON                             = 0x01,         //!< CRC activated
    LORA_CRC_OFF                            = 0x00,         //!< CRC not used
} RadioLoRaCrcMode_e;

/*!
 * \brief Represents the IQ mode for LoRa packet type
 */
typedef enum
{
    LORA_IQ_NORMAL                          = 0x00,
    LORA_IQ_INVERTED                        = 0x01,
} RadioLoRaIQMode_e;

typedef enum {
	LORA_MAC_PRIVATE_SYNCWORD               = 0x1424,
	LORA_MAC_PUBLIC_SYNCWORD                = 0x3444
} LoRaSyncWord_e;


typedef struct {
    RadioLoRaSpreadingFactors_e			SF;
	RadioLoRaBandwidths_e				BW;
	RadioLoRaCodingRates_e				CR;
    RadioLoraLowDataRateOptimize_e      LowDataRateOptimize;
 }  LoRaModulationParams_t;

typedef struct {
    uint8_t                             PreambleLength;
    RadioLoRaHeaderType_e               HeaderType;
    uint8_t                             PayloadSize;                    //!< Size of the payload (in bytes) to transmit or maximum size of the payload that the receiver can accept.
    RadioLoRaCrcMode_e                  CRCmode;
    RadioLoRaIQMode_e                   IQmode;
    uint16_t                            SyncWord;                       //!< The SX126x syncword setting
}  LoRaPacketParams_t;

typedef struct {
    uint32_t                            Frequency;
    Radio_TXpower_e                     TXpower;                         //!< TXpower, not used in receive mode
    LoRaModulationParams_t              Modulation;
    LoRaPacketParams_t                  Packet;
}  LoRaParams_t;

/*!
 * \brief Represents the possible device error states
 */
typedef union
{
    uint8_t Value;
    struct
    {   //bit order is lsb -> msb
        uint8_t				    : 1;  //!< Reserved
        uint8_t CmdStatus		: 3;  //!< Command status
        uint8_t ChipMode		: 3;  //!< Chip mode
        uint8_t CpuBusy		    : 1;  //!< Flag for CPU radio busy
    };
} chipStatus_t;

/*!
 * \brief Represents the possible IRQ states
 */
typedef union
{
    uint16_t Value;
    struct
    {
        uint8_t IRQ_TX_DONE					    : 1;                    //!< Radio received TX DONE IRQ
        uint8_t IRQ_RX_DONE					    : 1;                    //!< Radio received RX DONE IRQ
        uint8_t IRQ_PREAMBLE_DETECTED			: 1;                    //!< Radio received PREAMBLE DETECTED IRQ
        uint8_t IRQ_SYNCWORD_VALID			    : 1;                    //!< Radio received SYNCWORD VALID IRQ
        uint8_t IRQ_HEADER_VALID				: 1;                    //!< Radio received HEADER VALID IRQ
        uint8_t IRQ_HEADER_ERROR				: 1;                    //!< Radio received HEADER ERROR IRQ
        uint8_t IRQ_CRC_ERROR					: 1;                    //!< Radio received CRC ERROR IRQ
        uint8_t IRQ_CAD_DONE					: 1;                    //!< Radio received CAD DONE IRQ
        uint8_t IRQ_CAD_ACTIVITY_DETECTED		: 1;                    //!< Radio received CAD ACTIVITY DETECTED IRQ
        uint8_t IRQ_RX_TX_TIMEOUT				: 1;                    //!< Radio received RX TX TIMEOUT IRQ
        uint8_t								    : 6;                    //!< Reserved
    };
} IRQstatus_t;

/*!
 * \brief Represents the possible device error states
 */
typedef union
{
    uint16_t Value;
    struct
    {
        uint8_t Rc64kCalib              : 1;                    //!< RC 64kHz oscillator calibration failed
        uint8_t Rc13mCalib              : 1;                    //!< RC 13MHz oscillator calibration failed
        uint8_t PllCalib                : 1;                    //!< PLL calibration failed
        uint8_t AdcCalib                : 1;                    //!< ADC calibration failed
        uint8_t ImgCalib                : 1;                    //!< Image calibration failed
        uint8_t XoscStart               : 1;                    //!< XOSC oscillator failed to start
        uint8_t PllLock                 : 1;                    //!< PLL lock failed
        uint8_t BuckStart               : 1;                    //!< Buck converter failed to start
        uint8_t PaRamp                  : 1;                    //!< PA ramp failed
        uint8_t                         : 7;                    //!< Reserved
    };
} deviceErrors_t;

/*!
 * \brief Represents the packet status for every packet type
 */
typedef union
{
	uint32_t value;
	struct {
		int8_t RssiPkt;                                //!< The RSSI of the last packet
	    int8_t SnrPkt;                                 //!< The SNR of the last packet multiplied by 4
        int8_t SignalRssiPkt;                          //!< The RSSI of the LoRa signal of the last packet
	};
} LoRaPacketStatus_t;

/*!
 * \brief Represents the LoRa Packet statistics
 */
typedef struct  __attribute__((scalar_storage_order("big-endian"))) 
{
	uint16_t NbPktReceived;
    uint16_t NbPktCrcErr;
    uint16_t NbPktHeaderErr;
} LoRaStats_t;

typedef struct 
{
    chipStatus_t                    chipStatus;                 //!< The device error states
    IRQstatus_t                     IRQstatus;                  //!< The IRQ states
    deviceErrors_t                  deviceErrors;               //!< The device error states
    LoRaPacketStatus_t              LoRaPacketStatus;           //!< The packet status for every packet type
    LoRaStats_t                     LoRaStats;                  //!< The LoRa Packet statistics
} RadioStatus_t;

// Open stack implementation below

coreStatus_t        LoRa_RX(LoRaParams_t * LoRaParams, RadioStatus_t * RadioStatus, uint8_t * payload, uint8_t payloadSize, uint16_t timeOutMS);
coreStatus_t        LoRa_TX(LoRaParams_t * LoRaParams, RadioStatus_t * RadioStatus, uint8_t * payload, uint8_t payloadSize, uint16_t timeOutMS);
void                LoRaWAN_Debug(bool debugLedsOn, uint32_t * coreStatePNT); 

// Use the Unlock function before using any other extended functions. Unlocking may void LoRa Alliance Certification by Similarity.
void                LoRaWAN_Unlock();

#endif /* ONETHINXEXT01_H */
