/* -----------------------------------------------------------------------------
*
*  npconsole.c
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

#include "npconsole.h"

#include "../npdata.h"
#include "../npio.h"
#include "../os/npos.h"			//access to system console

#include "../data/npmapfile.h"	//zz debug remove, used by npFileOpenMap(), instead create npCmdOpen
#include "gl/nptags.h"

#include "npmouse.h"

#ifdef NP_MSW_
	#include <freeglut.h>
#endif
#ifdef NP_OSX_
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
#endif
#ifdef NP_LINUX_
	#include <GL/freeglut.h>
#endif

#include "../npctrl.h"


//zz debug console under construction!!!
/* -----------------------------------------------------------------------------
* multiple ASCII consoles with hybrid 2D and 3D console/menu/object interaction
* supports multiple user focus domains with grouped input/output devices
* --------------------------------------------------------------------------- */

void npKeyEvent (int key, int keyEventType);
void npUpdateConsoleText (pNPconsole console, void* dataRef);

//------------------------------------------------------------------------------
void npInitConsole (void* dataRef)
{
	//npConsoleCLS();
	return;
}


//------------------------------------------------------------------------------
void npCloseConsole (void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void npConsoleCmdText( pNPconsole console, void* dataRef )
{
	int i = 0;
	int id = 0;
	int temp = 0;
	int count = 0;
	int size = 0;
	int curs = 0;
	int lower = 0, upper = 0;
	char* str = NULL;
	char msg[256];

	pNPnode node = NULL;
	pData data = (pData) dataRef;
	//npStrCmdToLowerCase(console->inputStr, commandLength);

	str = console->inputStr;
	size = console->inputIndex;
//	printf("\nstr test: %s\n", str);

	//turn on messages to display command respones for user
//	console->mode = kNPconsoleMessage;
	
	
	if (data->io.mouse.tool == kNPtoolTag)	//zz debug, messy - if(..toolTag) else strncmp...
	{
		node = data->map.currentNode; //zz debug //selectedPinNode;	//

		node->tag->title[0] = '\0';
		strncat (node->tag->title, console->inputStr, kNPtagTitleMax);
		npUpdateTag(node->tag);	//sets titleSize and box dimensions

		//if none, create a record_id using the node_id
		if( !node->recordID )					//if no recordID then...
			node->recordID = node->id;			//set recordID to the node id

		node->tag->recordID = node->recordID;	//tag uses node's recordID
		node->tag->tableID = node->tableID;		//and tableID
		
		if ( !node->tagMode )					//unhides tag if needed
			node->tagMode++;

	//	npTagEditMode( false, data );
//		sprintf( msg, "record: %d tag: \"%s\"", node->recordID, node->tag->title );
//		npPostMsg(msg, kNPmsgCtrl, dataRef);			//send as kNPmsgCtrl
//		npPostMsg("Keyboard: Game Mode", kNPmsgCtrl, dataRef);
	}
	else if( strncmp ( "pick ", console->inputStr, 5 ) == 0 )
	{
		curs += 5;

		if( console->inputStr[curs] >= '0' && console->inputStr[curs] <= '9' )
		{
			id = npatoi( &console->inputStr[curs] );
			npSelectNode( (pNPnode)npGetNodeByID( id, data ), data );
			npSetCamTarget (data);
		}
		else
		{
			sprintf( msg, "Unknown: %0.20s", console->inputStr );	
			npPostMsg( msg, kNPmsgView, dataRef );
		}
		
//		npConsolePrompt( console, dataRef );
	}
	else if( strncmp ( "select ", console->inputStr, 7 ) == 0
		|| strncmp ( "sel ", console->inputStr, 4 ) == 0 )
	{
		if( strncmp( "sel ", console->inputStr, 4 ) == 0 )
			curs += 4;
		else
			curs += 7;

		str += curs;
		size -= curs;

		if(  !strncmp( "all", &console->inputStr[curs], 3 ) 
			|| !strncmp( "*", &console->inputStr[curs], 1 ) )
			npCtrlCommand( kNPcmdSelectAll, dataRef );
		else if( strncmp( "none", &console->inputStr[curs], 4 ) == 0 )
		{
			npCtrlCommand( kNPcmdSelectNone, dataRef );
			temp = 1;
		}
		else if( strncmp( "first", &console->inputStr[curs], 4 ) == 0 )	//zz debug turn this into an descriptor list
			npSelectNode( data->map.node[kNPnodeRootPin], dataRef );
		else if( strncmp( "last", &console->inputStr[curs], 4 ) == 0 )
			npSelectNode( data->map.node[data->map.nodeRootCount - 1], dataRef );
		else if( strncmp( "parent", &console->inputStr[curs], 6 ) == 0 )
			npCtrlCommand( kNPcmdPrevBranch, dataRef );
		else if( strncmp( "child", &console->inputStr[curs], 5 ) == 0 )
			npCtrlCommand( kNPcmdNextBranch, dataRef );
		else if(  !strncmp( "not", &console->inputStr[curs], 3 )
				 || console->inputStr[curs] == '!' )	 //except "not" or '!'
		{
			// npPushNot( pNPparseStack stack, pData dataRef );
			// npParseStr( pNPparseStack stack, pNPparseStack str, int size, pData dataRef );
/*			if ( console->inputStr[curs] == '!' )
			{	str += 1;		//increment 1 char
				size -= 1;
			} else
			{	str += 3;		//increment 3 chars for "not"
				size -= 3;		//and decrement 3 from str size
			}
			// push 'not' onto stack and recursively call with curs on next char
			npPushNot( stack, dataRef );
			// recursively call self to parse next
			npParseStr( stack, str, size, dataRef );
*/			npPostMsg( "parsing under construction: err 'not'", kNPmsgView, data );
		}
		else if(    !strncmp( "or", &console->inputStr[curs], 2 )	// 'or'
				 || !strncmp( "||", &console->inputStr[curs], 2 ) ) // '||'
		{
			// desc = npStrNodeDescToC( const char* descStr, int size, 
			// npSelectNodeList( pNPnodeDesc desc, resultMax, dataRef );
			npPostMsg( "parsing under construction: 'or'",kNPmsgView, data );
		}
		else if( !strncmp( "and", &console->inputStr[curs], 3 )		// 'and'
				 || !strncmp( "&", &console->inputStr[curs], 1 )	// '&'
				 || !strncmp( "&&", &console->inputStr[curs], 2 ) ) // '&&'
			npPostMsg( "parsing under construction: 'and'",kNPmsgView, data );
		 else if( npStrDigitIsNext( str, 3 ) )
		{
			if ( npStrToRange( &lower, &upper, &console->inputStr[curs], 
				(console->inputIndex - curs) ) == 0 )
			{
				sprintf( msg, "Unknown Range: %0.20s", console->inputStr );	
				npPostMsg( msg, kNPmsgView, dataRef );
				temp = 1;
			}
			else
			{
				if ( lower == upper )
				{
					node = npGetNodeByID( lower, data );
					node->selected = true;
					npSelectNode( node, data );
					npSetCamTarget (data);
				}
				else
				{
					//select all nodes in range
					for( i = lower; i <= upper; i++ )
					{
						node = npGetNodeByID( i, data );
						node->selected = true;
					}
					npSelectNode( node, data );
					npSetCamTarget (data);
					temp = 1;
				}
			}
		}
		else if( !strncmp( "branch", &console->inputStr[curs], 6 ) )
		{
			curs += 7;
			if ( !npStrDigitIsNext( &console->inputStr[curs], 1 )) curs++;
		//	if ( !npStrDigitIsNext( str, 1 )) curs++;
		//	if ( !npStrDigitIsNext( str, 1 )) curs++;
			//scan for first digit
	//		curs += npSeekFirstDigit( &console->inputStr[curs + 6],
	//									(console->inputIndex - curs) =);

			//convert the digit(s) to int for lower and (if exists) upper range
	//		str = &console->inputStr;
			size = console->inputIndex - curs;
			count = npStrToRange( &lower, &upper, &console->inputStr[curs], size );
			printf("\n branch select test: %d %d\n", lower, upper);
			if ( !count )
			{
				sprintf( msg, "Unknown Range: %0.20s", console->inputStr );	
				npPostMsg( msg, kNPmsgView, dataRef );
			}
			else
			{
				printf("\n branch select test: %d %d\n", lower, upper);

				if( 1 )//lower != upper )
				{
					// nodeList = npMapSelectNodes();
					for( i = 0; i < 500000; i++ )			//zz debug, improve this!!!
					{
						node = data->map.nodeID[i];
						if( node )
						{
							if(node->branchLevel >= lower && node->branchLevel <= upper )
								node->selected = true;
						}
					}
					npSelectNode( node, data );
					npSetCamTarget( data );
				}
				else	//else select single item
				{
					node = npGetNodeByID( lower, data );
					node->selected = true;
					npSelectNode( node, data );
					npSetCamTarget( data );
				}
			}
		}
		else
		{
			sprintf( msg, "Unknown: %0.20s", console->inputStr );	
			npPostMsg( msg, kNPmsgView, dataRef );
			temp = 1;
		}
		
		//select the node unless unknown selection
		data->map.currentNode->selected = true;

//		npConsolePrompt( console, dataRef );
	}
	else if( strncmp( "new",console->inputStr, 3 ) == 0 )
	{
		//just 'new' assumes user wants a new pin node
		if( console->inputIndex == 3 )
		{
			//if not in pin mode then select previous pin node
			if (data->map.nodeRootIndex < kNPnodeRootPin )
				npCtrlCommand( kNPcmdPin, data );

			npCtrlCommand( kNPcmdNew, dataRef );
		}
//		else if ( strncmp( "new child",console->inputStr, 8 ) == 0 )
		else if ( strncmp( "new pin",console->inputStr, 8 ) == 0 )
		{	//switch to pin mode if needed
			if( data->map.nodeRootIndex < kNPnodeRootPin )
				npCtrlCommand( kNPcmdPin, data );
			npCtrlCommand( kNPcmdNew, data );
		}
		else if ( strncmp( "new grid",console->inputStr, 8 ) == 0 )
		{
			//switch to grid mode if needed
			if( data->map.nodeRootIndex != kNPnodeRootGrid )
				npCtrlCommand( kNPcmdGrid, data );
			npCtrlCommand( kNPcmdNew, data );
		}
		else if ( strncmp( "new camera",console->inputStr, 8 ) == 0 )
		{
			if( data->map.nodeRootIndex != kNPnodeRootCamera )
				npCtrlCommand( kNPcmdCamera, data );
			npCtrlCommand( kNPcmdNew, data );
		}
	//	else
	//		npCtrlCommand( kNPcmdNew, dataRef );
	//	}
	//	else if( strncmp( "select none", console->inputStr, 10 ) == 0 )
	//		npCtrlCommand( kNPcmdSelectNone, dataRef );
		else
		{
			sprintf( msg, "Unknown Parameter: %0.20s", console->inputStr );	
			npPostMsg( msg, kNPmsgView, dataRef );
		}
		
//		npConsolePrompt( console, dataRef );
	}
	else if( !strncmp( "del", console->inputStr, 3 )
			 || !strncmp( "delete", console->inputStr, 6 ) )
	{
		if( !strncmp( "del ", console->inputStr, 4 ) )
			curs += 3;
		else
			curs += 6;

	//	if( strlen( console->inputStr ) == 3 )
	//		npCtrlCommand( kNPcmdDelete, data );
		if(  !strncmp( " all", &console->inputStr[curs], 4 ) 
			|| !strncmp( " *", &console->inputStr[curs], 2 ) )
		{
		//	npNodeDeleteAll(data);
			npSelectNode( data->map.node[data->map.nodeRootCount - 1], data );
			while ( data->map.nodeRootCount > kNPnodeRootPin )
				npCtrlCommand( kNPcmdDelete, dataRef );
		}
	//	else if( strncmp( "select none", console->inputStr, 10 ) == 0 )
	//		npCtrlCommand( kNPcmdSelectNone, dataRef );
		else
		{
			npCtrlCommand( kNPcmdDelete, data );
//			sprintf( msg, "Unknown Items: %0.20s", console->inputStr );	
//			npPostMsg( msg, kNPmsgView, dataRef );
		}
		
//		npConsolePrompt( console, dataRef );
	}
	else if( strncmp( "help",console->inputStr, 4 ) == 0
			|| strncmp( "?",console->inputStr, 1 ) == 0 )
	{
		//	sprintf( msg, "Help is under construction!!!");	
		//	npPostMsg( msg, kNPmsgView, dataRef );

		//ascert that console mode set to max lines
		data->io.gl.hud.console.level = kNPconsoleLevelMax;

		npPostMsg ("+---------------------- Command Console Help -----------------------+", kNPmsgView, dataRef);
		npPostMsg ("| Console commands can be used to create, select and modify, etc... |", kNPmsgView, dataRef);
		npPostMsg ("| Some commands have shortcuts and except multiple parameters.      |", kNPmsgView, dataRef);
		npPostMsg ("+-------------------------------------------------------------------+", kNPmsgView, dataRef);
		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);
		npPostMsg ("| new           - create default pin node                           |", kNPmsgView, dataRef);
		npPostMsg ("| new grid      - create grid or can specify 'camera'               |", kNPmsgView, dataRef);
		npPostMsg ("| delete        - delete current item                               |", kNPmsgView, dataRef);
		npPostMsg ("| del all       - delete all (pin) nodes using shortcut 'del'       |", kNPmsgView, dataRef);
//		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);
//		npPostMsg ("| save name.csv - save all nodes to a csv with 'name'               |", kNPmsgView, dataRef);
		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);
		npPostMsg ("| pick 22       - pick a specific node without selecting it         |", kNPmsgView, dataRef);
		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);
		npPostMsg ("| select all    - select all (pin) nodes                            |", kNPmsgView, dataRef);
		npPostMsg ("| sel all       - select all (pin) nodes using shortcut 'sel'       |", kNPmsgView, dataRef);
		npPostMsg ("| sel none      - clear selection                                   |", kNPmsgView, dataRef);
		npPostMsg ("| sel first     - first, last, next, prev, root, parent, or child   |", kNPmsgView, dataRef);
		npPostMsg ("| sel 42        - specify a node by record_id                       |", kNPmsgView, dataRef);
		npPostMsg ("| sel 42:99     - select a range of nodes by record_id              |", kNPmsgView, dataRef);
		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);
		npPostMsg ("| sel branch=1   - all nodes with branch_level = 1                  |", kNPmsgView, dataRef);
		npPostMsg ("| sel branch=1:3 - all nodes with branch_level = 1 to 3             |", kNPmsgView, dataRef);
		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);	
	//	npPostMsg ("| (current) selection attributes: geo, topo, color and branch       |", kNPmsgView, dataRef);
		npPostMsg ("| *future support for all attributes and boolean logic.             |", kNPmsgView, dataRef);
		npPostMsg ("| ie: sel 42:99 with geo = 3:7 and branch >= 1 and not color = 4    |", kNPmsgView, dataRef);
		npPostMsg ("|                                                                   |", kNPmsgView, dataRef);
		npPostMsg ("+-------------------------------------------------------------------+", kNPmsgView, dataRef);
		
