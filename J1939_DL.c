/////////////////////////////////////////////////////////////////////////////////////////
// File:         J1939_DL.c
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


CAN_PACKET_T in_buffer[IN_BUFFER_SIZE];
CAN_PACKET_T out_buffer[OUT_BUFFER_SIZE];

RING_T ring_in;
RING_T ring_out;

U08 DL_state;


//========================================================================================
// Datalink Layer Interface Functions
//========================================================================================

void DL_init(void)
{
   U08 i, j;

   // initialise inbound buffer
   for (i = 0; i < IN_BUFFER_SIZE; i++)
   {
      in_buffer[i].byte_count = 0;
      in_buffer[i].identifier = 0;

      for (j = 0; j < CAN_MAX_BYTE_COUNT; j++)
      {
         in_buffer[i].data[j] = 0; 
      }
   }

   // initialise outbound buffer
   for (i = 0; i < OUT_BUFFER_SIZE; i++)
   {
      out_buffer[i].byte_count = 0;
      out_buffer[i].identifier = 0;

      for (j = 0; j < CAN_MAX_BYTE_COUNT; j++)
      {
         out_buffer[i].data[j] = 0;
      }
   }

   // Initialise inbound ring buffer
   ring_in.buffer = &in_buffer[0];
   ring_in.head = 0;
   ring_in.tail = 0;
   ring_in.buffer_size = IN_BUFFER_SIZE - 1; 
     
   // Initialise outbound ring buffer
   ring_out.buffer = &out_buffer[0];
   ring_out.head = 0;
   ring_out.tail = 0;
   ring_out.buffer_size = OUT_BUFFER_SIZE - 1;
   
   DL_state = NOTPRIMED; 
   
   return;  
}


//#pragma CODE_SEG NON_BANKED
void DL_process(const CAN_PACKET_T *pkt_ptr)
{
   ring_enqueue(*pkt_ptr, &ring_in);
   return; 
}


//#pragma CODE_SEG NON_BANKED
void DL_periodic(void)
{ 
   U08 m_PRI, m_PF, m_PS, m_DA, m_SA, i;
   U32 m_ID;
   PGN_T m_PGN;
   CAN_PACKET_T *rpkt_ptr;
   CAN_PACKET_T *tpkt_ptr;
   J1939_RX_PDU_T pdu;

   m_DA = 0;
   m_SA = 0;
   m_PF = 0;
   m_ID = 0;
   m_PGN = 0;
   
   i = 0;

   // handle outbound packets
   tpkt_ptr = ring_dequeue(&ring_out);

   // if outbound ring buffer is not empty
   if (tpkt_ptr != NULL)
   {
      HAL_tx(tpkt_ptr);
   }
   
      
   // handle inbound packets
   while (TRUE)
   {
      rpkt_ptr = ring_dequeue(&ring_in);

      // if inbound ring buffer is empty
      if (rpkt_ptr == NULL)
      {
         break;
      }
      else
      {
         m_ID = rpkt_ptr->identifier;            // extract full 29 bit identifier from CAN packet
		 m_PRI = (PGN_T)((m_ID >> 26) & 0x7);    // rotate and mask bits 28-26 of CAN ID - J1939 Priority
#if PGN_24_BIT
		 m_PGN = (PGN_T)((m_ID >> 8) & 0x3FFFF); // rotate and mask bits 25-8 of CAN ID and then add top 6 zero bits by extending to 32 bits - J1939 PGN
#else
		 m_PGN = (PGN_T)(m_ID >> 8);             // rotate and mask bits 23-8 of CAN ID - J1939 PGN
#endif
		 m_PF = (U08)(m_ID >> 16);               // rotate and mask bits 23-16 of CAN ID  - J1939 PDU Format
		 m_PS = (U08)(m_ID >> 8);                // mask bits 15-8 of CAN ID - J1939 PDU Specific
		 m_DA = m_PS;                            // J1939 Destination Address = J1939 PDU Specific 
		 m_SA = (U08)m_ID;                       // mask bits 7-0  of CAN ID - J1939 Source Address

         // Handle PDU1
         if (m_PF < 240)
         {
            if (m_DA == GLOBADDR)
            {
               pdu.PGN = m_PGN & 0xFF00;
               pdu.byte_count = rpkt_ptr->byte_count;
               pdu.dest_addr = GLOBADDR;
               pdu.source_addr = m_SA; 

               for (i = 0; i < pdu.byte_count; i++)
               {
                  pdu.data[i] = rpkt_ptr->data[i];
               }
            } 
            else if (m_DA == NODEADDR)
            {
               pdu.PGN = m_PGN & 0xFF00;
               pdu.byte_count = rpkt_ptr->byte_count;
               pdu.dest_addr = m_DA;
               pdu.source_addr = m_SA; 

               for (i = 0; i < pdu.byte_count; i++)
               {
                  pdu.data[i] = rpkt_ptr->data[i];
               }
            }
            else
            {
               continue;
            }
         }
         // Handle PDU2
         else
         {
            pdu.PGN = m_PGN;
            pdu.byte_count = rpkt_ptr->byte_count;
            pdu.dest_addr = GLOBADDR;
            pdu.source_addr = m_SA;

            for (i = 0; i < pdu.byte_count; i++)
            {
               pdu.data[i] = rpkt_ptr->data[i];
            }        
         }
       }
      // send PDU up the stack to the Transport Layer
      TL_process(&pdu); 
   }
   return;
}


