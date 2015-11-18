/* -----------------------------------------------------------------------------
*
*  nptag.c
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

#include "nptags.h"

/// @todo remove this dependency, use freetype instead

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


#include "npgldraw.h"


//------------------------------------------------------------------------------
void npInitTags (void* dataRef)
{
	return;
}

//------------------------------------------------------------------------------
void npCloseTags (void* dataRef)		//clean-up
{
	npDeleteAllTags (dataRef);
}

//------------------------------------------------------------------------------
void npUpdateTags (void* dataRef)							//add to ctrl loop, debug zz
{
	return;
}

//------------------------------------------------------------------------------
void npImportTagsFromCSV (char* buffer, int size, int wordSize, void* dataRef)
{
	return;
}


//allocates buffer and fills with CSV formatted tags, includes header
//------------------------------------------------------------------------------ lde
void npExportTagsToCSV (char* buffer, int* size, int* wordSize, void* dataRef)
{
//	pNPtags tags = &dataRef->io.gl.hud.tags;
	
	return;
}

//deletes entire tag list
//------------------------------------------------------------------------------
void npDeleteAllTags (void* dataRef)
{

	//perhaps should lock list during this process for thread safety
	return;
}

//------------------------------------------------------------------------------
void npDeleteTag (int recordID, int tableID, void* dataRef)
{
	//remove tag from list and compact the gap
	//perhaps should lock list during this process for thread safety
	return;
}


//------------------------------------------------------------------------------
void npAddTag (pNPtag tag, void* dataRef)
{
	//insert tag in the list sorted first by tableID then recordID
	//perhaps should lock list during this process for thread safety
	return;
}

//------------------------------------------------------------------------------
pNPtag npGetTag (int recordID, int tableID, void* dataRef)
{
	int i = 0;
	pNPtag tag = NULL;
	pData data = (pData) dataRef;

	//using a simple loop for now, replace with HASH method, debug zz
//	for (i=0; i < data->map.tags.count; i++)

	return tag;
}

int tagsIndex;	//zzhp debug, move this to the global data struct
//------------------------------------------------------------------------------
void npSetNodeTag (pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;

	static int count = 0;	//zz debug, this method assumes the record_id is ordered, add sort flag for when note ordered
	int lastCount = count;
	int i = 0;

	pNPrecordTag recordTag = NULL;
	pNPtags tags = &data->io.gl.hud.tags;

	// printf("----npSetNodeTag----/n"); // temp, lde
	// using a simple loop for now, replace with HASH method or presort,		//zz debug, was i=0
	// starts searching at the last location accessed, rolls over if not found
	// this procedure is fast when nodes processed in order of record_id
	// and tags are pre-sorted by record_id and rouped by table_id
	// method is very slow for missing tags... loops entire list looking	//zz debug
	for (i = 0; i < tags->recordCount; i++)
//	for (i = tagsIndex; i < tags->recordCount; i++)
	//	for (i=count; i < data->io.gl.hud.tags.recordCount; i++)
	{
		recordTag = tags->recordList[i];
		if (recordTag == NULL)
		{
			printf("err 2985 - npSetNodeTag null tag in recordList\n");
			continue;
		}

		// if we find a matching record and table then replace contents
		if ( node->recordID == recordTag->recordID
			&& node->tableID == recordTag->tableID )
		{
			strcpy (node->tag->title, recordTag->title);
			strcpy (node->tag->desc, recordTag->desc);
			node->tag->titleSize = recordTag->titleSize;
			node->tag->descSize = recordTag->descSize;

			i = tags->recordCount;	// exit loop
		}
		else
		{
			tagsIndex++;			//zzhp keeps track of our current position
			recordTag = NULL;
		}

		//zz debug, could add err checking if out of order then sort and re-process
		count++;
	}

/*
	// if not found in last half of list then check first half
	if (recordTag == NULL)
	{
		for (i=0; i < lastCount; i++)
		{
			recordTag = data->io.gl.hud.tags.recordList[i];

			//if we find a matching record and table then replace contents
			if (node->recordID == recordTag->recordID
				&& node->tableID == recordTag->tableID )
			{
				strcpy (node->tag->title, recordTag->title);
				strcpy (node->tag->desc, recordTag->desc);
				node->tag->titleSize = recordTag->titleSize;
				node->tag->descSize = recordTag->descSize;

				i = lastCount;	//exit loop
			}
			else
				recordTag = NULL;
			//zz debug, could add err checking if out of order then sort and re-process
		}
		count = i;
	}
*/
	
	//if no tag matches the recordID and tableID
	if (recordTag == NULL)
	{
		node->tag->titleSize = sprintf (node->tag->title, "record: %d", 
										node->recordID);
		node->tag->desc[0] = '\0';
		node->tag->descSize = 0;
	}
}


