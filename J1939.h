/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939.h
// Module:       J1939 stack
// Description:  Defines for the J1939 stack
// Originator:   MB
// Derived from: Freescale J1939 stack
// Date created: 09 June 2014
//---------------------------------------------------------------------------------------
// Revision Log:
//   $Log: $
//
//---------------------------------------------------------------------------------------
// Notes:
//
//---------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C)2014 by Timespace Technology Ltd.  All rights reserved.
//
// This software is confidential.  It is not to be copied or distributed in any form
// to any third party.
//
// This software is provided by Timespace Technology on an 'as is' basis, and Timespace
// Technology expressly disclaims any and all warranties, expressed or implied including,
// without limitation, warranties of merchantability and fitness for a particular
// purpose. In no event shall Timespace Technology be liable for any direct, indirect,
// incidental, punitive or consequential damages - of any kind whatsoever - with respect
// to the software.
/////////////////////////////////////////////////////////////////////////////////////////


#ifndef _J1939_H_
#define _J1939_H_

#include "J1939_includes.h"
#define J1939STACK_VERSION             130

// TODO - Command Request/Ack Parameter Group Numbers
#define REQUEST_PGN                    0x00EA00  // 0x00EA00=59904
#define REQUEST_PGN_NUM                15
#define ACK_PGN                        0x00E800  // 0x00E800=59392
#define ACK_PGN_ACK                    0
#define ACK_PGN_NACK                   1
#define ACK_PGN_ACCESS_DENIED          2
#define ACK_PGN_CANNOT_RESPOND         3

// TODO - Proprietary Parameter Group Numbers
#define PROPRIETARY_A                  0x00EF00  // 0x00EF00=61184
#define PROPRIETARY_A2                 0x01EF00  // 0x01EF00=126720 Note: requires 24 bit PGN capability
#define PROPRIETARY_B_START            0x00FF00  // 0x00FF00=65280
#define PROPRIETARY_B_END              0x00FFFF  // 0x00FFFF=65535

// Receive state machine
#define WAIT_FOR_MESSAGE               0         // State Zero: Waiting for a BAM or RTS data packet to arrive
#define INIT_REASSEMBLE_STRUCTURE      1         // State One: Message reassemble structure and message timer are initialized.
#define CHECK_PACKET                   2         // State Two: The incoming message is for this node.
#define SEND_ABORT                     3         // State Three:  No more buffers to reassemble incoming message, Abort is sent.
#define SEND_CTS_WITH_COUNT            4         // State Four: Buffers are available to reassemble incoming message, send CTS back to sender to indicate number of packets between next CTS (TRANSPORT_PACKET_COUNT). 
#define WAIT_FOR_DATA                  5         // State Five: Waiting for DATA type packet from source.
#define CHECK_TIMER                    6         // State Six:  Message timer is checked for expiration.
#define RESET_REASSEMBLY_STRUCTURE     7         // State Seven: Message reassemble structure is reset.
#define CHECK_DATA_PACKET              8         // State Eight: A DATA type packet has arrived and checked.
#define SAVE_DATA                      9         // State Nine: The data is saved in the message reassemble buffer
#define SEND_EOM                       10        // State Ten:  EOM message is sent.
#define SEND_CTS                       11        // State Eleven: CTS is sent to trigger more data from the sender. 
#define FILL_USER_MESSAGE              12        // State Twelve: The J1939_RX_MESSAGE_T data structure is filled for user application.

// Transport Protocol Layer
#define TP_CM                          0x00EC00  // 0x00EC00=60416
#define TP_CM_RTS                      16
#define TP_CM_CTS                      17         
#define TP_CM_END_OF_MSG_ACK           19
#define TP_CM_CONN_ABORT               255
#define TP_CM_BAM                      32
#define TP_DT                          0x00EB00  // 0x00EB00=60160
#define TP_NONE                        0 

// TODO - Network Management Layer - PGNs
#define REQUEST_FOR_ADDRESS_CLAIMED    0x00EA00  // 0x00EA00=59904         
#define ADDRESS_CLAIMED                0x00EE00  // 0x00EE00=60928
#define CANNOT_CLAIM                   0x00EE00  // 0x00EE00=60928
#define COMMANDED_ADDRESS              0x00FED8  // 0x00FED8=65240

// Diagnostic Trouble Code (DTC)
#define J1939_DM1                      0x00FECA  // 0x00FECA=65226

#define PRIMED                         1
#define NOTPRIMED                      0 

#ifndef NULL
#define NULL                           0
#endif

#ifndef TRUE
#define TRUE                           1
#endif

#ifndef FALSE              
#define FALSE                          0
#endif

typedef struct
{
   PGN_T PGN;
   VU08 data[NUMBER_TRANS_TX_BUFFERS];
   VU16 byte_count;
   VU08 priority;
   VU08 dest_addr;
   VS08 status;
} J1939_TX_MESSAGE_T;

typedef struct
{
   PGN_T PGN;
   VU08 data[NUMBER_TRANS_RX_BUFFERS];
   VU16 byte_count;
   VU08 source_addr;
   VU08 dest_addr;
} J1939_RX_MESSAGE_T;

typedef struct
{
   PGN_T PGN;
   VU08 data[NUMBER_PDU_BUFFERS];
   VU16 byte_count;
   VU08 source_addr;
   VU08 dest_addr;
} J1939_RX_PDU_T;

typedef struct
{
   PGN_T PGN;
   VU08 status;
   VU08 packet_number;
   VU08 total_packet_number;
   VU16 byte_count;
   VU16 timer_counter;
   VU08 source_addr;
   VU08 dest_addr;
   VU08 TP;
} J1939_RX_STATE_MACHINE_T;

typedef struct
{
   VU32 identifier;
   VU08 data[CAN_MAX_BYTE_COUNT];
   VU08 byte_count;
} CAN_PACKET_T;

typedef struct
{
   CAN_PACKET_T *buffer;
   VU16 head;
   VU16 tail;
   VU08 buffer_size;
} RING_T;


//========================================================================================
//J1939 protocol stack Interface Functions
//========================================================================================
void J1939_stk_init(void);
void J1939_stk_periodic(void);

#endif
