/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_AL.h
// Module:       J1939 stack
// Description:  J1939 stack Application Layer
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
#include "FMS.h"


const PGN_T _pgn_table[PGN_NUM] =
{    
#if (FMS_COMPLIANCE & FMS_BUS_STANDARD || FMS_COMPLIANCE & FMS_TRUCK_STANDARD)
   // 1.1 Parameters for Bus and Truck FMS-Standard
   FMS_LFC,        // 1.1.1 Fuel consumption, 0x00FEE9=65,257, 1000ms
   FMS_DD,         // 1.1.2 Dash Display, 0x00FEFC=65,276, 1000ms
   FMS_EEC1,       // 1.1.3 Electronic Engine Controller #1, 0x00F004=61,444, 20ms
   FMS_HOURS,      // 1.1.4 Engine Hours, Revolutions, 0x00FEE5=65,253, 1000ms
   FMS_VI,         // 1.1.5 Vehicle Identification, 0x00FEEC=65,260, 10,000ms
   FMS_FMS,        // 1.1.6 FMS-standard Interface Identity, 0x00FDD1=64,977, 10,000ms
   FMS_VDHR,       // 1.1.7 High Resolution Vehicle Distance, 0x00FEC1=65,217, 1000ms
   FMS_TCO1,       // 1.1.8 Tachograph, 0x00FE6C=65,132, 20ms/50ms
   FMS_ET1,        // 1.1.9 Engine Temperature 1, 0x00FEEE=65,262, 1000ms
   FMS_AMB,        // 1.1.10 Ambient Conditions, 0x00FEF5=65,269, 1000ms
   FMS_DI,         // 1.1.11 Driver's Identification, 0x00FE6B=65,131, 10,000ms
   FMS_LFE,        // 1.1.12 Fuel Economy, 0x00FEF2=65,266, 100ms
   FMS_AIR1,       // 1.1.13 Air Supply Pressure, 0x00FEAE=65,198, 1000ms
   FMS_HRLFC,      // 1.1.14 High Resolution Fuel Consumption (Liquid), 0x00FD09=64,777, 1000ms
   FMS_AT1T1I,     // 1.1.15 Aftertreatment 1 Diesel Exhaust Fluid Tank 1 Information, 0x00FE56=65,110, 1000ms
   FMS_FMS1,       // 1.1.16 FMS Tell Tale Status, 0x00FD7D=64,893, 1000ms
#endif

#if (FMS_COMPLIANCE & FMS_TRUCK_STANDARD)
   // 1.2 Parameters for Truck FMS-Standard
   FMS_CCVS,       // 1.2.1 Cruise Control/Vehicle Speed, 0x00FEF1=65,265, 100ms
   FMS_EEC2,       // 1.2.2 Electronic Engine Controller #2, 0x00F003=61,443, 50ms
   FMS_VW,         // 1.2.3 Vehicle Weight, 0x00FEEA=65,258, 1000ms
   FMS_SERV,       // 1.2.4 Service Information, 0x00FEC0=65,216, 1000ms
   FMS_PTODE,      // 1.2.5 PTO Drive Engagement, 0x00FDA4=64,932, 100ms
   FMS_CVW,        // 1.2.6 Combination Vehicle Weight, 0x00FE70=65,136, 10,000ms
   FMS_ERC1        // 1.2.7 Electronic Retarder Controller, 0x00F000=61,440, 100ms
#endif

#if (FMS_COMPLIANCE & FMS_BUS_STANDARD)
   // 1.3 Parameters for Bus FMS-Standard
   FMS_CCVS,       // 1.3.1 Cruise Control/Vehicle Speed, 0x00FEF1=65,265, 100ms
   FMS_EEC2,       // 1.3.2 Electronic Engine Controller #2, 0x00F003=61,443, 50ms
   FMS_DC1,        // 1.3.3 Door Control 1, 0x00FE4E=65,102, 100ms
   FMS_DC2,        // 1.3.4 Door Control 2, 0x00FDA5=64,933, 100ms
   FMS_TD,         // 1.3.5 Time/Date, 0x00FEE6=65,254, 1000ms
   FMS_AS,         // 1.3.6 Alternator Speed, 0x00FED5=65,237, 1000ms
   FMS_ETC2        // 1.3.7 Electronic Transmission Controller 2, 0x00F005=61,445, 1000ms
#endif
};


void *FuelConsumption_ptr;
void *DashDisplay_ptr;
void *EngSpeed_ptr;
void *WatTemp_ptr;
void *FaultCode_ptr;


U16 FuelConsumption;
U16 DashDisplay;
U16 EngSpeed;
U08 WatTemp;
FAULT_CODE_T FaultCode;