//------------------------------------------------------------------------------
void npTagNode (pNPnode node, void* dataRef)
{
	//if tag is null then allocate memory and init a tag
	if (node->tag == NULL)
	{
		node->tag = malloc (sizeof(NPtag));
		npInitTextTag (node->tag, dataRef);
	}

	//if no recordID then set the tag title to the nodeID
	if (node->recordID == 0)
		sprintf (node->tag->title, "id: %d", node->id);
	else
		npSetNodeTag (node, dataRef);

	npUpdateTag (node->tag); 
}

//------------------------------------------------------------------------------
void npNodeTraverseTree ( void (*nodeFunc)(pNPnode node, void* dataRef), 
						  pNPnode node, void* dataRef)
{
	int i = 0;

	//process root node last
	nodeFunc(node, dataRef);

	for (i=0; i < node->childCount; i++)
	{
		// recursively call this function
		npNodeTraverseTree (nodeFunc, node->child[i], dataRef);
		
		// apply passed in function operation to node
		nodeFunc(node->child[i], dataRef);
	}
}

//calculate the box size based on text strings and font
//upgrade to calculate live values such as channel mapped attributes, zz
//for example coordinates of a moving object
//------------------------------------------------------------------------------
void npUpdateTag (pNPtag tag)													//zz html replace entire function
{
	int lineCount = 1;			///< @todo add procedures for multi-linem tags
	float charWidth = 9.0f;		//add procedure based on font type
	float charHeight = 15.0f;
	int curs = 9;
				
	/// calculate the tag-label character count
	tag->title[kNPtagTitleMax] = '\0';
	tag->titleSize = strlen(tag->title);

	/// If tag title is an HTML hyperlink then trim to the text portion
	if( tag->title[0] == '<' )
	{
		if( strncmp("<a href=\"", tag->title, 9) == 0 )
		{
			/// Search for the end of the href URL
			while( tag->title[curs] != '\"' && curs < kNPtagTitleMax )
			{curs++;}

			/// Advance to beginning of text content
			curs += 2;
			tag->labelHead = curs;

			while( tag->title[curs] != '<' && curs < kNPtagTitleMax )
			{curs++;}

			tag->labelTail = curs - 1;
		
			tag->labelSize = tag->labelTail - tag->labelHead + 1;
		}
		else
			tag->labelSize = tag->titleSize;
	}
	else
		tag->labelSize = tag->titleSize;

	/// calculate the tag box size	
	tag->boxSize.x = 10.0f + charWidth * (float)tag->labelSize;
	tag->boxSize.y = 6.0f + charHeight * (float)lineCount;					//zz html end
}

//------------------------------------------------------------------------------
int npCompareNodes (const void* a, const void* b)
{
//	const pNPnode nodeA = a;
//	const pNPnode nodeB = b;
	pNPnode nodeA = *(pNPnode*)a;
	pNPnode nodeB = *(pNPnode*)b;

	if (nodeA->tableID != nodeB->tableID)
	{
		if (nodeA->tableID > nodeB->tableID)
			return 1;
	}
	else
	{
		if (nodeA->recordID > nodeB->recordID)
			return 1;

		if (nodeA->recordID == nodeB->recordID)
			return 0;
	}

	//if none of the abover then b > a
	return -1;
}

//------------------------------------------------------------------------------
int npCompareTags (const void* a, const void* b)
{
	pNPtag tagA = *(pNPtag*)a;
	pNPtag tagB = *(pNPtag*)b;

	if (tagA->tableID != tagB->tableID)
	{
		if (tagA->tableID > tagB->tableID)
			return 1;
	}
	else
	{
		if (tagA->recordID > tagB->recordID)
			return 1;

		if (tagA->recordID == tagB->recordID)
			return 0;
	}

	//if none of the abover then b > a
	return -1;
}