//		npConsolePrompt( console, dataRef );
	}
	else
	{
		// npUpdateConsoleMenu( console, data );
		sprintf( msg, "Unknown Command: %0.20s", console->inputStr );
		npPostMsg( msg, kNPmsgView, dataRef );

//		npConsolePrompt( console, dataRef );
	}

	//done processing user input, prompt the user for next command
//	console->mode = kNPconsoleCmd;
	npConsolePrompt( console, dataRef );
}
//------------------------------------------------------------------------------
void npConsoleMenuText( pNPconsole console, void* dataRef )
{
	char msg[256];
	int itemChosen = 0;

	itemChosen = npatoi(console->inputStr);

	if (itemChosen > 0 && itemChosen <=  console->menu->count)
	{	
		sprintf(msg,"item %s", console->menu->list[itemChosen]);
		npPostMsg(msg, kNPmsgView, dataRef);

		//exit menu mode
		console->mode = kNPconsoleMessage;
		console->cursorShow = false;
		npPostMsg("Exit Console Menu", kNPmsgCtrl, dataRef);			//send as kNPmsgCtrl
		npPostMsg("Keyboard: Game Mode", kNPmsgCtrl, dataRef);

		console->pMenuCallback(itemChosen, dataRef);
	}
	else
	{						
		// npUpdateConsoleMenu(console, data);
		sprintf(msg,"Unknown Item - range is 1 to %d", console->menu->count);
		npPostMsg(msg, kNPmsgView, dataRef);

		npConsolePrompt(console, dataRef);
	}
}

