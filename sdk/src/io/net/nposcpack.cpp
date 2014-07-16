/* -----------------------------------------------------------------------------
*
*  nposcpack.cpp
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
/* 
*  This file provides a C-Wrapper for the C++ based oscpack library
*
* --------------------------------------------------------------------------- */

//#include "nposcpack.h"		//JJ-zz debug									//JJ-zz //debug, not sure why this include causes problems

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>



using namespace std;

#include "../../../libs/oscpack/src/osc/OscReceivedElements.h"
#include "../../../libs/oscpack/src/osc/OscPacketListener.h"
#include "../../../libs/oscpack/src/ip/UdpSocket.h"
#include "../../../libs/oscpack/src/osc/OscOutboundPacketStream.h"

extern "C"
{
#include "../../data/nptypes.h"
#include "../../npdata.h"
#include "../../npctrl.h"
#include "../../os/npos.h"
#include "../npch.h"                                              
#include "nposc.h"
}

#define LINE_BUFFER_SIZE 60000
#define LINE_WRAP_THRESHHOLD (LINE_BUFFER_SIZE - 1000)
#define NUMBER_LINES 1000
class LineList {
private:
	char lineData[LINE_BUFFER_SIZE];
	int nextWriteBufferIdx;
	char* lines[NUMBER_LINES];
	int nextReadLineIdx;
	int nextWriteLineIdx;
public:
	LineList()
	{
		nextWriteBufferIdx = 0;
		for (int i = 0; i < LINE_BUFFER_SIZE; i++) {
			lineData[i] = 0;
		}
		for (int i = 0; i < NUMBER_LINES; i++) {
			lines[i] = NULL;
		}
		nextReadLineIdx = -1;
		nextWriteLineIdx = 0;
		counter = 0;
	}

	char* ReadLine() {
		if ((nextReadLineIdx != -1) && (nextReadLineIdx != nextWriteLineIdx)) {
			char* result = lines[nextReadLineIdx];
			nextReadLineIdx++;
			if (nextReadLineIdx >= NUMBER_LINES) {
				nextReadLineIdx = 0;
			}
			return result;
		} else {
			return NULL;
		}
	}

	char* BlockingReadLine() {
		char* result = ReadLine();
		while (result == NULL) {
			nposSleep(0.1);
			//printf("ReadLine found nothing, trying again\n");
			result = ReadLine();
		}
		printf("ReadLine: %s\n", result);
		return result;
	}

	//
	//  Add a line to a circular buffer of text lines
	//
	//  Wrap the buffer destination when end is reached.
	//
	//  If the current read location overlaps the area being written, increment it until it doesn't
	//
	int counter;
	void AddLine( const char* line )
	{
		counter++;
		//printf("AddLine %d: %s\n", counter, line );
		char* linePtrStart = &lineData[ nextWriteBufferIdx ];
		nextWriteBufferIdx += strlen(line);
		nextWriteBufferIdx++;
		char* linePtrEnd = &lineData[ nextWriteBufferIdx ];
		if (nextWriteBufferIdx > LINE_WRAP_THRESHHOLD) {
			nextWriteBufferIdx = 0;
		}

		// If we have already read something, and this is going to wipe out some data, adjust the pointers to ignore the data being
		// wiped out
		if (nextReadLineIdx != -1) {
			while ((lines[nextReadLineIdx] >= linePtrStart) && (lines[nextReadLineIdx] <= linePtrEnd)) {
				nextReadLineIdx++;
				if (nextReadLineIdx >= NUMBER_LINES) {
					nextReadLineIdx = 0;
				}
			}
		}

		//
		//  Copy the line, update the line pointers
		//
		strcpy( linePtrStart, line );
		lines[nextWriteLineIdx++] = linePtrStart;
		if (nextWriteLineIdx >= NUMBER_LINES) {
			nextWriteLineIdx = 0;
		}
		// if there has never been a read index, it gets initialized to 0 (since there is now data to read)
		if (nextReadLineIdx == -1) {
			nextReadLineIdx = 0;
		// if the readindex is the same as the write index
		} else if (nextReadLineIdx == nextWriteLineIdx) {
			nextReadLineIdx++;
			if (nextReadLineIdx >= NUMBER_LINES) {
				nextReadLineIdx = 0;
			}
		}
		dump();
	}
	
