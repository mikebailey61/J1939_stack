--------------------------------------------------------------------------------

             xlCANdemo - Demoprogram for the 'XL Family Driver Library'

                      Vector Informatik GmbH, Stuttgart

--------------------------------------------------------------------------------

Vector Informatik GmbH
Ingersheimer Straﬂe 24
D-70499 Stuttgart

Tel.:  0711-80670-200
Fax.:  0711-80670-555
EMAIL: support@vector-informatik.de
WEB:   www.vector-informatik.de

--------------------------------------------------------------------------------

xlCANdemo is a small test application for the CAN functionality on 

- CANcardX 
- CANcardXL
- CANcaseXL
- CANboardXL
- CANboardXL pxi 

For further information look into the 'XL Driver Library - Description.pdf' 
document.

--------------------------------------------------------------------------------

Function:

You can use the xlCANdemo sample to generate and log messages to/from the CAN bus.
This sample utilizes all important functions from the 'XL Family Driver Library'.
While the program is running you can change the options by pressing the appropriate key.

Options:

  xlCANdemo:
  ----------
  
  Keyboard commands: 
                                     
  't'      Transmit a message                            
  'b'      Transmit a message burst                      
  'm'      Transmit a remote message                     
  'g'      Request chipstate                             
  's'      Start/Stop                                    
  'r'      Reset clock                                   
  '+'      Select channel      (up)                      
  '-'      Select channel      (down)                    
  'i'      Select transmit Id  (up)                      
  'I'      Select transmit Id  (down)                    
  'x'      Toggle extended/standard Id                   
  'o'      Toggle output mode                            
  'a'      Toggle timer                                  
  'v'      Toggle logging to screen                      
  'p'      Show hardware configuration                   
  'h'      Help                                          
  'ESC'    Exit                                          

The example xlCANdemo.exe is compiled with Microsoft VC++ 6.0. There is also
Dev-C++ project file: xlCANdemo.dev
 
To compile a new exe file you will need the import library, header file and the dll.



