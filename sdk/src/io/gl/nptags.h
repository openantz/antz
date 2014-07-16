/* -----------------------------------------------------------------------------
*
*  nptag.h
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

#ifndef NPTAGS_H_
#define NPTAGS_H_

#include "../../npdata.h"

//------------------------------------------------------------------------------
void npInitTags (void* dataRef);
void npCloseTags (void* dataRef);

void npUpdateTags (void* dataRef);

//below belong in /data/...
//------------------------------------------------------------------------------

//passed a buffer filled with CSV formatted tags, includes header
void npImportTagsFromCSV (char* buffer, int size, int wordSize, void* dataRef);

//allocates buffer and fills with CSV formatted tags, includes header
void npExportTagsToCSV (char* buffer, int* size, int* wordSize, void* dataRef);

//deletes entire tag list
void npDeleteAllTags (void* dataRef);

void npDeleteTag (int recordID, int tableID, void* dataRef);
void npAddTag (pNPtag tag, void* dataRef);

pNPtag npGetTag (int recordID, int tableID, void* dataRef);
void npTagNode (pNPnode node, void* dataRef);

//called after loading a new state file or tag list file
void npUpdateTag (pNPtag tag);
void npSyncTags (void* dataRef);

void npDrawNodeTextTag (pNPnode node, void* dataRef);
void npDrawTextTag (pNPtag tag, void* dataRef);
void npDrawNodeTags (void* dataRef);

void npGlutDrawString (void *font, char *str);

int npCompareNodes (const void* a, const void* b);

#endif