	void dump() {
		//std::cout << "nextReadLineIdx: " << nextReadLineIdx << "\n";
		//std::cout << "nextWriteLineIdx: " << nextWriteLineIdx << "\n";
		//std::cout << "nextWriteBufferIdx: " << nextWriteBufferIdx << "\n";
		fflush(stdout);
	}

};

#define MAX_OSC_LISTENERS 256						//zz changed from 10
static void* listeners[ MAX_OSC_LISTENERS ];
static void* listenerSockets[ MAX_OSC_LISTENERS ];
static int listenerPorts[ MAX_OSC_LISTENERS ];
static int txPorts[ MAX_OSC_LISTENERS ];
static IpEndpointName lastReceivedEndpoint[ MAX_OSC_LISTENERS ];
static int nextAvailableListenerIdx = 0;
static bool adapterInitialized = false;

#define kNPmaxCommands 200
#define OUTPUT_BUFFER_SIZE 1000
class OscAntzPacketListener : public osc::OscPacketListener {
private:
	void* dataRef_;
	int oscID_;

public:
	OscAntzPacketListener(void *dataRef, int oscID)
	{
		dataRef_ = dataRef;
		oscID_ = oscID;
	}

protected:
	virtual void ProcessMessage( const osc::ReceivedMessage& m, 
				const IpEndpointName& remoteEndpoint )
    {
		int argumentIdx = 0;
		void* npRxArguments[256];  // each pointer points to one of the arrays below
	
		float floats[256];			//zz-JJ changed limit from 10 to 256
		osc::int32 ints[256];
	
		const char* strings;//[256];	//zz-JJ added 's' string type

		lastReceivedEndpoint[ oscID_ ] = remoteEndpoint;
        try {
			npRxArguments[0] = 0;
//zz-JJ commented out because was flooding console and causing second of lag...
//			std::cout << "Received message: " << m.AddressPattern() << "\n";
//			if (m.TypeTags() != NULL) std::cout << "also got typeTags: " << m.TypeTags() << "\n";
//			fflush(stdout);
			const char* types = m.TypeTags();
			if (types != NULL) 
			{
				osc::ReceivedMessageArgumentStream argumentStream = m.ArgumentStream();
				while (*types) {
					if (*types == 'f') {
						argumentStream >> floats[argumentIdx];
						npRxArguments[argumentIdx] = &floats[argumentIdx];
					} else if (*types == 'i') {
						argumentStream >> ints[argumentIdx];
						npRxArguments[argumentIdx] = &ints[argumentIdx];
					} else if (*types == 's') {
						argumentStream >> strings;
						npRxArguments[argumentIdx] = (void*)strings;		//zz-JJ debug add support for multiple strings
					}
					argumentIdx++;
					types++;
				}
			}
			npOscRx (oscID_, m.AddressPattern(), m.TypeTags(), npRxArguments, dataRef_ );
        } catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
		fflush(stdout);
    }
};

class OscTrackPacketListener : public osc::OscPacketListener {
private:
	void* dataRef_;
	int commandIdx;
	const char* incomingCommands[kNPmaxCommands];
	int outgoingCommand[kNPmaxCommands];

public:
	OscTrackPacketListener(void *dataRef)
	{
		dataRef_ = dataRef;
		commandIdx = 0;
		oscCommandIdx = 0;
	}

	void RegisterCommand(const char* incomingOsc, int command ) 
	{
		if (commandIdx < kNPmaxCommands)
		{
			incomingCommands[commandIdx] = incomingOsc;
			outgoingCommand[commandIdx] = command;
			commandIdx++;
		}
	}
	
private:
	int oscCommandIdx;
	const char* incomingOscMessages[10];  // what message comes in
	int incomingParameterOffsets[10];     // which parameter we access
	const char* incomingDataTypeSpecifications[10];  // what data type we expect it to be
	float minimumExpectedIncomingFloatValue[10];            // if it's a float, the minimum and maximum expected values
	float maximumExpectedIncomingFloatValue[10];
	const char* antzFloatTargetOscAddress[10];       // where we put the result
	float floatTargetMinimumValue[10];               // minimum and maximum destination values
	float floatTargetMaximumValue[10];

