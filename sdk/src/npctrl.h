/* -----------------------------------------------------------------------------
*
*  npctrl.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2015 by Shane Saxon - saxon@openantz.com
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

#ifndef npctrl_H_
#define npctrl_H_

#ifdef __cplusplus
	extern "C" {
#endif

/* -----------------------------------------------------------------------------
*
* input event command mapping to target context and flow control by npctrl.h
* control command actions processed by npcmd.h
*
* --------------------------------------------------------------------------- */

//------------------------------------------------------------------------------

void npInitCtrl (void* dataRef);

void npCloseCtrl (void* dataRef);

void npUpdateCtrl (void* dataRef);					//typically once per frame

int npAppLoop(void* dataRef);

void npCtrlCommand (int command, void* dataRef);	//apply to all selected nodes

void npCtrlCommandSet (int command, void* node, void* dataRef);	// specific node

void npCmdOpen( char* filePath, void* dataRef);

#ifdef __cplusplus
	}
#endif
#endif