//------------------------------------------------------------------------------
void npConsoleKeyEvent (int key, int keyEventType, void* dataRef)
{
	int i = 0;
	int topItem = 1;
	int bottomItem = 20;
	int lastPage = 0;
	int itemChosen = 0;
	int activeItem = 0;
	int lineStart = 0;

//	char msg[256];
	pData data = (pData) dataRef;

	pNPnode node = NULL;
	pNPconsole console = &data->io.gl.hud.console;


	switch (keyEventType) //first process specific ASCII keys
	{	case kKeyDownASCII :
			switch (key)
			{	case 8  : // kKeyCodeBackSpace
					if(console->inputIndex > 0)	//delete only if input exists
					{
						//perhaps rename inputIndex to inputCount
						//translate col row to cursorIndex or store separately? zz debug
						//if deleting in the middle of string then compact the gap
			
						if ( console->selectText )	//clear inputStr
						{
							console->selectText = false;
							console->inputStr[0] = '\0';
							console->inputIndex = 0;
						}
						else
						{
//							console->inputStr[kNPconsoleInputMax - 1] = '\0';	//zz debug remove this

							if( console->inputStr[console->inputIndex] == '\0' )
								console->inputStr[console->inputIndex - 1] = '\0';
							else
								for( i = console->inputIndex - 1;
									 i < kNPconsoleInputMax - 1; i++ )
								  console->inputStr[i] = console->inputStr[i+1];

							console->inputIndex--;
						}

						npUpdateConsoleUserText(console, data);
					}				
					break;

//				case 32 : //kKeyCodeSpace : //display next page if it exists
					// console->page++;
					// npUpdateConsoleMenu(console, data);
					// break;
				case 13 : //kKeyCodeReturn :   //process command string			
					switch (console->mode)						//zzf
					{
						case kNPconsoleMessage : break;
						case kNPconsoleCmd :
						case kNPconsoleTag :
							//zz debug, add the newline to the string
							npConsoleCmdText(console, data);
							break;
						case kNPconsoleEdit :
							//npConsoleEditEntry (console, data);
							break;
						case kNPconsoleMenu :
							npConsoleMenuText (console, data);
							break;
						default : break;
					}
					break;
				case 27 : //kKeyCodeESC :
					//return to default console mode when done
					console->mode = kNPconsoleMessage;
					console->cursorShow = false;
							
					npPostMsg("Leave Console", kNPmsgCtrl, data);			//send as kNPmsgCtrl
					npPostMsg("Keyboard: Game Mode", kNPmsgCtrl, data);

					// free(console->menu->list);	//zz debug commented-out might fix W8 'Esc' crash
					break;

	//			case 115 : //kKeyCodeF4 : 
	//				data->io.gl.screenGrab = 1;
					break;	//zz debug screenGrab, overides key map

				default :
					if( console->selectText )  // 8 = kKeyCodeBackSpace		//zz debug
						//treat as kKeyCodeBackSpace if text is selected
						npConsoleKeyEvent( 8 , keyEventType, dataRef );

					if( console->inputIndex >= kNPconsoleInputMax - 2 )
						return;				//ignores input once end of buffer -1 hit

					//write key to the first slot and null terminate the string
					if( console->inputStr[console->inputIndex] == '\0' )
					{
						console->inputStr[console->inputIndex++] = (char)key;
						console->inputStr[console->inputIndex] = '\0';
					}
					else	//insert text by shifting buffer +1 char
					{						
						for (i =  kNPconsoleInputMax - 1; i > console->inputIndex; i--)
							console->inputStr[i] = console->inputStr[i-1];

						console->inputStr[kNPconsoleInputMax - 1] = '\0';					
						console->inputStr[console->inputIndex++] = (char)key;
					}

					npUpdateConsoleUserText(console, data);
					break;
			}
			break;


		//process Special NON-ASCII keys, arrows
		case kKeyDownSpecial :			// non-ASCII Glut constants
			switch (key) 
			{							
				case kKeyCodeRight :
					if(1)// console->inputStr[console->inputIndex] != '\0' )
					{
						console->inputIndex++;
						npUpdateConsoleUserText(console, data);
					}
					break;
				case kKeyCodeLeft :		//if modShift then select region
					if( console->selectText && !data->io.key.modShift )
					{	
						console->insertText = true;					//zz debug ???
						console->inputIndex = 0;
						console->selectText = false;

						npUpdateConsoleUserText(console, data);
					}
					else
					{
						console->inputIndex--;
						npUpdateConsoleUserText(console, data);
					}
					break;
				case kKeyCodeUp :	// UP arrow
					switch (console->mode)						//zzf
					{
						case kNPconsoleMessage : break;
						case kNPconsoleCmd :
							npConsoleCmdText( console, data );
							break;
						case kNPconsoleTag :	//re-route, same as left arrow
							npConsoleKeyEvent(kKeyCodeLeft, keyEventType, data);
							break;
						case kNPconsoleEdit : break;
						case kNPconsoleMenu :
							console->page--;
							if (console->page < 0)				//rollover to last page
								console->page = console->menu->count / kNPmenuPerPageMax;		
							npUpdateConsoleMenu(console, data);
							break;
						default : break;
					}
					break;

				case kKeyCodeDown :	// DOWN arrow
					switch (console->mode)						//zzf
					{
						case kNPconsoleMessage : break;
						case kNPconsoleCmd : 
							npConsoleCmdText(console, data);
							break;
						case kNPconsoleTag : 
							npConsoleKeyEvent(kKeyCodeRight, keyEventType, data);
							break;
						case kNPconsoleEdit : break;
						case kNPconsoleMenu :
							console->page++;					//calc last page
							if (console->page > (console->menu->count / kNPmenuPerPageMax))
								console->page = 0;
							npUpdateConsoleMenu(console, data);
							break;
						
						default : break;
					}
					break;
					
				default : break;	
			}
			break;
		default : break;
	}

	//note that arrow selecting return's before this
	console->selectText = false;

	return;
}