	float incomingFloatValue[10];  // the value we extracted from Osc message
	float outgoingFloatValue[10];  // the translated value
	bool antzTargetInitialValueSaved[10];    // tracks whether we have set the base value
	float baseFloatValue[10];      // some floats are relative to existing value, so this is the starting value
	float* outgoingFloatAddress[10];  // memory address of the destination float
	
	// sending OSC back out
	bool echoFloat[10];
	const char* echoMessage[10];
	int echoPort[10];

public:
	void EchoFloat( int registrationOffset, const char* outgoingMessage, int portNumber )
	{
		echoFloat[registrationOffset] = true;
		echoMessage[registrationOffset] = outgoingMessage;
		echoPort[registrationOffset] = portNumber;
	}

	void SetIncomingFloatValue( int offset, float value) // used just for testing
	{
		incomingFloatValue[offset] = value;
	}

	float GetOutgoingFloatValue(int offset)
	{
		return ((incomingFloatValue[offset] - minimumExpectedIncomingFloatValue[offset])*(floatTargetMaximumValue[offset] - floatTargetMinimumValue[offset])) + floatTargetMinimumValue[offset];
	}

	// RegisterFloatValueMapping( "/1/fader1", 0, 
	//    (minimum expected float value), (maximum expected float value))
	//    "/node/<<selected>>/translate_x", "f", 0.0, 200.0)
	//
	//  Once a Float Value Mapping configuration is registered, it can intercept an incoming message, extract the
	//  value it needs from that message, and propagate that value to the specified destination.
	//
	//  The "processOscCommands" method checks for messages registered by "RegisterFloatValueMapping"
	//
	int RegisterFloatValueMapping(const char* incomingOscMessage, int incomingParameterOffset, 
		float minimumExpectedFloatValue, float maximumExpectedFloatValue, 
		const char* floatTargetSpecification, float minimumValueToProvide, float maximumValueToProvide, const char* mappingType )
	{
		incomingOscMessages[oscCommandIdx] = incomingOscMessage;
		incomingParameterOffsets[oscCommandIdx] = incomingParameterOffset;
		minimumExpectedIncomingFloatValue[oscCommandIdx] = minimumExpectedFloatValue;
		maximumExpectedIncomingFloatValue[oscCommandIdx] = maximumExpectedFloatValue;
		antzFloatTargetOscAddress[oscCommandIdx] = floatTargetSpecification;
		floatTargetMinimumValue[oscCommandIdx] = minimumValueToProvide;
		floatTargetMaximumValue[oscCommandIdx] = maximumValueToProvide;
		antzTargetInitialValueSaved[oscCommandIdx] = false;
		echoFloat[oscCommandIdx] = false;
		if (strcmp(mappingType, "direct") == 0)
		{
			antzTargetInitialValueSaved[oscCommandIdx] = true;
			baseFloatValue[oscCommandIdx] = floatTargetMinimumValue[oscCommandIdx];
		}
		oscCommandIdx++;
		return oscCommandIdx - 1;
	}


	void processCommands( const char* oscAddress )
	{
		for (int i = 0; i < commandIdx; i++) 
		{
			if (strcmp(oscAddress, incomingCommands[i]) == 0) 
			{
				npCtrlCommand( outgoingCommand[ i ], dataRef_ );
			}
		}
	}

