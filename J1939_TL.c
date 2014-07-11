/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_TL.c
// Module:       J1939 stack
// Description:  J1939 stack Transport Layer
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


static J1939_RX_MESSAGE_T J1939_rx_message;
static J1939_RX_STATE_MACHINE_T J1939_rx_state_machine;
static J1939_RX_PDU_T J1939_rx_pdu;


//==========================================================================================
// Transport Layer Interface Functions
//==========================================================================================

void TL_init(void)
{
   U08 i;
  
   J1939_rx_message.PGN = 0;
   J1939_rx_message.dest_addr = 0;
   J1939_rx_message.source_addr = 0;
   J1939_rx_message.byte_count = 0;
   
   J1939_rx_pdu.PGN = 0;
   J1939_rx_pdu.dest_addr = 0;
   J1939_rx_pdu.source_addr = 0;
   J1939_rx_pdu.byte_count = 0;
  
   J1939_rx_state_machine.PGN = 0;         
   J1939_rx_state_machine.dest_addr = 0;
   J1939_rx_state_machine.source_addr = 0;
   J1939_rx_state_machine.packet_number = 0;
   J1939_rx_state_machine.timer_counter = 0;
   J1939_rx_state_machine.total_packet_number = 0;
   J1939_rx_state_machine.byte_count = 0;
   J1939_rx_state_machine.status = WAIT_FOR_MESSAGE;
   J1939_rx_state_machine.TP = TP_NONE;
   

   for (i = 0; i < NUMBER_TRANS_RX_BUFFERS; i++)
   {
      J1939_rx_message.data[i] = 0;
   }
   
   for (i = 0; i < NUMBER_PDU_BUFFERS; i++)
   {
      J1939_rx_pdu.data[i] = 0;
   }
}


void TL_process(J1939_RX_PDU_T *pdu_ptr)
{
   U08 i;
   
   if ((pdu_ptr->PGN == TP_CM) || (pdu_ptr->PGN == TP_DT))
   {
      J1939_rx_pdu.PGN = pdu_ptr->PGN;
      J1939_rx_pdu.dest_addr = pdu_ptr->dest_addr;
      J1939_rx_pdu.source_addr = pdu_ptr->source_addr;
      J1939_rx_pdu.byte_count = pdu_ptr->byte_count;
      
      for (i = 0; i < NUMBER_PDU_BUFFERS; i++)
      {
         J1939_rx_pdu.data[i] = pdu_ptr->data[i];
      }    
   } 
   else
   {
      J1939_RX_MESSAGE_T msg;

	  msg.PGN = pdu_ptr->PGN;
      msg.dest_addr = pdu_ptr->dest_addr;
      msg.source_addr = pdu_ptr->source_addr;
      msg.byte_count = pdu_ptr->byte_count;
      
      //for (i = 0; i < NUMBER_TRANS_RX_BUFFERS; i++)
	  for (i = 0; i < NUMBER_PDU_BUFFERS; i++)
	  {
         msg.data[i] = pdu_ptr->data[i];
      }      
      // send message up the stack to the Network Management Layer
      NML_process(&msg);
   }
   
   return;
}