//------------------------------------------------------------------------------
void npUpdateConsole (void* dataRef)
{
	int cmdLineCount = 0;
	double halfSecTest = 0.0;

	pData data = (pData) dataRef;
	pNPconsole console = NULL;
	
	//currently supports only 1 console,  make console a node type		//zz debug 
	console = &data->io.gl.hud.console;

	//zz debug, move this code chunk to npTime()
	//subtract the integer portion of the time to get the decimal portion
	halfSecTest = data->io.time - (int)data->io.time;
	//set the blink cursor state every half sec, one sec for full cycle
	//inverse cursor drawn on top of text (in GL) when blinkState is true
	if ( halfSecTest < 0.5 ) 
		data->io.blinkState = true;
	else
		data->io.blinkState = false;

									//call this from event handler instead //zz debug
	//zz tag workaround, messy... but deals with invisible text problem
	if( console->mode == kNPconsoleTag )
		npUpdateConsoleUserText( console, data);
//	npUpdateConsoleUserText(&data->io.gl.hud.console, data);	
}


//entry point for cmd, tag... use of inputStr
//displays a menu list passed in and registers callback function then returns
//selected item sent to callback function upon user action
//------------------------------------------------------------------------------
// void npConsoleText (void (*pTextCallback)(char* textEntry, void* dataRef),
void npConsoleCmd( pNPconsole console, void* dataRef )
{
	pData data = (pData) dataRef;

	if(!console)
	{
		npPostMsg("err 4284 - null console ptr, npConsoleCmd", kNPmsgErr, data);
		return;
	}

	if( data->io.mouse.tool == kNPtoolTag ) //console->mode != kNPconsoleTag ) //zz tag
	{
		data->map.previousNode = data->map.currentNode;	//zz debug ???
					//zz could also restore console->mode to return to previous mode...
		console->mode = kNPconsoleTag;
		console->cursorShow = true;
	}

	//pause message view and switch to command view
	if( console->mode != kNPconsoleTag ) //zz tag
		console->mode = kNPconsoleCmd;
	else
	{
		console->selectText = true;
		printf("TAG TEST!!!\n");
	}

	npUpdateConsoleText( console, data );
}