	void processOscMessages( const char* oscAddress, const char* typeTags,  osc::ReceivedMessageArgumentStream argumentStream, const IpEndpointName& remoteEndpoint )
	{
		const char* previousMessage = "";
		float previousFloatValue = 0.0;
		int previousParameterOffset = 0;
		for (int i = 0; i < oscCommandIdx; i++)
		{
			if (strcmp(oscAddress, incomingOscMessages[i]) == 0)
			{
				// if first incoming type is a float, and incoming parameter offset is 0, handle it
				if ((typeTags != NULL) && (*typeTags == 'f') && incomingParameterOffsets[i] == 0)
				{
					// if there was a previous value for this message, just use that
					if ((strcmp(oscAddress, previousMessage) == 0) && (previousParameterOffset == incomingParameterOffsets[i]))
					{
						incomingFloatValue[i] = previousFloatValue;
					}
					else
					{
						previousMessage = oscAddress;
						previousParameterOffset = incomingParameterOffsets[i];
						argumentStream >> incomingFloatValue[i];
						previousFloatValue = incomingFloatValue[i];
					}
					outgoingFloatValue[i] = GetOutgoingFloatValue(i);
					cout << "extracted float value " << incomingFloatValue[i] << ", outgoing value " << outgoingFloatValue[i] << "\n";
					fflush(stdout);
					outgoingFloatAddress[i] = GetOutgoingFloatAddress(antzFloatTargetOscAddress[i]);
					if (outgoingFloatAddress[i] != NULL) 
					{
						if (!antzTargetInitialValueSaved[i])
						{
							antzTargetInitialValueSaved[i] = true;
							baseFloatValue[i] = *outgoingFloatAddress[i];
						}
						*outgoingFloatAddress[i] = outgoingFloatValue[i] + baseFloatValue[i];
						if (echoFloat[i])
						{
							cout << "supposed to echo value using message " << echoMessage[i] << ", port " << echoPort[i] << "\n";
							cout << "the outgoing float value is " << (float)(*outgoingFloatAddress[i]) << "\n";
							fflush(stdout);				
							
							UdpTransmitSocket transmitSocket( IpEndpointName( remoteEndpoint.address, echoPort[i] ));
							char buffer[OUTPUT_BUFFER_SIZE];
							osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

							p << osc::BeginBundleImmediate
								<< osc::BeginMessage( echoMessage[i] ) 
								<< (float)(*outgoingFloatAddress[i])
								<< osc::EndMessage
								<< osc::EndBundle;
							transmitSocket.Send( p.Data(), p.Size() );
						}
					}
				}
				
				// if first incoming type is a float, and incoming parameter offset is 0, handle it
				if ((typeTags != NULL) && (*typeTags == 'f') && incomingParameterOffsets[i] == 1)
				{
					// this only works if the previous value was extracted (we are not keeping track of where we are in extraction)
					argumentStream >> incomingFloatValue[i];
					outgoingFloatValue[i] = GetOutgoingFloatValue(i);
					cout << "(2)extracted float value " << incomingFloatValue[i] << ", outgoing value " << outgoingFloatValue[i] << "\n";
					fflush(stdout);
					outgoingFloatAddress[i] = GetOutgoingFloatAddress(antzFloatTargetOscAddress[i]);
					if (outgoingFloatAddress[i] != NULL) 
					{
						if (!antzTargetInitialValueSaved[i])
						{
							antzTargetInitialValueSaved[i] = true;
							baseFloatValue[i] = *outgoingFloatAddress[i];
						}
						*outgoingFloatAddress[i] = outgoingFloatValue[i] + baseFloatValue[i];
						if (echoFloat[i])
						{
							cout << "supposed to echo value using message " << echoMessage[i] << ", port " << echoPort[i] << "\n";
							cout << "the outgoing float value is " << (float)(*outgoingFloatAddress[i]) << "\n";
							fflush(stdout);				
							
							UdpTransmitSocket transmitSocket( IpEndpointName( remoteEndpoint.address, echoPort[i] ));
							char buffer[OUTPUT_BUFFER_SIZE];
							osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

							p << osc::BeginBundleImmediate
								<< osc::BeginMessage( echoMessage[i] ) 
								<< (float)(*outgoingFloatAddress[i])
								<< osc::EndMessage
								<< osc::EndBundle;
							transmitSocket.Send( p.Data(), p.Size() );
						}
					}
				}
			}
		}
	}

protected:

   
   //
   //  Convert an antz float OSC address specification into a specific float memory address, this is 
   //  in NPmapLink for globals, for nodes, addresses need to be calculated based on node selector.
   //
   //  This is set with RegisterFloatMapping "floatTargetSpecification"
   //
   float* GetOutgoingFloatAddress( const char* floatTargetOscAddress )
   {
	   if (strcmp(floatTargetOscAddress, "/node/<<selected>>/translate/x") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   pNPnode node = data->map.currentNode;
		   if (node != NULL)
		   {
			   return &node->translate.x;
		   }
	   } 
	   else if (strcmp(floatTargetOscAddress, "/node/<<selected>>/translate/y") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   pNPnode node = data->map.currentNode;
		   if (node != NULL)
		   {
			   return &node->translate.y;
		   }
	   } 
	   else if (strcmp(floatTargetOscAddress, "/node/<<selected>>/translate/z") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   pNPnode node = data->map.currentNode;
		   if (node != NULL)
		   {
			   return &node->translate.z;
		   }
	   } 
	   else if (strcmp(floatTargetOscAddress, "/node/<<selected>>/rotate/x") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   pNPnode node = data->map.currentNode;
		   if (node != NULL)
		   {
			   return &node->rotate.x;
		   }
	   } 
	   else if (strcmp(floatTargetOscAddress, "/node/<<selected>>/rotate/y") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   pNPnode node = data->map.currentNode;
		   if (node != NULL)
		   {
			   return &node->rotate.y;
		   }
	   } 
	   else if (strcmp(floatTargetOscAddress, "/node/<<selected>>/rotate/z") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   pNPnode node = data->map.currentNode;
		   if (node != NULL)
		   {
			   return &node->rotate.z;
		   }
	   }
	   else if (strcmp(floatTargetOscAddress, "/np/gl/1/background_r") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   return &data->io.clear.r;
	   }
	   else if (strcmp(floatTargetOscAddress, "/np/gl/1/background_g") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   return &data->io.clear.g;
	   }
	   else if (strcmp(floatTargetOscAddress, "/np/gl/1/background_b") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   return &data->io.clear.b;
	   }
	   else if (strcmp(floatTargetOscAddress, "/np/gl/1/background_a") == 0)
	   {
		   pData data = (pData) npGetDataRef();
		   return &data->io.clear.a;
	   }
	   return NULL;
   }

