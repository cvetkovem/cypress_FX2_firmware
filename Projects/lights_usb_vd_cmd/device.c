/**
 * Copyright (C) 2009 Ubixum, Inc. 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **/

#include <fx2macros.h>

#ifdef DEBUG_FIRMWARE
#include <stdio.h>
#else
#define printf(...)
#endif

BOOL handle_get_descriptor() {
 return FALSE;
}

//************************** Configuration Handlers *****************************

// change to support as many interfaces as you need
//volatile xdata BYTE interface=0;
//volatile xdata BYTE alt=0; // alt interface

// set *alt_ifc to the current alt interface for ifc
BOOL handle_get_interface(BYTE ifc, BYTE* alt_ifc) {
 printf ( "Get Interface\n" );
 if (ifc==0) {*alt_ifc=0; return TRUE;} else { return FALSE;}
}
// return TRUE if you set the interface requested
// NOTE this function should reconfigure and reset the endpoints
// according to the interface descriptors you provided.
BOOL handle_set_interface(BYTE ifc, BYTE alt_ifc) {  
 return TRUE;
}

// handle getting and setting the configuration
// 1 is the default.  If you support more than one config
// keep track of the config number and return the correct number
// config numbers are set int the dscr file.
//volatile BYTE config=1;
BYTE handle_get_configuration() { 
 return 1;
}

// NOTE changing config requires the device to reset all the endpoints
BOOL handle_set_configuration(BYTE cfg) { 
 return cfg==1 ? TRUE : FALSE; // we only handle cfg 1
}


//******************* VENDOR COMMAND HANDLERS **************************
#define VC_LEN_ON  0xB1
#define VC_LEN_OFF 0xB2

BOOL handle_vendorcommand(BYTE cmd) {
 switch ( cmd ) {
 
    case VC_LEN_ON:
        {         
		 PD2 = 1;

         EP0BUF[0] = 0x01;
         EP0BCH=0;
         EP0BCL=1;
         return TRUE;
        }
	
	case VC_LEN_OFF:
        {         
		 PD2 = 0;

         EP0BUF[0] = 0x00;
         EP0BCH=0;
         EP0BCL=1;
         return TRUE; 
        }
    default:
          printf ( "Need to implement vendor command: %02x\n", cmd );
 }
 return FALSE; // not handled by handlers
}


//********************  INIT ***********************

void main_init() {

 REVCTL=3;
 SETCPUFREQ(CLK_48M);
 SETIF48MHZ();
 
 // set IFCONFIG
 // config your endpoints etc.
 // config gpif
 
 OED = (1 << 2); // PD2 output
 PD2 = 0;
 
 printf ( "Initialization Done.\n" );

}


void main_loop() {
 // do some work
}


