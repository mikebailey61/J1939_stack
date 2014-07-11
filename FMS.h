/////////////////////////////////////////////////////////////////////////////////////////
// File:         FMS.h
// Module:       J1939 stack
// Description:  Defines for the FMS layer
// Originator:   MB
// Derived from: N/A
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


// FMS compliance options
#define FMS_TRUCK_STANDARD             1
#define FMS_BUS_STANDARD               2
#define FMS_COMPLIANCE                (FMS_TRUCK_STANDARD)

// 1 Parameters for FMS gateway
// always MSB (Most Significant Bit first)
#if (FMS_COMPLIANCE & FMS_BUS_STANDARD || FMS_COMPLIANCE & FMS_TRUCK_STANDARD)
// 1.1 Parameters for Bus and Truck FMS-Standard
#define FMS_LFC    0x00FEE9  // 1.1.1 Fuel consumption, 0x00FEE9=65,257, 1000ms
#define FMS_DD     0x00FEFC  // 1.1.2 Dash Display, 0x00FEFC=65,276, 1000ms
#define FMS_EEC1   0x00F004  // 1.1.3 Electronic Engine Controller #1, 0x00F004=61,444, 20ms
#define FMS_HOURS  0x00FEE5  // 1.1.4 Engine Hours, Revolutions, 0x00FEE5=65,253, 1000ms
#define FMS_VI     0x00FEEC  // 1.1.5 Vehicle Identification, 0x00FEEC=65,260, 10,000ms
#define FMS_FMS    0x00FDD1  // 1.1.6 FMS-standard Interface Identity, 0x00FDD1=64,977, 10,000ms
#define FMS_VDHR   0x00FEC1  // 1.1.7 High Resolution Vehicle Distance, 0x00FEC1=65,217, 1000ms
#define FMS_TCO1   0x00FE6C  // 1.1.8 Tachograph, 0x00FE6C=65,132, 20ms/50ms
#define FMS_ET1    0x00FEEE  // 1.1.9 Engine Temperature 1, 0x00FEEE=65,262, 1000ms
#define FMS_AMB    0x00FEF5  // 1.1.10 Ambient Conditions, 0x00FEF5=65,269, 1000ms
#define FMS_DI     0x00FE6B  // 1.1.11 Driver's Identification, 0x00FE6B=65,131, 10,000ms
#define FMS_LFE    0x00FEF2  // 1.1.12 Fuel Economy, 0x00FEF2=65,266, 100ms
#define FMS_AIR1   0x00FEAE  // 1.1.13 Air Supply Pressure, 0x00FEAE=65,198, 1000ms
#define FMS_HRLFC  0x00FD09  // 1.1.14 High Resolution Fuel Consumption (Liquid), 0x00FD09=64,777, 1000ms
#define FMS_AT1T1I 0x00FE56  // 1.1.15 Aftertreatment 1 Diesel Exhaust Fluid Tank 1 Information, 0x00FE56=65,110, 1000ms
#define FMS_FMS1   0x00FD7D  // 1.1.16 FMS Tell Tale Status, 0x00FD7D=64,893, 1000ms
#endif

#if (FMS_COMPLIANCE & FMS_TRUCK_STANDARD)
// 1.2 Parameters for Truck FMS-Standard
#define FMS_CCVS   0x00FEF1  // 1.2.1 Cruise Control/Vehicle Speed, 0x00FEF1=65,265, 100ms
#define FMS_EEC2   0x00F003  // 1.2.2 Electronic Engine Controller #2, 0x00F003=61,443, 50ms
#define FMS_VW     0x00FEEA  // 1.2.3 Vehicle Weight, 0x00FEEA=65,258, 1000ms
#define FMS_SERV   0x00FEC0  // 1.2.4 Service Information, 0x00FEC0=65,216, 1000ms
#define FMS_PTODE  0x00FDA4  // 1.2.5 PTO Drive Engagement, 0x00FDA4=64,932, 100ms
#define FMS_CVW    0x00FE70  // 1.2.6 Combination Vehicle Weight, 0x00FE70=65,136, 10,000ms
#define FMS_ERC1   0x00F000  // 1.2.7 Electronic Retarder Controller, 0x00F000=61,440, 100ms
#endif

#if (FMS_COMPLIANCE & FMS_BUS_STANDARD)
// 1.3 Parameters for Bus FMS-Standard
#define FMS_CCVS   0x00FEF1  // 1.3.1 Cruise Control/Vehicle Speed, 0x00FEF1=65,265, 100ms
#define FMS_EEC2   0x00F003  // 1.3.2 Electronic Engine Controller #2, 0x00F003=61,443, 50ms
#define FMS_DC1    0x00FE4E  // 1.3.3 Door Control 1, 0x00FE4E=65,102, 100ms
#define FMS_DC2    0x00FDA5  // 1.3.4 Door Control 2, 0x00FDA5=64,933, 100ms
#define FMS_TD     0x00FEE6  // 1.3.5 Time/Date, 0x00FEE6=65,254, 1000ms
#define FMS_AS     0x00FED5  // 1.3.6 Alternator Speed, 0x00FED5=65,237, 1000ms
#define FMS_ETC2   0x00F00f  // 1.3.7 Electronic Transmission Controller 2, 0x00F005=61,445, 1000ms
#endif