	virtual void ProcessMessage( const osc::ReceivedMessage& m, 
				const IpEndpointName& remoteEndpoint )
    {
        try {
//zz-JJ		commented out because was flooding console and causing second of lag...
//			std::cout << "Received message: " << m.AddressPattern() << "\n";
//			fflush(stdout);
//			if (m.TypeTags() != NULL) std::cout << "also got typeTags: " << m.TypeTags() << "\n";
//			fflush(stdout);
			processCommands(m.AddressPattern());
			processOscMessages(m.AddressPattern(), m.TypeTags(), m.ArgumentStream(), remoteEndpoint);
        } catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
		fflush(stdout);
    }
};

extern "C" void TestIt()
{
	printf("TESTING SOMETHING NEW....\n");
	int x;
	void *bogusDataRef = (void*)&x;
	OscTrackPacketListener* packetListener = new OscTrackPacketListener(bogusDataRef);
	// since we have no idea whatsoever of the current value of "translate_x", that 0.0 to 200.0 range is relative to whatever 
	// the value is when we first encounter translate_x
	packetListener->RegisterFloatValueMapping("/1/fader1", 0, 0.0, 1.0, "/node/<<selected>>/translate/x", 0.0, 200.0, "direct");
	packetListener->SetIncomingFloatValue(0, 0.3f);
	printf("0.3 output is %f\n", packetListener->GetOutgoingFloatValue(0));
	printf("DONE TESTING SOMETHING NEW....\n");
	fflush(stdout);
}

class CsvTrackPacketListener : public osc::OscPacketListener {

public:
	CsvTrackPacketListener( LineList* lines, void* dRef )
	{
		udpLines = lines;
		dataRef = dRef;
	}
	
