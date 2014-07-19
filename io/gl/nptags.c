/* -----------------------------------------------------------------------------
*
*  nptag.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2012 by Shane Saxon - contact@saxondigital.net
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

#include "nptags.h"


#ifdef NP_MSW_
	#include <freeglut.h>
#endif
#ifdef NP_OSX_
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
#endif
#ifdef NP_LINUX_
	#include <GL/freeglut.h>
	//#include "../../lib/freeglut/freeglut_2012_02_05/include/GL/freeglut.h"
	//#include "/root/Desktop/freeglut-2.8.0/include/GL/freeglut.h"
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
//------------------------------------------------------------------------------
void npExportTagsToCSV (char* buffer, int* size, int* wordSize, void* dataRef)
{
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
void npAddTag (pNPtextTag tag, void* dataRef)
{
	//insert tag in the list sorted first by tableID then recordID
	//perhaps should lock list during this process for thread safety
	return;
}

//------------------------------------------------------------------------------
pNPtextTag npGetTag (int recordID, int tableID, void* dataRef)
{
	int i = 0;
	pNPtextTag tag = NULL;
	pData data = (pData) dataRef;

	//using a simple loop for now, replace with HASH method, debug zz
//	for (i=0; i < data->map.tags.count; i++)

	return tag;
}

//------------------------------------------------------------------------------
void npSetNodeTag (NPnodePtr node, void* dataRef)
{
	int i = 0;
	pNPrecordTag recordTag = NULL;
	pData data = (pData) dataRef;

	//using a simple loop for now, replace with HASH method, debug zz
	for (i=0; i < data->io.gl.hud.tags.recordCount; i++)
	{
		recordTag = data->io.gl.hud.tags.recordList[i];

		//if we find a matching record and table then copy contents
		if (node->recordID == recordTag->recordID
			&& node->tableID == recordTag->tableID )
		{
			strcpy (node->tag->title, recordTag->title);
			strcpy (node->tag->desc, recordTag->desc);
			node->tag->titleSize = recordTag->titleSize;
			node->tag->descSize = recordTag->descSize;

			i = data->io.gl.hud.tags.recordCount;	//exit loop
		}
		else
			recordTag = NULL;
	}
	
	 //if no tag matches the recordID and tableID
	if (recordTag == NULL)
	{
		node->tag->titleSize = sprintf (node->tag->title, "record_id: %d", 
										node->recordID);
		node->tag->desc[0] = '\0';
		node->tag->descSize = 0;
	}
	else
	{
		
	}
}

//------------------------------------------------------------------------------
void npTagNode (NPnodePtr node, void* dataRef)
{
	//if tag is null then allocate memory and init a tag
	if (node->tag == NULL)
	{
		node->tag = malloc (sizeof(NPtextTag));
		npInitTextTag (node->tag, dataRef);
	}

	//if no recordID then set the tag title to the nodeID
	if (node->recordID == 0)
	{
		sprintf(node->tag->title, "id: %d", node->id);
	}
	else
		npSetNodeTag (node, dataRef);

	npUpdateTextTag (node->tag, dataRef); 
}

//------------------------------------------------------------------------------
void npNodeTraverseTree ( void (*nodeFunc)(NPnodePtr node, void* dataRef), 
						  NPnodePtr node, void* dataRef)
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
void npUpdateTextTag (pNPtextTag tag, void* dataRef)
{
	int length = 0;
	int lineCount = 1;
	float charWidth = 9.0f;		//add procedure based on font type
	float charHeight = 15.0f;

	length = strlen(tag->title);
	
	//add procedure to count lines and width length
	//strlen(tag->desc);

	tag->boxSize.x = 10.0f + charWidth * (float)length;
	tag->boxSize.y = 6.0f + charHeight * (float)lineCount;
}


//called after loading a new state file or tag list file
//------------------------------------------------------------------------------
void npSyncTags (void* dataRef)
{
	int i = 0;
	pData data = (pData) dataRef;

	//loop through all nodes and attach tagPtr based on recordID and tableID
	for (i=0; i < data->map.nodeRootCount; i++)
		npNodeTraverseTree (npTagNode, data->map.node[i], dataRef);
}


//draws the text and optional background box with outline
//------------------------------------------------------------------------------
void npDrawTextTag (pNPtextTag tag, void* dataRef)
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
	glutBitmapString (GLUT_BITMAP_9_BY_15, tag->title);
}

//draws the text and optional background box with outline
//------------------------------------------------------------------------------
void npDrawNodeTextTag (NPnodePtr node, void* dataRef)
{		
	int idRed = 0, idGrn = 0, idBlu = 0;
	pData data = (pData) dataRef;
	pNPtextTag tag = node->tag;


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
		//outline box using node color with fixed 50% opacity
		glColor4ub(node->color.r, node->color.g, node->color.b, 128);

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
	
	glutBitmapString (GLUT_BITMAP_9_BY_15, tag->title);
}

//Draw the Text Labels (Simple Ring now)
//------------------------------------------------------------------------------
void npDrawNodeTags (void* dataRef)
{
	int i = 0;

	pData data = (pData) npGetDataRef();
	NPnodePtr node;

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

