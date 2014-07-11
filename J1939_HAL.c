/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_HAL.c
// Module:       J1939 stack
// Description:  J1939 stack Hardware Abstraction Layer
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


#include "J1939_includes.h"

// this module is just a wrapper to the real HAL layer
// select which HAL layer to use
#define HAL_MODE_PC          1
#define HAL_MODE_X300        2
#define HAL_MODE_V400        3
#define HAL_MODE             HAL_MODE_PC

CAN_PACKET_T rev_can_packet;

#if (HAL_MODE == HAL_MODE_PC)
#if defined(_Windows) || defined(_MSC_VER) || defined (__GNUC__)
#define  STRICT
#include <windows.h>
#endif

#include <stdio.h>

#define UNUSED_PARAM(a)      { a=a; }

#define RECEIVE_EVENT_SIZE   1                   // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE        4096                // internal driver queue size in CAN events


#include "vxlapi.h"

/////////////////////////////////////////////////////////////////////////////
// globals

extern char            g_AppName[XL_MAX_LENGTH + 1];       // Application name which is displayed in VHWconf
extern XLportHandle    g_xlPortHandle;                     // Global porthandle (we use only one!)
extern XLdriverConfig  g_xlDrvConfig;                      // Contains the actual hardware configuration
extern XLaccess        g_xlChannelMask;                    // Global channelmask (includes all founded channels)
extern XLaccess        g_xlPermissionMask;                 // Global permissionmask (includes all founded channels)
extern unsigned int    g_BaudRate;                         // Default baudrate
extern int             g_silent;                           // flag to visualize the message events (on/off)
extern unsigned int    g_TimerRate;                        // Global timerrate (to toggel)
extern XLaccess        g_xlChanMaskTx;                     // Global transmit channel mask

// thread variables
extern XLhandle        g_hMsgEvent;                        // notification handle for the receive queue
extern HANDLE          g_hRXThread;                        // thread handle (RX)
extern HANDLE          g_hTXThread;                        // thread handle (TX)
extern int             g_RXThreadRun;                      // flag to start/stop the RX thread
extern int             g_TXThreadRun;                      // flag to start/stop the TX thread (for the transmission burst)
#elif (HAL_MODE == HAL_MODE_X300)
#elif (HAL_MODE == HAL_MODE_V400)
#endif


//==========================================================================================
// Hardware Abstraction Layer Interface Functions
//==========================================================================================

void HAL_init(void)
{
#if HAL_MODE == HAL_MODE_PC
#elif HAL_MODE == HAL_MODE_X300
   BSP_CAN_init();
#elif HAL_MODE == HAL_MODE_V400
   BSP_CAN_init();
#else
   #error HAL layer not selected
#endif
}


void HAL_periodic(void)
{
   // forward declaration
   void HAL_rx(void);
   // poll the receive port
   HAL_rx();
}


void HAL_tx(const CAN_PACKET_T *pkt_ptr) 
{
#if HAL_MODE == HAL_MODE_PC
   static XLevent       xlEvent;
   XLstatus             xlStatus;
   unsigned int         messageCount = 1;
   VU32 id;

   memset(&xlEvent, 0, sizeof(xlEvent));

   id = pkt_ptr->identifier;
   if (id > 0x7FF) // if identifier is greater than 11 bits
	   id |= 0x80000000; // then set bit 31 of identifier to tell HAL layer to send extended frame

   xlEvent.tag = XL_TRANSMIT_MSG;
   xlEvent.tagData.msg.id      = id;
   xlEvent.tagData.msg.dlc     = pkt_ptr->byte_count;
   xlEvent.tagData.msg.flags   = 0;
   xlEvent.tagData.msg.data[0] = pkt_ptr->data[0];
   xlEvent.tagData.msg.data[1] = pkt_ptr->data[1];
   xlEvent.tagData.msg.data[2] = pkt_ptr->data[2];
   xlEvent.tagData.msg.data[3] = pkt_ptr->data[3];
   xlEvent.tagData.msg.data[4] = pkt_ptr->data[4];
   xlEvent.tagData.msg.data[5] = pkt_ptr->data[5];
   xlEvent.tagData.msg.data[6] = pkt_ptr->data[6];
   xlEvent.tagData.msg.data[7] = pkt_ptr->data[7];
   g_TXThreadRun = 1;

   xlStatus = xlCanTransmit(g_xlPortHandle, g_xlChanMaskTx, &messageCount, &xlEvent);

#elif HAL_MODE == HAL_MODE_X300
#elif HAL_MODE == HAL_MODE_V400
#endif

#if J1939_HAL_DEBUG
   printf("- Transmit         : CM(0x%I64x), %s\n", g_xlChanMaskTx, xlGetErrorString(xlStatus));
#endif
}


// MB removed - #pragma TRAP_PROC
void HAL_rx(void)  
{   
#if HAL_MODE == HAL_MODE_PC
   XLstatus        xlStatus;

   unsigned int    msgsrx = RECEIVE_EVENT_SIZE;
   XLevent         xlEvent; 

   g_RXThreadRun = 1;

   //while (g_RXThreadRun)
   { 
      WaitForSingleObject(g_hMsgEvent,10);

      xlStatus = XL_SUCCESS;

      while (!xlStatus)
      {
         msgsrx = RECEIVE_EVENT_SIZE;
         xlStatus = xlReceive(g_xlPortHandle, &msgsrx, &xlEvent);

         if (xlStatus != XL_ERR_QUEUE_IS_EMPTY)
         {
			CAN_PACKET_T rev_can_packet;

#if J1939_HAL_DEBUG
			if (!g_silent)
            {
               printf("%s\n", xlGetEventString(&xlEvent));
			}
#endif

			rev_can_packet.identifier = xlEvent.tagData.msg.id;
			rev_can_packet.data[0] = xlEvent.tagData.msg.data[0];
			rev_can_packet.data[1] = xlEvent.tagData.msg.data[1];
			rev_can_packet.data[2] = xlEvent.tagData.msg.data[2];
			rev_can_packet.data[3] = xlEvent.tagData.msg.data[3];
			rev_can_packet.data[4] = xlEvent.tagData.msg.data[4];
			rev_can_packet.data[5] = xlEvent.tagData.msg.data[5];
			rev_can_packet.data[6] = xlEvent.tagData.msg.data[6];
			rev_can_packet.data[7] = xlEvent.tagData.msg.data[7];
			rev_can_packet.byte_count = (VU08)(xlEvent.tagData.msg.dlc);
			DL_process(&rev_can_packet);
            //ResetEvent(g_hMsgEvent);
         }
      }
   }
#elif HAL_MODE == HAL_MODE_X300
#elif HAL_MODE == HAL_MODE_V400
#endif
}

// MB removed - #pragma TRAP_PROC
void HAL_CAN_error(void)
{
#if NOT_YET    
   //DisableInterrupts;
   //OS_PUSH_PPAGE();
   OSIntEnter();
   OS_SAVE_SP();
  
   if(CAN1RFLG_CSCIF!=0)
   {
      CAN1RFLG_CSCIF = 1;    
      PTL = 0xFF;
      HAL_init();
   } 
   else if(CAN1RFLG_OVRIF!=0) 
   {
      CAN1RFLG_OVRIF = 1;
      PTL = 0xFF;
      HAL_init();    
   }
                  
   OSIntExit();
   //OS_POP_PPAGE();
#endif
}
