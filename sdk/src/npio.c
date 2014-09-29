/* -----------------------------------------------------------------------------
*
*  nppio.c
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

#include "npio.h"

#include "io/npfile.h"
#include "io/npch.h"
#include "io/npmouse.h"
#include "io/npconsole.h"
#include "io/net/nposc.h"

#include "io/db/npdb.h"


/*! Initialize IO systems
*
*	@param dataRef is a global map reference instance
*
*	@todo add shared resource handling for multiple global map instances
*
*/
//-----------------------------------------------------------------------------
void npInitIO( void* dataRef )
{
	/// init the local IO devices

	/// launch file services and updates hard-coded global variables from file
	npInitFile( dataRef );

	/// Plugin Manager for native libraries and 3rd party modules.
	/// Once loaded, plugins can modify any method or data.
	npInitPlugin( dataRef );

	/// IO channels use both local drives and networking
	npInitCh( dataRef );	

	/// keyboard mapping and event handling
//	npInitKeyboard (dataRef);		//zz

	/// mouse event handling
	npInitMouse( dataRef );
//	npInitSerial (dataRef);			//zz support terminal based system boot-up

	/// audio video input and output
//  npInitAV (dataRef);				//zz
	npInitVideo( dataRef );

	/// start network connections which in turn can further update init state
	npInitOSC( dataRef );			//zz-osc

	/// @todo change npConnectDB over to npInitDB
	npInitDB( dataRef );

}


//-----------------------------------------------------------------------------
void npCloseIO (void* dataRef)
{
	npCloseDB( dataRef );
	npCloseMouse( dataRef );
	npCloseCh( dataRef );	//zz-JJ
	npCloseFile( dataRef );
}


// update IO devices such as keyboard, mice, video, TNG3B and other inputs
//------------------------------------------------------------------------------
void npUpdateIO (void* dataRef)
{
	pData data = (pData) dataRef;

	data->io.cycleCount++;

	//we double buffer the mouse delta movement to maintain engine cycle sync
	npUpdateMouse (dataRef);
	
	npUpdateConsole (dataRef);

	npUpdateCh (dataRef);			//zz-JJ

	npUpdateDB( dataRef );			//zzd
}


//view tree of files catalogued
//------------------------------------------------------------------------------
void npViewFiles (void* dataRef)
{
	FILE* file = NULL;

	pData data = (pData) dataRef;

	//display names and paths of indexed files

	//let user find and choose the files
//	npFileBrowser (dataRef);
}


/// @todo move npViewDatabases, related to: DB, ctrl, map, gl scene, hud, file
//view catalog of DBs from pathmap
//------------------------------------------------------------------------------
void npViewDatabases (void* dataRef)
{
	pData data = (pData) dataRef;
	pNPconsole console = &data->io.gl.hud.console;

	//menuList = malloc(sizeof(NPmenu));

	console->menu = &console->menuStruct;

	console->menu = npdbGetMenu(console->menu, dataRef); //get the list

	if (console->menu == NULL)
	{
		printf( "err 5517 - null console->menu \n" );
		return;					//failed to get menu
	}
	//call menu function, pass list ptr and callback function ptr
	npConsoleMenu (npdbLoadMenuItem, console, dataRef);
}

//hybrid ASCII text based console(s) with 3D objects, xtree vr
//choose a folder, file or DB to view, can select multiple items
//manipulating 3D objects changes console text and simultaneously vice versa
//supports multiple instances of the viewer and its components
//selection sets and active object tracking per console, multi-user conpatible
//xtree gold style UI with wildcards and key shortcuts... user prompts
//commands - Load, Save, Cut, Copy, Paste, Delete, Split, Merge, Move, Convert
//select - drag region, shift/ctrl/arrows, wildcard and boolean expressions
//------------------------------------------------------------------------------
void npViewer (void* dataRef)
{
	FILE* file = NULL;

	pData data = (pData) dataRef;

//	if (data->io.gl.fullscreen)	//workaround, gl file dialog conflict
//		npCtrlCommand(kNPcmdFullscreen, data);

	//dynamic components, default has Files on left and DBs on the right
//	npViewFiles(dataRef);			//view tree of files catalogued
	npViewDatabases(dataRef);		//view catalog of DBs from pathmap

	//npViewDataMap()		//can be used for runtime code profiling analysis
	//npViewIO()
	//npViewNetworks()
	//npViewSystem()		//shows HW and SW components, IO, RAM, CPU, PCIe
	//npViewCode()			//code editor with syntax highlighting and compile
							//automatic dynamic plugin loading within antz
	
//	npPostTool (kNPtoolBrowse, data);
	
	//npThread(npDataBrowser, dataRef);

	//currently display a simple list or to launch a file dialog
	//later add full 3D xtree viewer with live data sampling and thumb images...
	// npDisplayDatasets()	//known folders on the left and known DB on the right
	//						//option to specify or browse for new folder or DB
							//options to filter data type, 
								// meta info (date, size, etc.)
								// file types, DB, CSV, JPG...
	// npCtrlCommand(kNPcmdPauseUpdates, dataRef);		//freeze NPE, GL, etc...
	// if (file) Then
	//  file = npFileDialog (data->io.file.csvPath, kNPfileDialogOpen, dataRef);
	//	if (file == directory || file.tableType == tablemap)
	//   do the following in a loop for all files... CSV, JPG...
	//		npFileOpen (file,dataRef);
	//		format = npGetFileFormat (file, dataRef);
	//		dataType = npGetDataType (file, dataRef);
	//		npMultiThread (npParse(fileBuffer,dataRef));
	//		do npReadFileChunk()
	// else (DB)
	//	npListDB();	//list known databases and allow user to choose (multiple)
	//  db = npBrowseDB(dataRef);
	//  npOpenDB (db, dataRef);
	//	
	// sync 
	// npCtrlCommand(kNPcmdDoUpdates, dataRef);	//update tracks, NPE, GL...
	// return 0;


	return;
}
