/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_DL.h
// Module:       J1939 stack
// Description:  Defines for the J1939 stack Datalink Layer
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


#ifndef _J1939_DL_H_
#define _J1939_DL_H_


//==========================================================================================
// Datalink Layer Interface Functions
//==========================================================================================

void DL_init(void);
void DL_process(const CAN_PACKET_T* pkt_ptr);
void DL_periodic(void);

const CAN_PACKET_T* Req_CANpkt(void);
void Build_CANpkt(J1939_TX_MESSAGE_T *msg_ptr, U08 tflag);


//========================================================================================
//Datalink Module Data Objects
//========================================================================================
void ring_enqueue(CAN_PACKET_T msg, RING_T *ring);
CAN_PACKET_T *ring_dequeue(RING_T *ring);

#endif