//------------------------------------------------------------------------------
//void npConsoleEnter() //set user focus to console
void npUpdateConsoleText (pNPconsole console, void* dataRef)
{
	int i = 0;
	int firstItem = 0;			//first menu item on page
	int lastItem = 0;			//last item on page
	int pageItemCount = 0;		//number of items on page
//	char lineStr[256];

	pNPmenu menu = NULL;
	pData data = (pData) dataRef;

	if(!console)
	{
		npPostMsg("err 4284 - null console ptr, npConsoleMenu", kNPmsgErr, data);
		return;
	}

	menu = console->menu;

	npConsoleCLS(console, data);	//clear screen

	switch (console->mode)			//zz tag debug
	{
	  case kNPconsoleTag :
		npPostMsg ("+------------------------- TEXT TAG EDIT ---------------------------+", kNPmsgView, dataRef);
		break;
	  case kNPconsoleCmd :
		npPostMsg ("+------------------------ COMMAND CONSOLE --------------------------+", kNPmsgView, dataRef);
		npPostMsg ("+------- Type 'help' and press Enter to get list of commands -------+", kNPmsgView, dataRef);
		break;
	  default :
		npPostMsg ("+---------------------------- CONSOLE ------------------------------+", kNPmsgView, dataRef);
	}

	npPostMsg(">", kNPmsgView, dataRef);		//zz tag

	//display user command prompt
	npConsolePrompt (console, data);

	//npConsoleKeyEvent(kNPkeyCodeArrowRight, k
	if(0)// console->mode == kNPconsoleTag )
	{
		console->inputIndex++;
		npUpdateConsoleUserText( console, data );
		console->selectText = false;
	}
}


//clear console screen
//------------------------------------------------------------------------------
void npConsoleCLS (pNPconsole console, void* dataRef)
{
	int i = 0;

	//clear user text input buffer and reset index
	console->inputIndex = 0;						//this resets the cursor
	console->inputStr[0] = '\0';					//this clears the text
	console->inputStr[kNPconsoleInputMax] = '\0';	//endpoint safety null '\0'

	//clear console text display buffer and reset cursor
	console->cursorColumn = 0;						//based on inputIndex
	console->cursorLine = 0;
	for (i=0; i < kNPconsoleLineMax; i++)
	{
		console->line[i][0] = '\0';
		console->line[i][kNPconsoleLineMax] = '\0';	//endpoint safety null '\0'
	}

}


//-----------------------------------------------------------------------------
void npConsolePrompt(pNPconsole console, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPtag tag = data->map.currentNode->tag;		//zz debug, should this be selectedPinNode instead


//	npPostMsg(">", kNPmsgView, dataRef);		//zz tag

	if( console->mode == kNPconsoleTag )
	{
/*
		char msg[kNPmessageLengthMax + 1];
		msg[0] = '>';
		msg[1] = '\0';
		// strncpy( &msg[1], tag->title, kNPmessageLengthMax);
		if ( kNPmessageLengthMax < kNPtagTitleMax )
			strncat( &msg[1], tag->title, kNPmessageLengthMax - 1);
		else
			strncat( &msg[1], tag->title, kNPtagTitleMax - 1);
		npPostMsg( msg, kNPmsgView, data );
*/	
		//clear user input buffer
		if ( tag->titleSize < kNPconsoleInputMax )
		{
			console->inputStr[0] = '\0';
			strncat( console->inputStr, tag->title, tag->titleSize );
			console->inputIndex = tag->titleSize;
		}
		else	//zz debug, better handling would be good, user option, del, modify portion
			npPostMsg("err 7373 - title exceeds kNPconsoleInputMax", kNPmsgErr, data);
		
		console->selectText = true;
		console->cursorShow = true;

		npUpdateConsoleUserText( console, dataRef );
	}
	else if( data->io.gl.hud.console.mode == kNPconsoleCmd )		//zz tag
	{
		npPostMsg(">", kNPmsgView, dataRef);		//zz tag

		//clear the input string
		console->inputStr[0] = '\0';
		console->inputIndex = 0;

		console->cursorShow = true;
		npUpdateConsoleUserText( console, dataRef );				//zz tag

	//	npUpdateConsoleUserText( console, data);					//zz tag
	}
}


