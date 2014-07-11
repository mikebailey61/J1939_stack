/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_AL.h
// Module:       J1939 stack
// Description:  Defines for the J1939 stack Application Layer
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


#ifndef _J1939_AL_H_
#define _J1939_AL_H_

// DTC - Diagnostic Trouble Code
typedef struct
{
   U08 LS;  // Lamp Status
   U08 FS;  // Flash Status
   U32 SPN; // suspect Parameter Number
   U08 FMI; // Failure Mode Index
   U08 OC;  // Occurrence Count
   U08 CM;  // Conversion Method
}DTC_T;

typedef struct 
{
   DTC_T DTC[J1939_FAULT_NUM];
   U08 count;    
}FAULT_CODE_T; 

typedef struct 
{
   U32 trip_data;
   U32 total_data; 
}VEH_DISTANCE_T;


//========================================================================================
// Application Layer Interface Functions
//========================================================================================

void AL_init(void);
void AL_process(J1939_RX_MESSAGE_T *msg_ptr);
void AL_periodic(void);

U08 PGN_filter(PGN_T pgn);

#endif 