//called after loading a new state file or tag list file, performance counts //zzhp
//------------------------------------------------------------------------------
void npWriteTagCSV (void* dataRef)
{
	pData data = (pData) dataRef;

	int i = 0, j = 0;
	int count = 0;
	void** nodes;
						
	//allocate a pair of destination buffers for the nodes and tags
	nodes = npMalloc(0, sizeof(pNPnode) * kNPnodeMax, data);			//data->map.nodeCount
	if (!nodes) return;

	//populate the sort lists, skip over null nodes
	for (i=0; i < kNPnodeMax; i++)	//data->map.nodeCount; i++)
		if (data->map.nodeID[i] != NULL)
			nodes[j++] = data->map.nodeID[i];

	//sort nodes and tags grouped first by table_id and second by record_id
	npSort(nodes, j, sizeof(void*), npCompareNodes, data);

	//loop through all nodes and update with tag
//	for (i=0; i < j; i++)
//		node = nodes[i];

	npFree(nodes, data);
}

//called after loading a new state file or tag list file, performance counts //zzhp
//------------------------------------------------------------------------------
void npSyncTags (void* dataRef)
{
	pData data = (pData) dataRef;

	int i = 0, j = 0;
	int count = 0;
	void** nodes;
	pNPnode node;

	data->map.syncNodes = false;		//reset sync flags
	data->map.syncTagsReady = false;

						
	//allocate a pair of destination buffers for the nodes and tags
	nodes = npMalloc(0, sizeof(pNPnode) * kNPnodeMax, data);			//data->map.nodeCount
	if (!nodes) return;
//	tags = npMalloc(0, sizeof(pNPnode) * kNPnodeMax, data);			//update to use tag count //zz debug
//	if (!tags) return;

	//populate the sort lists, skip over null nodes
	for (i=0; i < kNPnodeMax; i++)	//data->map.nodeCount; i++)
		if (data->map.nodeID[i] != NULL)
			nodes[j++] = data->map.nodeID[i];

	//sort nodes and tags grouped first by table_id and second by record_id
	npSort(nodes, j, sizeof(void*), npCompareNodes, data);
	npSort(data->io.gl.hud.tags.list,
			data->io.gl.hud.tags.count,
			sizeof(void*), npCompareTags, data);	//update to use tag count //zz debug

	//loop through all nodes and update with tag
	tagsIndex = 0;	//reset the index, takes advantage of sorted lists, zzhp
	for (i=0; i < j; i++)
	{	npTagNode(nodes[i], data);
	node = (pNPnode)nodes[i];
	//printf("id: %d  tag: %s\n",  node->id, node->tag->title); // Was flooding console, lde
	}
	printf("tag count J: %d\n", j);
	//loop through all nodes and attach tagPtr based on recordID and tableID
//	for (i=0; i < data->map.nodeRootCount; i++)
//		npNodeTraverseTree (npTagNode, data->map.node[i], dataRef);

	npFree(nodes, data);
}


