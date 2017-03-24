/* -----------------------------------------------------------------------------
*
*  nposcpack.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2016 by Shane Saxon - saxon@openantz.com
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
*  with this software (license file named LICENSE.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */
/* 
*  This file provides a C-Wrapper for the C++ based oscpack library
*
* --------------------------------------------------------------------------- */

#ifndef NPOSCPACK_H_
#define NPOSCPACK_H_


//use above function instead of custom data struct
struct NPoscConn {
	int id;

	char* txURL;	//requires DNS lookup, unless is a numeric IP address
	char* rxURL;

	char txIP[24];		//either IPv4 or IPv6 as a string "255.1.1.1"
	char rxIP[24];

	int txPort;
	int rxPort;
	//int oscPackIdx;  // index into array that holds the oscpack instances
};
typedef struct NPoscConn NPoscConn;
typedef struct NPoscConn* pNPoscConn;

struct NPoscPackSender {
	int oscPackIdx; // index into array that holds the oscpack instances
};
typedef struct NPoscPackSender NPoscPackSender;
typedef struct NPoscPackSender* pNPoscPackSender;

// listener functions
//zz void npStartListeners( pNPoscPackConn oscListener, int argc, char** argv, void* dataRef );
void npInitOscPackListener( pNPoscConn oscListener, void* dataRef );
void npStartOscPackListener( pNPoscConn oscListener );

//zz osc
//create listener, returns listener id
//either IPv4 or IPv6 as a string "255.1.1.1"
//requires DNS lookup, unless is a numeric IP address
//int npOscNewListener( char* txURL, char* txIP, int txPort,
//					  char* rxURL, char* rxIP, int rxPort, void* dataRef );

// read a line that has been received by listener
//zz note this is used by npch.c
char* npOscReadLine( char* bufferStart, int maxLength, int udpFd );

// Tx sender functions
void npInitOscPackSender( pNPoscPackSender oscSender, char* ip, int port );
void npCreateOscPackBundle( pNPoscPackSender oscSender );
void npBeginOscPackMessage( pNPoscPackSender oscSender, char* message );
void npOscAddString( pNPoscPackSender oscSender, char* s );
void npOscAddInt( pNPoscPackSender oscSender, int i );
void npOscAddFloat( pNPoscPackSender oscSender, float f );
void npOscAddBool( pNPoscPackSender oscSender, bool b );
void npEndOscPackMessage( pNPoscPackSender oscSender );
void npSendOscPackBundle( pNPoscPackSender oscSender );

/*	//zz-JJ may want something like this... not sure yet...
// Rx receive functions
void npOscGetNTP( long int timeStampNTP );
void npOscGetString( pNPoscPackSender oscSender, char* s );
void npOscGetInt( pNPoscPackSender oscSender, int i );
void npOscGetFloat( pNPoscPackSender oscSender, float f );
void npOscGetBool( pNPoscPackSender oscSender, bool b );
*/

//send OSC message  //zz-jj moved from nposc.c
void npOscTx( int oscID, char* addr, char* tag, void** arguments, void* dataRef);

//
void npOscListenerThread (void* oscListenerX);	//JJ-zz called from nposc

#define OSC_csvCh "/csvCh"
#define OSC_csvChMap "/csvChMap"
#define OSC_csvNode "/csvNode"
#define OSC_csvTag "/csvTag"

#define READLINE "/readline"
#define DUMP "/dump"


#endif