	LineList* udpLines;
	void* dataRef;
protected:

	void csvTokenize( const char* textLine, char* tempBuffer, char** tokenList )
	{
		// copy the text into a buffer we can modify
		strcpy( tempBuffer, textLine );

		// set the first token
		char* bufferScan = tempBuffer;
		*tokenList++ = bufferScan;
		*tokenList = NULL;

		// scan that buffer for commas
		while (*bufferScan) 
		{
			// if there is a comma, the next character is the token
			if (*bufferScan == ',')
			{
				*bufferScan = 0;
				*tokenList++ = bufferScan + 1;
				*tokenList = NULL;
			} 
			bufferScan++;
		}
	}

    virtual void ProcessMessage( const osc::ReceivedMessage& m, 
				const IpEndpointName& remoteEndpoint )
    {
		char* tokenList[ kNPmaxTokens ];
		char  textLine[ kNPmaxLineLength ];

        try {
			// not clear yet how to structure incoming address patterns
			// for now, "/channel/csv" is going to be routed to track data somehow
			if( strcmp( m.AddressPattern(), OSC_csvCh ) == 0){
				osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
				const char* s;
				args >> s >> osc::EndMessage;
				udpLines->AddLine( s );
				//std::cout << "Message: " << s << "\n";
			} else if( strcmp( m.AddressPattern(), OSC_csvChMap ) == 0){
				osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
				const char* s;
				args >> s >> osc::EndMessage;
				csvTokenize( s, &textLine[0], &tokenList[0] );
				npMapTokens( &tokenList[0], (pData)dataRef );
				std::cout << "chMap message was: " << s << "\n";
			} else if( strcmp( m.AddressPattern(), READLINE ) == 0){
				std::cout << udpLines->ReadLine() << "\n";
            } else {
				std::cout << "unrecognized message " << m.AddressPattern() << "\n";
			}
        } catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
		fflush(stdout);
    }
};



#define MAX_OSC_SENDERS 256											//zz was 10
//#define OUTPUT_BUFFER_SIZE 10000
static UdpTransmitSocket* senderSockets[ MAX_OSC_SENDERS ];
static osc::OutboundPacketStream* senderStreams[ MAX_OSC_SENDERS ];
static char buffers[MAX_OSC_SENDERS][OUTPUT_BUFFER_SIZE];
static int nextAvailableSenderIdx = 0;

void initAdapter()
{
	if (!adapterInitialized) 
	{
		for (int i = 0; i < MAX_OSC_LISTENERS; i++)
		{
			listenerSockets[i] = NULL;
			listeners[i] = NULL;
			listenerPorts[i] = 0;
		}
		nextAvailableListenerIdx = 0;

		for (int i = 0; i < MAX_OSC_SENDERS; i++)
		{
			senderSockets[i] = NULL;
			senderStreams[i] = NULL;
		}
		nextAvailableSenderIdx = 0;
		adapterInitialized = true;
	}
}

//
//  Start listening on the UDP port
//
void npStartOscPackListener( pNPoscPackListener oscListener )
{
	((UdpListeningReceiveSocket*)listenerSockets[oscListener->id])->RunUntilSigInt();
}