//------------------------------------------------------------------------------
void npUpdateConsoleUserText(pNPconsole console, void* dataRef)
{
	int i = 0;
	int inputSize = 0;
	int lineStartChar = 0;
	int pageWidth = 0;
	int cursCol = 0;
	int cursLine = 0;
	int currentLine = 0;
	int lineCount = 0;
	char* title = NULL;
//	char str[kNPconsoleCharPerLine + 1];

	pData data = (pData) dataRef;
	pNPtag tag = NULL;

	//if console input is active, then update the cursor and user text
	if( console->mode == 0 )
		return;

	//get inputSize and ascert inputIndex is inbounds
	inputSize = strnlen( console->inputStr, kNPconsoleInputMax - 1 );
	if ( console->inputIndex >= inputSize )
		console->inputIndex = inputSize;
	else if ( console->inputIndex < 0 )
		console->inputIndex = 0;

	//copy user input to tag title and update tag
	if( console->mode == kNPconsoleTag )
	{
		tag = data->map.currentNode->tag;	//get currently selected tag
		if( !tag )
			npPostMsg( "err 7894 - no tag", kNPmsgErr, data );
		else
		{
			tag->title[0] = '\0';
			strncat( tag->title, console->inputStr, kNPtagTitleMax );
			npUpdateTag( tag );
		}
	}

	//1st calc page lineCount from inputSize
	pageWidth = console->charPerLine;
	lineCount = (inputSize +1) / pageWidth;	// +1 for '>' prompt on 1st line

	//2nd calc curs Col/Line from inputIndex
	if( lineCount < 1 )							 // +1 for '>' prompt
	{	
		console->cursorColumn = console->inputIndex +1;
		console->cursorLine = 0;
	}
	else		//zz debug, make cursorColumn range 1-80 (not 0-79)
	{						// and cursorLine   ... 1-40
		console->cursorColumn = ( console->inputIndex +1 )
								- ( pageWidth * lineCount );
		console->cursorLine = ( console->inputIndex +1 ) / 
								 ( pageWidth * ( lineCount + 1 ) );
	}

	//3rd lines from inputStr
	if( lineCount )
	{
		for( i=0; i <= lineCount; i++ )
		{
			//set current line and handle buffer roll-over
			currentLine = console->lineIndex - i;
			while ( currentLine >= console->lineMax ) 
				currentLine -= console->lineMax;

			strncpy( &console->line[currentLine][0],
				 &console->inputStr[((lineCount - i) * pageWidth) -1], pageWidth );
		}
		
		//workaround for 1st line copies when backspacing across lines //zz tag debug 
		strcpy( &console->line[console->lineIndex - lineCount -1 ][0], ">\0" );
	}

	//workaround for 1st line copies when backspacing across lines //zz tag debug
	//add a method to detect backspacing across lines...
	currentLine = console->lineIndex - lineCount - 2;
	while ( currentLine < 0 )
		currentLine += console->lineMax;
	strcpy( &console->line[currentLine][0], ">\0" );

		currentLine = console->lineIndex - lineCount - 1;
	

			while ( currentLine < 0 )
		currentLine += console->lineMax;
	if( tag != NULL )
		sprintf( &console->line[currentLine][0], ">record_id: %d", tag->recordID);

	//4th and lastly, copy 1st line
	currentLine = console->lineIndex - lineCount;
	while ( currentLine < 0 )
		currentLine += console->lineMax;

	console->line[currentLine][0] = '>';						// -1 for '>'
	if( console->mode == kNPconsoleTag )
		strncpy( &console->line[currentLine][1], tag->title, pageWidth -1 );
	else
		strncpy( &console->line[currentLine][1], console->inputStr, pageWidth -1 );

}
//zz tag end


//------------------------------------------------------------------------------
void npUpdateConsoleMenu (pNPconsole console, void* dataRef)
{
	int i = 0;
	int firstItem = 0;			//first menu item on page
	int lastItem = 0;			//last item on page
	int pageItemCount = 0;		//number of items on page
	char lineStr[256];

	pNPmenu menu = NULL;
	pData data = (pData) dataRef;

	if(!console)
	{
		npPostMsg("err 4284 - null console ptr, npConsoleMenu", kNPmsgErr, data);
		return;
	}

	menu = console->menu;

	npConsoleCLS(console, data);	//clear screen

	//display names and paths of indexed files from paths table
	//page at a time with index number next to each entry for user to punch in

	firstItem = console->page * kNPmenuPerPageMax + 1;

	//if last page then calculate number of items on last page
	if ((menu->count - firstItem) < kNPmenuPerPageMax)
		pageItemCount = menu->count - (firstItem - 1);
	else
		pageItemCount = kNPmenuPerPageMax;	//not last page, set to max per page

	lastItem = firstItem + pageItemCount - 1;

	//build this out for mouse and cursor to highlight specific item...    zz debug
	//arrows as page up/down, alternate method is press 'Spacebar' for next
	sprintf(lineStr,"Showing %d to %d (of %d) - press up/down arrow keys for next page", 
			firstItem, lastItem, menu->count);
	npPostMsg (lineStr, kNPmsgView, dataRef);

	//format and print the menubar header
	npPostMsg ("+-----+------------------+------------------------------------------+", kNPmsgView, dataRef);
	sprintf (lineStr, "|   # | %16s | %40s |", menu->name, menu->details);
	npPostMsg (lineStr, kNPmsgView, dataRef);	
	npPostMsg ("+-----+------------------+------------------------------------------+", kNPmsgView, dataRef);

	//print the current page of items
	for (i = firstItem; i <= lastItem; i++)
		npPostMsg (menu->list[i], kNPmsgView, dataRef);

	npPostMsg ("+-----+------------------+------------------------------------------+", kNPmsgView, dataRef);
	npPostMsg ("", kNPmsgView, dataRef);
	npPostMsg ("Enter item number:", kNPmsgView, dataRef);
	
	//display user command prompt
	npConsolePrompt (console, data);
}


//displays a menu list passed in and registers callback function then returns
//selected item sent to callback function upon user action
//------------------------------------------------------------------------------
void npConsoleMenu (void (*pMenuCallback)(int menuItem, void* dataRef),
					pNPconsole console, void* dataRef)
{
	pNPmenu menu = NULL;
	pData data = (pData) dataRef;

	if(!console)
	{
		npPostMsg("err 4284 - null console ptr, npConsoleMenu", kNPmsgErr, data);
		return;
	}

	menu = console->menu;

	//thread the process and/or callback, or								 //zz debug
	//currently driven by GLUT key events processed within the main GLUT app loop

	//store callback function with console, called upon sucessful item selection
	console->pMenuCallback = pMenuCallback;

	//set the initial menu page
	console->page = 0;

	console->mode = kNPconsoleMenu;	//change to menu mode and pause messages

	//register console for user input events		
	//	npNewEventHandler (kNPeventKey, pConsoleKeyHandler, dataRef);		//zz debug
	//	npNewEventHandler (kNPeventMouse, pConsoleMouseHandler, dataRef);
	//	npNewEventHandler (kNPeventHID, pConsoleMouseHandler, dataRef);

	npUpdateConsoleMenu (console, data);
}


