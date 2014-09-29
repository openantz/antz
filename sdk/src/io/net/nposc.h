/* -----------------------------------------------------------------------------
*
*  nposc.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - saxon@openantz.com
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named COPYING.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#ifndef NPOSC_H_
#define NPOSC_H_

#include "../../npdata.h"
#include "../file/npcsv.h"

/// @todo Build-out mapping of commands and data to 3rd party OSC Schemas.

void npInitOSC( void* dataRef );			//zz-osc
//		npCloseOSC (dataRef);			//zz-osc//
//
//		npInitOSC (dataRef);			//zz-osc
// 'OSC_API' dev docs on sourceforge wiki

// Our 3 High Performance OSC handlers

//npOscElement()	//individual data elements and commands '/np/node/42/hide'
//npOscFile()		//encapsulated CSV, JSON, JPG... etc.
//npOscPlugin()		//3rd party schema mapping '1/fader2'

//npOscSend (int destID, int oscIDchar* address, char* typeTag, void** paramList, void* dataRef)
/*
//RGBA float values or as bytes 0-255
//oscID references the destination IP/port connection and OSC address with type tag
*/


//void npOscTx (int oscID, char* addr, char* tag, void** arguments, void* dataRef );

// oscID - is a connection ID that represents the specific ip/port connection
// tag - type tag string (ie "iii") used to cast the void* argument types
// arguments - is a list of void* pointers, one for each argument

//REVERSE

void npOscRx (int oscID, const char* addr, const char* tag, void** arguments, void* dataRef );


#endif