//
//  Initialize NPoscPackListener, get it ready to listen on a port, but does not start listening
//  Listening on port is done by "npStartOscPackListener"
//
//JJ-zz swapped in/out to be out followed by in TxRx 
void npInitOscPackListener( pNPoscPackListener oscListener, void* dataRef )
{
	int outgoingPort = oscListener->txPort;
	int incomingPort = oscListener->rxPort;

	initAdapter();
	oscListener->id = -1;	//JJ-zz debug... change osc err method....

	// port may have already been set up
	for (int i = 0; i < nextAvailableListenerIdx; i++) 
	{
		if (listenerPorts[i] == incomingPort)
		{
			oscListener->id = i;			//zz could offset this by + 1
			return;
		}
	}

	// set up the port
/*	OscTrackPacketListener* packetListener = new OscTrackPacketListener( dataRef );

	// map iphone touchOsc "/2" (control page 2 selection) to the antz kNPcmdCamera command 4250

packetListener->RegisterCommand("/2", kNPcmdCamera);
	packetListener->RegisterFloatValueMapping("/1/fader1", 0, 0.0, 1.0, "/node/<<selected>>/translate/x", 0.0, 200.0, "relative");
	packetListener->RegisterFloatValueMapping("/1/fader1", 0, 0.0, 1.0, "/node/<<selected>>/rotate/x", 0.0, 360.0, "direct");
	packetListener->RegisterFloatValueMapping("/1/fader2", 0, 0.0, 1.0, "/node/<<selected>>/translate/y", 0.0, 100.0, "relative");
	packetListener->RegisterFloatValueMapping("/1/fader2", 0, 0.0, 1.0, "/node/<<selected>>/rotate/y", 0.0, 360.0, "direct");
	packetListener->RegisterFloatValueMapping("/1/fader3", 0, 0.0, 1.0, "/node/<<selected>>/translate/z", 0.0, 50.0, "relative");
	packetListener->RegisterFloatValueMapping("/1/fader3", 0, 0.0, 1.0, "/node/<<selected>>/rotate/z", 0.0, 360.0, "direct");
	int regOffset = packetListener->RegisterFloatValueMapping("/1/fader4", 0, 0.0, 1.0, "/np/gl/1/background_r", 0.0, 1.0, "direct");
	packetListener->RegisterFloatValueMapping("/3/xy", 0, 0.0, 1.0, "/np/gl/1/background_g", 0.0, 1.0, "direct");
	packetListener->RegisterFloatValueMapping("/3/xy", 1, 0.0, 1.0, "/np/gl/1/background_b", 0.0, 1.0, "direct");

	packetListener->EchoFloat(regOffset, "/1/fader5", 9000);  // send the last registered antz float back out port 9000 using /1/fader5
*/
	PacketListener* packetListener = new OscAntzPacketListener(dataRef, nextAvailableListenerIdx);
	listeners[ nextAvailableListenerIdx ] = packetListener; //new CsvTrackPacketListener( new LineList(), dataRef );
	listenerSockets[ nextAvailableListenerIdx ] = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, incomingPort ), (PacketListener*)listeners[ nextAvailableListenerIdx ]);
	listenerPorts[ nextAvailableListenerIdx ] = incomingPort;
	txPorts[ nextAvailableListenerIdx ] = outgoingPort;
	oscListener->id = nextAvailableListenerIdx;
	nextAvailableListenerIdx++;
}

extern "C" void npOscTx (int oscID, char* addr, char* tag, void** arguments, void* dataRef ) 
{
	// send out to same IP address, possibly different port
	UdpTransmitSocket transmitSocket( IpEndpointName( lastReceivedEndpoint[oscID].address, txPorts[oscID]));
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	p << osc::BeginBundleImmediate
			<< osc::BeginMessage( addr );
	while (*tag)		//zz-jj
	{
		switch (*tag)
		{
			case 'f' : p << (float)(*(float*)arguments); break;;
			case 'i' : p << (int)(*(int*)arguments); break;
			case 's' : p << (char*)arguments; break;
			default : npPostMsg("err 3232 - unknown tag type: %c", kNPmsgOSC, dataRef);
		// case 's' : p << ... npNewStrCpy((const char*)arguments)
		}
		arguments++;
		tag++;
	}
//	p << 1.0f; arguments++;   //zz-jj

	p << osc::EndMessage << osc::EndBundle;
	transmitSocket.Send( p.Data(), p.Size() );
}

//
//  Create an NPoscPackSender instance, get it ready to send to a destination IP and port
//
void npInitOscPackSender( pNPoscPackSender oscSender, char* ip, int port )
{
	initAdapter();
	senderSockets[ nextAvailableSenderIdx ] = new UdpTransmitSocket( IpEndpointName( ip, port )); 
	oscSender->oscPackIdx = nextAvailableSenderIdx;
	nextAvailableSenderIdx++;
}