void npQueCmdFromCSV ( const char* cmdElement, const char* cmdData, void* dataRef );
// takes a command name in CSV format and gets the associated function ptr
// converts parameters from a CSV string to C-struct
//------------------------------------------------------------------------------
void npQueCmdFromCSV ( const char* cmdElement, const char* cmdData, void* dataRef )
{
	int type = 0;
	int cmdType = 0;
//2	char msg[256];

	pData data = (pData) dataRef;

	// get cmdFuncPtr using by searching mapItem 
//	npGetCmdFuncPtr(cmdElement, dataRef);
	
	// takes params as CSV row and returns ptr C (struct) type
	// npProcessQue() calls npFree(cmdData) which frees data to npMemoryPool()
//	cmdData = npMapValueCSVtoC(mapItem, value);

	//test
	if ( !strncmp( "alpha_mode", cmdElement, 10 ) ) 
		data->io.gl.alphaMode = npatoi(cmdData);
	else
		printf( "err 4738 - unknown CmdElement: %s\n", kNPmsgErr, data );
/*	
	//convert cmdParams based on command function type and call the function
	switch (type)
	{
		case kNPcmdFullscreen :
					kNPcmdAlphaMode;
			if (cmdParams
	}
*/ 
}

//------------------------------------------------------------------------------
void npMainArgcArgvHandler( int argc, char** argv, void* dataRef )
{
	int i = 0;
	int count = 0;		//parameter counter

	int flagType = 0;

	char msg[256];
	
//	char** argv = NULL;

	pData data = (pData) dataRef;

	msg[0] = '\0';

	//zz replace below with agnostic argument parser... pass to npMap...()
	// console should only handle basic parsing
	// npMap...() parses data and commands based on the flagType


	// argv[0] = antz
	// argv[1] = first flag (argc >= 2)
	// argv[2] = parameters associated with first flag, if none, then next flag
	// ....repeat for all flags and their params
	// ie... antz -flagA paramA1 paramA2 -flagB paramB1 paramB2 paramB3...

	for (i=1; i < data->io.argc; i++)		//start at arg 1, 0 is the exe name
	{
		// get the argv flag type
		if (	  strncmp( "-n", argv[i], 2 ) == 0 ) flagType = kNPflagN;
		else if ( strncmp( "-c", argv[i], 2 ) == 0 ) flagType = kNPflagCmd;
		else if ( strncmp( "-f", argv[i], 2 ) == 0 ) flagType = kNPflagFile;
		else if ( strncmp( "-u", argv[i], 2 ) == 0 ) flagType = kNPflagURL;
		else if ( strncmp( "-p", argv[i], 2 ) == 0 ) flagType = kNPflagUDP;
		else if ( strncmp( "-o", argv[i], 2 ) == 0 ) flagType = kNPflagOSC;

		// these flags can exit app before startup process
		else if ( strncmp( "?",  argv[i], 1 ) == 0 ) flagType = kNPflagHelp;
		else if ( strncmp( "/?", argv[i], 2 ) == 0 ) flagType = kNPflagHelp;
		else if ( strncmp( "help", argv[i], 4 ) == 0 ) flagType = kNPflagHelp;
		else if ( strncmp( "-v", argv[i], 2 ) == 0 ) flagType = kNPflagVer;
		else flagType = kNPflagNull;	 // unknown argv reset for next pass

		// 
		
		printf(" c: %d  argv[%d]: %s\n  type %d\n", argc, i, argv[i], flagType);

		count = 0;	//reset the param counter

		//handle number of arguments based on type
		switch( flagType )
		{
			case kNPflagN :
				i++;		//check for 5 params, verify no '-'flag
				while ( *argv[i+count] != '-' && (i+count) < argc ) count++;
				if (count < 5)
				{
					printf("err 3231 - flag '-n' expects 5 parameters\n");
					break;
				}
				npSetMapItem( argv[i+1], argv[i+2], argv[i+3], argv[i+4],
								   argv[i+5], data);
				break;
			case kNPflagCmd :
				i++;		//check for 2 params, verify no '-'flag
				while ( *argv[i+count] != '-' && (i+count) < argc ) count++;
				if (count < 2)
				{
					printf("err 3231 - flag '-c' expects 2 parameters\n");
					i += count;
					exit(0);
					break;
				}
				else
					i += 2;
//				npQueCmd ( argv[i+1], argv[i+2], data);
				break;
			case kNPflagFile : 
				if (i < argc)
					npCmdOpen( argv[i+1], dataRef );
				else
					printf("err 3238 - flag '-f' expects a file-path name\n");
				i++;		//only 1 arg
				break;
			case kNPflagURL : 
				if (i < argc)
					strcpy( data->io.url, argv[i+1] );
				else
					printf("err 3238 - flag '-u' expects a URL parameter\n");
				i++;		//has 1 arg
				break;
			case kNPflagUDP :		//adds connection list and ques for startup
			//	npConnectUDP( argv[i+1], argv[i+2], data );
				i += 2;		//has 2 args
				break;
			case kNPflagOSC :		//adds connection list and ques for startup
			//	npConnectOSC( argv[i+1], argv[i+2], argv[i+3], argv[i+4], data );
				i += 4;		//has 4 args
				break;	
			case kNPflagHelp :	//no args
				// npSystemConsoleHelp( argc, argv ); //zz debug called from npInitData()
				break;
			case kNPflagVer :	//no args
				// npSystemConsoleHelp( argc, argv ); //zz debug called from npInitData()
				break;
			default :
				printf("err 1010 - unknown console flag: %.32s\n", argv[i]);
				break;
		}

//		npPostMsg (data->io.argv[i], kNPmsgCtrl, dataRef);
//		npPostMsg (msg, kNPmsgCtrl, dataRef);

	}
}


#define kNPappVer "0.1 alpha"

