/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_CFG.h
// Module:       J1939 stack
// Description:  Defines for the J1939 stack configuration
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


#ifndef _J1939_CFG_H_
#define _J1939_CFG_H_

// debug conditional compilation switches
#define J1939_HAL_DEBUG                1         // debug switch for the Hardware Abstraction Layer
#define J1939_DL_DEBUG                 1         // debug switch for the Datalink Layer
#define J1939_TL_DEBUG                 1         // debug switch for the Transport Layer
#define J1939_NML_DEBUG                1         // debug switch for the Network Management Layer
#define J1939_AL_DEBUG                 1         // debug switch for the Application Layer


// code conditional compilation switches
#define MUTIPLE_PACKAGE_TRANSPORT      1         // 1:enable;  0:disable  
#define PGN_24_BIT                     1         // 1:enable;  0:disable  
#define PGN_FILTERING                  0         // 1:enable;  0:disable  

#define PGN_NUM                        23
#define TICK                           20        // 20ms
#define NUMBER_TRANS_RX_MACHINES           
#define J1939_MAX_MESSAGE_LENGTH       1785
#define NUMBER_TRANS_RX_BUFFERS        64
#define NUMBER_TRANS_TX_BUFFERS        8
#define NUMBER_PDU_BUFFERS             8
#define BIG_E                          0         // 1:Big-Endian 0:Little-Endian 
#define OUT_BUFFER_SIZE                21
#define IN_BUFFER_SIZE                 21
#define CAN_MAX_BYTE_COUNT             8
#define MIN_PDU2                       240
#define ADDR_NUM                       2
//#define J1939ParDaraNum              62

#define  J1939_FAULT_NUM               (NUMBER_TRANS_RX_BUFFERS-2)/4

#define NODEADDR                       0x23
#define GLOBADDR                       0xFF
#define NULLADDR                       0xFE


//================================    typedefs   ===============================
#if PGN_24_BIT // allow full 24 bit PGN
typedef volatile unsigned long PGN_T;            // unsigned 32 bit definition
#else // allow only 16 bit PGN
typedef volatile unsigned short PGN_T;           // unsigned 16 bit definition
#endif

typedef unsigned char U08;                       // unsigned 8 bit definition
typedef unsigned short U16;                      // unsigned 16 bit definition
typedef unsigned long U32;                       // unsigned 32 bit definition

typedef volatile unsigned char VU08;             // volatile unsigned 8 bit definition
typedef volatile unsigned short VU16;            // volatile unsigned 16 bit definition
typedef volatile unsigned long VU32;             // volatile unsigned 32 bit definition

typedef signed char  S08;                        // signed 8 bit definition
typedef signed short S16;                        // signed 16 bit definition
typedef signed long  S32;                        // signed 32 bit definition

typedef volatile signed char VS08;               // volatile signed 8 bit definition
typedef volatile signed short VS16;              // volatile signed 16 bit definition
typedef volatile signed long VS32;               // volatile signed 32 bit definition

#endif