//
//  Create an OSC bundle
//
void npCreateOscPackBundle( pNPoscPackSender oscSender )
{
	if ( senderStreams[ oscSender->oscPackIdx ] == NULL )
	{
		senderStreams[ oscSender->oscPackIdx ] = new osc::OutboundPacketStream( &buffers[ oscSender->oscPackIdx ][0], OUTPUT_BUFFER_SIZE );
		*senderStreams[ oscSender->oscPackIdx ] << osc::BeginBundleImmediate;
	}
}

//
//  Add a message to the current OSC bundle
//
void npBeginOscPackMessage( pNPoscPackSender oscSender, char* address )
{
	*senderStreams[ oscSender->oscPackIdx ] << osc::BeginMessage( address );
}

//
//  OSC messages support 4 data types:
//
//     char*
//     int
//     float
//     bool
//
//  The following 4 functions append those data types to the OSC message under construction
//
void npOscAddString( pNPoscPackSender oscSender, char* val )
{
	*senderStreams[ oscSender->oscPackIdx ] << val;
}
void npOscAddInt( pNPoscPackSender oscSender, int val )
{
	*senderStreams[ oscSender->oscPackIdx ] << val;
}
void npOscAddFloat( pNPoscPackSender oscSender, float val )
{
	*senderStreams[ oscSender->oscPackIdx ] << val;
}
void npOscAddBool( pNPoscPackSender oscSender, bool val )
{
	*senderStreams[ oscSender->oscPackIdx ] << val;
}

//
//  End the current OSC message, but leave bundle available for additional messages
//
void npEndOscPackMessage( pNPoscPackSender oscSender )
{
	*senderStreams[ oscSender->oscPackIdx ] << osc::EndMessage;
}

//
//  End the current OSC bundle, then send it
//
void npSendOscPackBundle( pNPoscPackSender oscSender )
{
	*senderStreams[ oscSender->oscPackIdx ] << osc::EndBundle;
	senderSockets[ oscSender->oscPackIdx ]->Send( senderStreams[ oscSender->oscPackIdx ]->Data(), senderStreams[ oscSender->oscPackIdx ]->Size() );
}

//
// The listener starts, but in its own thread, this function never returns
//
void npOscListenerThread (void* oscListenerX)
{
	pNPoscPackListener oscListener = (pNPoscPackListener)oscListenerX;
	npStartOscPackListener( oscListener );
}

//
//  Startup any UDP listeners as specified on command line
//
/*
extern "C" void npStartListeners( pNPoscPackListener oscListener, int argc, char **argv, void* dataRef )
{
	npPostMsg("npStartListeners, starts OSC UDP listeners if any specified on command line", kNPmsgDebug, dataRef);
	for (int i = 0; i < argc; i++) 
	{
		if (_strnicmp(argv[i], "udp", 3) == 0) 
		{
			if ((i+1) < argc) 
			{
				int incomingPort = atoi( argv[i+1] );
				int outgoingPort = atoi( argv[i+2] );
				npPostMsg("starting a UDP listener!", kNPmsgDebug, dataRef);
				
				// initialize then start listener in another thread
				npInitOscPackListener( oscListener, outgoingPort, incomingPort, dataRef );
				nposBeginThread (npOscListenerThread, oscListener);
			}
		}
}
*/
extern "C" void npOscConnect( pNPosc Item, void* dataRef )
{
	int i = 0;
	pData data = (pData) dataRef;
//	pNPoscItem oscItem = NULL;
	pNPoscPackListener oscItem = NULL;

//	oscItem = data->io.osc.list[oscID];

	// initialize then start listener in another thread
	npInitOscPackListener( oscItem, data );
	nposBeginThread (npOscListenerThread, oscItem);
}

//
// Must be called from a thread that is OK with blocking
//
extern "C" char* npOscReadLine( char* bufferStart, int maxLength, int udpFd )
{
	char* result = ((CsvTrackPacketListener*)listeners[ udpFd ])->udpLines->BlockingReadLine();
	strncpy(bufferStart, result, maxLength);
	return result;
}