//------------------------------------------------------------------------------
void npSystemConsoleHelp (int argc, char** argv)
{
	int ch = 0;		//used for getch()

	//exit if no arguments, must be at least 2
	if( argc < 2)
		return;

	// app ver request
	if ( !strncmp( "-v", argv[1],2) )
	{										//keep ver up-to-date with changes
		printf("\nANTz ver: %s\n", kNPappVer);
		exit(0);
	}

	// if help requested the print the help ver, otherwise return
	if(		!strncmp( "?",	 argv[1], 1)
		 || !strncmp( "/?",	 argv[1], 2)
		 || !strncmp( "-?",	 argv[1], 2)
		 || !strncmp( "help",argv[1], 4) )
	{
		printf("v %s", kNPappVer);					//keep ver up-to-date with changes
	}
	else
		return;	// no help requested, continue to Startup...


	// this routine exits at the end of help contents
	// 24 row by 62 column layout
	printf("\n");
	printf("+------------------------------------------------------------+\n");
	printf("|              ANTz Console Help... DON'T PANIC!             |\n");
	printf("|------------------------------------------------------------|\n");
	printf("|                                                            |\n");
	printf("|  ?  or 'help' or '/?' to show (this) help menu             |\n");
	printf("| -f  file(path) to load ex: -f antz0001.csv                 |\n");
	printf("| -u  URL Browser record_id path ex: \"http://.../query.php\"  |\n");
	printf("| -p  UDP ports Tx followed by Rx ex: -p 9000 8000           |\n");
	printf("| -o  OSC IP & ports: -o 192.168.0.11 9000 192.168.0.11 8000 |\n");
	printf("| -n  any mapItem as CSV: 'mapPath,item,element,type,value'  |\n");
	printf("| -v  display app version                                    |\n");
	printf("|                                                            |\n");
	printf("|------------------------------------------------------------|\n");
	printf("| multiple flags ex: antz -f antz0001.csv -o 0 9000 0 8000   |\n");
	printf("|                                                            |\n");
	printf("| '-f' to load file antz0001.csv                             |\n");
	printf("| '-o' with IP = 0 will autoset IP by listening to Rx port   |\n");
	printf("|                                                            |\n");
	printf("|------------------------------------------------------------|\n");
	printf("|         ANTz is dedicated to the Public Domain, CC0        |\n");
	printf("+------------------------------------------------------------+\n");

	printf("\n 'Esc' exits or 'space' continues... ");
	ch = nposGetKey();	// printf(" keycode: %c = %d\n",ch,ch);
	if (ch == 27)		//'Esc' keycode = 27
		exit(0);		//exit

	printf("\n\n");
	printf("+------------------------------------------------------------+\n");
	printf("| '-f'  File name to load, can include file path.            |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -f antz0001.csv                                   |\n");
	printf("|                                                            |\n");
	printf("| On startup will load file antz0001.csv                     |\n");
	printf("| Can also specify multiple file types and relative paths.   |\n");
	printf("| *future support for directory trees and *.* wildcards.     |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -f map00001.jpg -f \"../usr/csv/*.*\"               |\n");
	printf("|                                                            |\n");
	printf("|------------------------------------------------------------|\n");
	printf("| '-u'  URL used for Browser Query to view by record_id      |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -u \"http://openantz.com/docs/query.php\"           |\n");
	printf("|                                                            |\n");
	printf("| URL points to a web service page generated per record_id   |\n");
	printf("| Launches default browser with record_id of active node.    |\n");
	printf("|                                                            |\n");
	printf("| ex: \"http://openantz.com/docs/query.php?record_id=42\"      |\n");
	printf("|                                                            |\n");
	printf("+------------------------------------------------------------+\n");

	printf("\n'Esc' exits or 'space' continues... ");
	ch = nposGetKey();
	if (ch == 27)		//'Esc' keycode = 27
		exit(0);			//exit

	printf("\n\n");
	printf("+------------------------------------------------------------+\n");
	printf("| '-p'  Ports to use for networking such as OSC over UDP     |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -p 9000 8000                                      |\n");
	printf("|                                                            |\n");
	printf("| Setup a bi-directional OSC connection with Tx port = 9000  |\n");
	printf("| Autodetects Tx and Rx IP by listeing on Rx port = 8000     |\n");
	printf("| *future support planned for additional protocols.          |\n");
	printf("|                                                            |\n");
	printf("|------------------------------------------------------------|\n");
	printf("| '-o'  OSC connection TxRx with both IP's and ports.        |\n");
	printf("|                                                            |\n");
	printf("| uses 4 parameters: TxIP TxPort RxIP RxPort                 |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -o 192.168.0.11 9000 192.168.0.11 8000            |\n");
	printf("|                                                            |\n");
	printf("| Can manually specify both pairs of TxRx IP and ports, or   |\n");
	printf("| set IP = 0 to autodetect IP(s) by listening on Rx port.    |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -o 0 9000 0 8000                                  |\n");
	printf("|                                                            |\n");
	printf("+------------------------------------------------------------+\n");

	printf("\n'Esc' exits or 'space' continues... ");
	ch = nposGetKey();		//zz msw ver of posix getch()
	if (ch == 27)		//'Esc' keycode = 27
		exit(0);			//exit

	printf("\n\n");
	printf("+------------------------------------------------------------+\n");
	printf("| '-n'  Any mapItem & value, formatted as CSV row in quotes. |\n");
	printf("|                                                            |\n");
	printf("| uses 5 parameters: mapPath item element type value         |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -n np_gl 1 fullscreen i 0                         |\n");
	printf("|                                                            |\n");
	printf("| Causes app to startup in a window, fullscreen = 0 for OFF  |\n");
	printf("| Note that certain parameters need double quotes!           |\n");
	printf("|                                                            |\n");
	printf("| ex: antz -n np_gl 1 clear_rgba ffff \"0,0.4,0,1.0\"          |\n");
	printf("|                                                            |\n");
	printf("| Sets the background clear buffer color to a forest green.  |\n");
	printf("|                                                            |\n");
	printf("| also see:                                                  |\n");
	printf("|                                                            |\n");
	printf("| antzglobals.csv for mapping global startup values.         |\n");
	printf("| antzosc.csv for mapping data and commands to OSC Schema.   |\n");
	printf("|                                                            |\n");
	printf("+------------------------------------------------------------+\n");

	printf(" - END HELP -\n");

	printf("\n'Esc' exits or 'Enter' to Startup... ");
	ch = nposGetKey();

	// loop key input until either Esc or Enter is pressed
	while (ch != 27 && ch != 13) ch = nposGetKey();
	
	if (ch == 27)		//'Esc' keycode = 27 and 'Enter' = 13
		exit(0);		//exit
	
	return;				//they hit 'Enter' continue to startup
}