/// @todo remove npDrawTextTag and replace with npDrawNodeTextTag
//draws the text and optional background box with outline
//------------------------------------------------------------------------------
void npDrawTextTag (pNPtag tag, void* dataRef)
{	
	glColor4ub(tag->boxColor.r,tag->boxColor.g,tag->boxColor.b,tag->boxColor.a);

	glBegin (GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(tag->boxSize.x, 0.0f);
		glVertex2f(tag->boxSize.x, tag->boxSize.y);
		glVertex2f(0.0f, tag->boxSize.y);
	glEnd();

	glColor4ub(tag->lineColor.r, tag->lineColor.g, tag->lineColor.b, tag->lineColor.a);

	glBegin (GL_LINE_LOOP);
		glVertex2f(1.0f, 2.0f);
		glVertex2f(tag->boxSize.x - 2.0f, 2.0f);
		glVertex2f(tag->boxSize.x - 2.0f, tag->boxSize.y - 1.0f);
		glVertex2f(1.0f, tag->boxSize.y - 1.0f);
	glEnd();

	glColor4ub(tag->color.r, tag->color.g, tag->color.b, tag->color.a);

	//offset for text margin inside the background box
	glRasterPos2f (5.0f, 6.0f);

	npGlutDrawString (GLUT_BITMAP_9_BY_15, tag->title);
}

//draws the text and optional background box with outline
//------------------------------------------------------------------------------
void npDrawNodeTextTag (pNPnode node, void* dataRef)
{		
	int idRed = 0, idGrn = 0, idBlu = 0;
	pData data = (pData) dataRef;
	pNPtag tag = node->tag;


	if (node->hide)
		return;

	//allow for picking a node by clicking on the text tag
	if (data->io.gl.pickPass)
	{											//Convert nodeID to RGB triplet
		npIDtoRGB( node->id, &idRed, &idGrn, &idBlu );
		glColor4ub( idRed, idGrn, idBlu, 255 );	//nodeID encoded as pixel color

		glBegin (GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(tag->boxSize.x, 0.0f);
			glVertex2f(tag->boxSize.x, tag->boxSize.y);
			glVertex2f(0.0f, tag->boxSize.y);
		glEnd();

		return;		//only need to draw the quad for pickPass, exit to save time
	}

	//draw background box unless the most basic mode ...ModeHUD
	if (1)//node->tagMode != kNPtagModeHUD)	//too hard to read without box, debug zz
	{
		glColor4ub(tag->boxColor.r,tag->boxColor.g,tag->boxColor.b,tag->boxColor.a);

		glBegin (GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(tag->boxSize.x, 0.0f);
			glVertex2f(tag->boxSize.x, tag->boxSize.y);
			glVertex2f(0.0f, tag->boxSize.y);
		glEnd();
	}

	//either box outline or text drawn the same color as the node, not both
	if (node->tagMode == kNPtagModeBoxOutlineHUD)
	{
		//outline box using node color with fixed 50% opacity, unless HUD item
		if (node->type == kNodeHUD)
			glColor4ub(node->color.r, node->color.g, node->color.b, node->color.a);
		else
			glColor4ub(node->color.r, node->color.g, node->color.b, 128); //50%

		glBegin (GL_LINE_LOOP);
			glVertex2f(1.0f, 2.0f);
			glVertex2f(tag->boxSize.x - 2.0f, 2.0f);
			glVertex2f(tag->boxSize.x - 2.0f, tag->boxSize.y - 1.0f);
			glVertex2f(1.0f, tag->boxSize.y - 1.0f);
		glEnd();

		//using the consoles text color
		glColor4ub (tag->color.r, tag->color.g, tag->color.b, tag->color.a);
	}
	else	//else text color is node color with fixed 80% opacity
		glColor4ub(node->color.r, node->color.g, node->color.b, 205);


	//offset for text margin inside the background box
	glRasterPos2f (5.0f, 6.0f);
	
	/// @todo workaround for dealing with hyperlink tags		//zz html
	if( tag->titleSize != tag->labelSize )
	{
		char* labelText = &tag->title[tag->labelHead];
		tag->title[tag->labelTail + 1] = '\0';
		npGlutDrawString (GLUT_BITMAP_9_BY_15, labelText);
		tag->title[tag->labelTail + 1] = '<';
	}
	else
		npGlutDrawString (GLUT_BITMAP_9_BY_15, tag->title);		//zz html end
}

//Draw the Text Labels (Simple Ring now)
//------------------------------------------------------------------------------
void npDrawNodeTags (void* dataRef)
{
	int i = 0;

	pData data = (pData) npGetDataRef();
	pNPnode node;

	glPushMatrix();

	//offset screen coordinates such that 0,0 is in lower left corner
	glTranslatef(data->io.gl.width * -0.5f, data->io.gl.height * -0.5f, 0.0f);

	for (i=0; i < data->io.gl.hud.tags.count; i++)
	{
		node = data->io.gl.hud.tags.list[i];					//change to sort[], debug zz
		
		if (node == NULL)
		{
			printf ("err 4901 - null node in text tags list");
			continue;
		}

		glPushMatrix();
		
			glTranslatef(node->screen.x, node->screen.y, 0.0f);
			
		//	node->tag = &data->io.gl.hud.fps;

			npDrawNodeTextTag( node, data);
	
		glPopMatrix();
	}
	data->io.gl.hud.tags.count = 0;

	glPopMatrix();
}

//------------------------------------------------------------------------------
void npGlutDrawString (void *font, char *str)
{
	int i = 0, size = 0;
	
	size = strlen (str);
	
	for (i=0; i < size; i++)
		glutBitmapCharacter (font, str[i]);
}