const CAN_PACKET_T* Req_CANpkt(void)
{
   CAN_PACKET_T *pkt_ptr;

   pkt_ptr = ring_dequeue(&ring_out);

   if (pkt_ptr == NULL)
   {
      DL_state = NOTPRIMED;
      return NULL;
   }
   else
   {
      DL_state = PRIMED;
      return pkt_ptr;
   }
}


void Build_CANpkt(J1939_TX_MESSAGE_T *msg_ptr, U08 tflag)
{
   CAN_PACKET_T pkt_ptr;
   U08 i;
   
   if (tflag == 0)
   {
      pkt_ptr.byte_count = (U08)msg_ptr->byte_count;
      pkt_ptr.identifier = msg_ptr->priority;
      pkt_ptr.identifier = (pkt_ptr.identifier << 18) + msg_ptr->PGN;

      // PDU1 - Peer-to-Peer
      if (msg_ptr->PGN < 0xF000)
      {
         pkt_ptr.identifier = pkt_ptr.identifier + msg_ptr->dest_addr;
      }
      pkt_ptr.identifier = (pkt_ptr.identifier << 8) + NODEADDR;

      for (i = 0; i < pkt_ptr.byte_count; i++)
      {
         pkt_ptr.data[i] = msg_ptr->data[i];
      }

      ring_enqueue(pkt_ptr, &ring_out);
      
      DL_state = PRIMED;
   }
   else
   {
   }
   return;   
}


//#pragma CODE_SEG NON_BANKED
void ring_enqueue(CAN_PACKET_T msg, RING_T *ring)
{
   // if ring buffer is full less one
   if (((ring->tail+1) == ring->head) || ((ring->tail == ring->buffer_size) && (ring->head == 0)))
   {
      return;
   }
   else
   {
      ring->buffer[ring->tail] = msg;            // enqueue CAN message to tail
      ring->tail++;

      // if tail pointer is at end of ring buffer
	  if (ring->tail > ring->buffer_size) 
	  {
         ring->tail = 0;                         // wrap to start of ring buffer->0
	  }
      return;
   }
}


//#pragma CODE_SEG NON_BANKED
CAN_PACKET_T *ring_dequeue(RING_T *ring)
{
   CAN_PACKET_T *temp;

   // if ring buffer is empty
   if (ring->head == ring->tail)
   {
      return NULL;
   }
   else
   {
      temp = &ring->buffer[ring->head];          // dequeue CAN message from head
      ring->head++;

	  // if head pointer is at end of ring buffer
	  if (ring->head > ring->buffer_size)
	  {
         ring->head = 0;                         // wrap to start of ring buffer->0
	  }
      return temp;
   }   
} 