void TL_periodic(void)
{      
   U08 i;
   U08 go_on;
   
   go_on = TRUE;      
   J1939_rx_state_machine.timer_counter++;

   while (go_on)
   {
      switch (J1939_rx_state_machine.status)
      {
         case  WAIT_FOR_MESSAGE:
            if (J1939_rx_pdu.PGN == TP_CM && J1939_rx_pdu.dest_addr == GLOBADDR || J1939_rx_pdu.dest_addr == NODEADDR)
            {
               if (J1939_rx_pdu.data[0] == TP_CM_RTS)
               {
                  J1939_rx_state_machine.PGN = J1939_rx_pdu.data[6];
                  J1939_rx_state_machine.PGN <<= 8;
                  J1939_rx_state_machine.PGN += J1939_rx_pdu.data[5];
                  J1939_rx_state_machine.dest_addr = J1939_rx_pdu.dest_addr;
                  J1939_rx_state_machine.source_addr = J1939_rx_pdu.source_addr;
                  J1939_rx_state_machine.packet_number = 0;
                  J1939_rx_state_machine.timer_counter = 0;
                  J1939_rx_state_machine.total_packet_number = J1939_rx_pdu.data[3];
                  J1939_rx_state_machine.byte_count = J1939_rx_pdu.data[2];
                  J1939_rx_state_machine.byte_count <<= 8;
                  J1939_rx_state_machine.byte_count += J1939_rx_pdu.data[1];
                  J1939_rx_state_machine.status = INIT_REASSEMBLE_STRUCTURE;
                  J1939_rx_state_machine.TP = TP_CM_RTS;
               }
               else if (J1939_rx_pdu.data[0] == TP_CM_BAM)
               {
                  J1939_rx_state_machine.PGN = J1939_rx_pdu.data[6];
                  J1939_rx_state_machine.PGN <<= 8;
                  J1939_rx_state_machine.PGN += J1939_rx_pdu.data[5];
                  J1939_rx_state_machine.dest_addr = J1939_rx_pdu.dest_addr;
                  J1939_rx_state_machine.source_addr = J1939_rx_pdu.source_addr;
                  J1939_rx_state_machine.packet_number = 0;
                  J1939_rx_state_machine.timer_counter = 0;
                  J1939_rx_state_machine.total_packet_number = J1939_rx_pdu.data[3];
                  J1939_rx_state_machine.byte_count = J1939_rx_pdu.data[2];
                  J1939_rx_state_machine.byte_count <<= 8;
                  J1939_rx_state_machine.byte_count += J1939_rx_pdu.data[1];
                  J1939_rx_state_machine.status = INIT_REASSEMBLE_STRUCTURE;
                  J1939_rx_state_machine.TP = TP_CM_BAM;          
               }
               else
               {
                  go_on = FALSE;//break;
               }
            }
            else
            {
               go_on = FALSE;//break;
            }
         break;
         
         case INIT_REASSEMBLE_STRUCTURE:
            if (J1939_rx_state_machine.TP == TP_CM_RTS)
            {   
            }
            else if (J1939_rx_state_machine.TP == TP_CM_BAM)
            {
               if (J1939_rx_state_machine.byte_count < NUMBER_TRANS_RX_BUFFERS)
               {                  
                  J1939_rx_message.PGN = J1939_rx_state_machine.PGN;
                  J1939_rx_message.dest_addr = J1939_rx_state_machine.dest_addr;
                  J1939_rx_message.source_addr = J1939_rx_state_machine.source_addr;
                  J1939_rx_message.byte_count = J1939_rx_state_machine.byte_count;
               
                  for (i = 0; i < NUMBER_TRANS_RX_BUFFERS; i++)
                  {
                     J1939_rx_message.data[i] = 0;
                  }                          
                  J1939_rx_state_machine.status = WAIT_FOR_DATA;
               }
               else
               {
                  J1939_rx_state_machine.status= WAIT_FOR_MESSAGE;
                  J1939_rx_state_machine.timer_counter = 0;
                  go_on = FALSE;
                  //break;
               }         
            }
            else
            {
               J1939_rx_state_machine.status = WAIT_FOR_MESSAGE;
               J1939_rx_state_machine.timer_counter = 0;
               go_on = FALSE;
               //break;
            }
         break;
  
         case CHECK_PACKET:
         break;
      
         case SEND_ABORT:
         break;
      
         case SEND_CTS_WITH_COUNT:
         break;
         
         case WAIT_FOR_DATA:
            if (J1939_rx_pdu.PGN == TP_DT)
            {
               J1939_rx_state_machine.status = CHECK_DATA_PACKET;
               //break;
            }
            else
            {
               J1939_rx_state_machine.status = CHECK_TIMER;
               //break;
            }
         break;
         
         case CHECK_TIMER:
            if (J1939_rx_state_machine.timer_counter > (750/TICK))   // 750ms
            {
               J1939_rx_state_machine.status = RESET_REASSEMBLY_STRUCTURE;
               //J1939_rx_state_machine.timer_counter = 0;
               //break;
            }
            else
            {
               J1939_rx_state_machine.status = WAIT_FOR_DATA;
               go_on = FALSE;//break;
            }
         break;                  

         case RESET_REASSEMBLY_STRUCTURE:
            J1939_rx_state_machine.PGN = 0;         
            J1939_rx_state_machine.dest_addr = 0;
            J1939_rx_state_machine.source_addr = 0;
            J1939_rx_state_machine.packet_number = 0;
            J1939_rx_state_machine.timer_counter = 0;
            J1939_rx_state_machine.total_packet_number = 0;
            J1939_rx_state_machine.byte_count = 0;
            J1939_rx_message.PGN = 0;
            J1939_rx_message.dest_addr = 0;
            J1939_rx_message.source_addr = 0;
            J1939_rx_message.byte_count = 0;

            for (i = 0; i < NUMBER_TRANS_RX_BUFFERS; i++)
            {
               J1939_rx_message.data[i] = 0;
            }

            J1939_rx_state_machine.status = WAIT_FOR_MESSAGE;
            go_on = FALSE;
         break; 

         case CHECK_DATA_PACKET:
            if (J1939_rx_state_machine.TP == TP_CM_BAM)
            {               
               if (J1939_rx_pdu.source_addr == J1939_rx_state_machine.source_addr &&
                  J1939_rx_pdu.dest_addr == J1939_rx_state_machine.dest_addr)
               {
                  if (J1939_rx_state_machine.packet_number == J1939_rx_pdu.data[0]-1)
                  {
                     J1939_rx_state_machine.status = SAVE_DATA;
                     J1939_rx_state_machine.timer_counter = 0;
                     J1939_rx_pdu.data[0] = 0;               
                  }
                  else if(J1939_rx_pdu.data[0] != 0)
                  {
                     J1939_rx_state_machine.status = RESET_REASSEMBLY_STRUCTURE;
                  } 
                  else 
                  {
                     J1939_rx_state_machine.status = WAIT_FOR_DATA;
                     go_on = FALSE;
                  }
               }
               else
               {
                  J1939_rx_state_machine.status = WAIT_FOR_DATA;
                  go_on = FALSE;
               }
            }
            else if (J1939_rx_state_machine.TP == TP_CM_RTS)
            {}
            else
            {} 
         break;      
         
         case SAVE_DATA:
            i=0;
         
            while ((i < 7) && ((J1939_rx_state_machine.packet_number*7+i) <= J1939_rx_state_machine.byte_count))
            {
               J1939_rx_message.data[J1939_rx_state_machine.packet_number*7+i] = J1939_rx_pdu.data[i+1];
               J1939_rx_pdu.data[i+1] = 0;
               i++;
            }
            J1939_rx_state_machine.packet_number++;
         
            if (J1939_rx_state_machine.packet_number == J1939_rx_state_machine.total_packet_number)
            {
               J1939_rx_state_machine.status = FILL_USER_MESSAGE;                          
            }
            else
            {
               J1939_rx_state_machine.status = WAIT_FOR_DATA;
               go_on = FALSE;
            }
         break;
             
         case SEND_EOM:
      
         case SEND_CTS:
      
         case FILL_USER_MESSAGE:
            J1939_rx_state_machine.status = RESET_REASSEMBLY_STRUCTURE;
            J1939_rx_state_machine.timer_counter = 0;

            // send message up the stack to the Network Management Layer
            NML_process(&J1939_rx_message);
            
            //go_on = FALSE;
         break;
         

         default:
            go_on = FALSE;
         break;
                                                
      } // end switch   
   } // end while   
}


U08 Transmit_J1939msg(J1939_TX_MESSAGE_T *msg_ptr)
{
   if (msg_ptr->byte_count > 8)
   {
#if NOT_YET
      // Transport layer for transmission of J1939 messages > 8 bytes not implemented yet
#endif
   }
   else
   {
      Build_CANpkt(msg_ptr, 0);
   }

   return TRUE;
}