//========================================================================================
// Application Layer Interface Functions
//========================================================================================

void AL_init(void)
{
   U08 i;

   EngSpeed = 0;
   WatTemp = 0;

   FaultCode.count = 0;

   // initialise the fault code table
   for (i = 0; i < J1939_FAULT_NUM; i++)
   {
      FaultCode.DTC[i].LS = 0;
	  FaultCode.DTC[i].FS = 0;
	  FaultCode.DTC[i].SPN = 0;
      FaultCode.DTC[i].FMI = 0;
      FaultCode.DTC[i].OC = 0;
      FaultCode.DTC[i].CM = 0;
   }
}


void AL_periodic(void)
{
}


void AL_process(J1939_RX_MESSAGE_T *msg_ptr)
{
	int i, j;

#if J1939_AL_DEBUG
	//printf("AL_process(RX) SA=%02X, DA=%02X, PGN=%8X\n", msg_ptr->source_addr, msg_ptr->dest_addr, msg_ptr->PGN);
#endif

   switch (msg_ptr->PGN)
   { 
      case FMS_LFC:
		 FuelConsumption = (msg_ptr->data[4] << 8) + msg_ptr->data[3];
         //FuelConsumption_ptr = &FuelConsumption;
#if J1939_AL_DEBUG
		 printf("FMS_LFC Fuel Consumption=%d\n", FuelConsumption);
#endif
		 break;

	  case FMS_DD:
		  DashDisplay = (msg_ptr->data[4] << 8) + msg_ptr->data[3];
         //DashDisplay_ptr = &DashDisplay;
#if J1939_AL_DEBUG
		  printf("FMS_DD DashDisplay=%d\n", DashDisplay);
#endif
		 break;

	  case FMS_EEC1:
		  EngSpeed = (msg_ptr->data[4] << 8) + msg_ptr->data[3];
         //EngSpeed_ptr = &EngSpeed;
#if J1939_AL_DEBUG
		 printf("FMS_EEC1 EngSpeed=%d\n", EngSpeed);
#endif
		 break;
      
	  case FMS_EEC2:
#if J1939_AL_DEBUG
		  printf("FMS_EEC2\n");
#endif
		  break;
	  
	  case FMS_ET1:
         WatTemp = msg_ptr->data[0];
         //WatTemp_ptr = &WatTemp;
#if J1939_AL_DEBUG
		 printf("FMS_ET1 WatTemp=%d\n", WatTemp);
#endif
		 break;
     
      #if MUTIPLE_PACKAGE_TRANSPORT
	  case J1939_DM1:
         FaultCode.count = (U08)((msg_ptr->byte_count - 2) / 4);
   
		 if (FaultCode.count > J1939_FAULT_NUM)
		 {
            j = J1939_FAULT_NUM;
		 }
		 else
		 {
            j = FaultCode.count;
		 }

		 for (i = 0; i < j; i++)
         {
            FaultCode.DTC[i].LS  = msg_ptr->data[0 + i * 4];
            FaultCode.DTC[i].FS  = msg_ptr->data[1 + i * 4];
            FaultCode.DTC[i].SPN = msg_ptr->data[4 + i * 4] & 0xE0;
            FaultCode.DTC[i].SPN = FaultCode.DTC[i].SPN << 3;
            FaultCode.DTC[i].SPN += msg_ptr->data[3 + i * 4];
            FaultCode.DTC[i].SPN = (FaultCode.DTC[i].SPN << 8) + msg_ptr->data[2 + i * 4];
            FaultCode.DTC[i].FMI = msg_ptr->data[4 + i * 4] & 0x1F;
            FaultCode.DTC[i].OC  = msg_ptr->data[5 + i * 4] & 0x7F;
            FaultCode.DTC[i].CM  = (msg_ptr->data[5 + i * 4] & 0x80) >> 7;
#if J1939_AL_DEBUG
			printf("DM1 SPN=%d\n", FaultCode.DTC[i].SPN);
#endif
		 }
         //FaultCode_ptr = &FaultCode;
      break;
      #endif
     
      default:
      break; 
   }
}  


U08 PGN_filter(PGN_T pgn) 
{
   U08 left = 0; 
   U08 right = PGN_NUM - 1;
   U08 middle = 0;

   while (left <= right) 
   { 
      middle = (U08)((left + right) >> 1); 

      if (pgn == _pgn_table[middle])
      { 
         return TRUE; 
      }
      
      if (pgn > _pgn_table[middle]) 
      {
         left = middle + 1;
      } 
      else
      { 
         right = middle - 1;
      }
   } 
   return FALSE;
} 
