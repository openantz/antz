/* -----------------------------------------------------------------------------
*
*  npgldraw.c
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

#include "npgldraw.h"

#include "npgeometry.h"
#include "nptags.h"

#include "../../npdata.h"
#include "../../npctrl.h"
#include "../npglut.h"


void DrawDefault (NPnodePtr node);
void DrawCamera (NPnodePtr node);
void DrawSurface (NPnodePtr node);
void DrawPoints (NPnodePtr node);
void DrawPin (int selected, NPnodePtr node, void* dataRef);
void DrawPinChild (NPnodePtr node, void* dataRef);
void DrawVideo (NPnodePtr node);
void DrawGrid (NPnodePtr node, void* dataRef);
void npDrawSort (void* dataRef);
NPfloatXYZ npProjectWorldToScreen (const pNPfloatXYZ offset);
void npDrawNodeTags (void* dataRef);

//------------------------------------------------------------------------------
void npInitGLDraw(void* dataRef)
{
	pData data = (pData) dataRef;

//	InitTorus (dataRef);

	data->io.gl.pickPass = false;			//zz global init
}


//------------------------------------------------------------------------------
void DrawDefault (NPnodePtr node)
{
	return;
}


// draw an icon at the location of the camera in the scene
//------------------------------------------------------------------------------
void DrawCamera (NPnodePtr node)
{
	// draw icon of camera in the scene, but not the current viewport camera
	return;
}


//------------------------------------------------------------------------------
void DrawSurface (NPnodePtr node)
{
	// draw ground
	glPushMatrix();

	// position the node
	glTranslatef (node->translate.x, node->translate.y, node->translate.z);
	glRotatef (node->rotate.y, 0.0f, 0.0f, 1.0f);
	glRotatef (node->rotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef (node->rotate.z, 0.0f, 0.0f, 1.0f);

	glScalef (node->scale.x, node->scale.y, node->scale.z);

	glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

	glBegin(GL_QUADS);
		glVertex3f (-100.0f, -100.0f, 0.0f);
		glVertex3f (-100.0f,  100.0f, 0.0f);
		glVertex3f ( 100.0f,  100.0f, 0.0f);
		glVertex3f ( 100.0f, -100.0f, 0.0f);
	glEnd();

	glPopMatrix();
}


//------------------------------------------------------------------------------
void DrawPoints (NPnodePtr node)
{
	return;
}


//MB//
// converts the ID value to an RGB value for picking objects by color
//------------------------------------------------------------------------------
void npIDtoRGB( int id, int* r, int* g, int* b )
{
	int remain = id;
	
	*r = remain / (256*256);	remain = remain - *r * (256*256);
	*g = remain / 256;			remain = remain - *g * 256;
	*b = remain;
}

//convert RGB pixel value back to the encoded node ID
int npRGBtoID( int r, int g, int b )
{
	int id;
	id = (256*256)*r + 256*g + b;

	return id;
}
//MB-End//


//calculate screen position and add to tags draw list if on-screen
//------------------------------------------------------------------------------
void npAddTagToDraw (NPnodePtr node, void* dataRef)
{
	pData data = (pData) dataRef;

	//get screen coordinates
	node->screen = npProjectWorldToScreen (&node->tagOffset);

	//if true then exit, the node is behind the camera
	if (node->screen.z < 0.0f || node->screen.z > 1.0f)
		return;

	//exit if tag is not on-screen
	if ( node->screen.x < 0 || node->screen.x > data->io.gl.width
		|| node->screen.y < 0 || node->screen.y > data->io.gl.height )
		return;

	//add to tags draw list
	if (data->io.gl.hud.tags.count < kNPtagListMax)
		data->io.gl.hud.tags.list[data->io.gl.hud.tags.count++] = node;
}


//------------------------------------------------------------------------------
void DrawPin (int selectedRootNode, NPnodePtr node, void* dataRef)
{
	int i = 0;
	int idRed = 0, idGrn = 0, idBlu = 0;

	pData data = (pData) dataRef;

	NPnodePtr nodeChild = data->map.currentNode;
	NPnodePtr rootGrid = data->map.node[kNPnodeRootGrid];

	//if pickPass render scene with node ID encoded as color	//MB//
	if( data->io.gl.pickPass )
	{											//Convert nodeID to RGB triplet
		npIDtoRGB( node->id, &idRed, &idGrn, &idBlu );
		glColor4ub( idRed, idGrn, idBlu, 255 );	//ID of Node is encoded on the Pixel! In RGB Triplet   (Range: 0-  16.7 Million)
	}
	else
		glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a); //-


	glPushMatrix();

	// position the node and scale the z position based on root grid scale
	glTranslatef (node->translate.x, node->translate.y, 
					node->translate.z * rootGrid->scale.z);

	// rotate
	glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
	glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
	glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

	//scale
	glScalef (node->scale.x, node->scale.y, node->scale.z);

	glLineWidth (node->lineWidth);

	//draw node
	if (node->geometry == kNPprimitiveSolidPin)
	{
		npGLSurface (true, node, data);	
		glTranslatef (0.0f, 0.0f, 5.0f);			//should this be here, debug, zz
	}
	else
	{
		glTranslatef (0.0f, 0.0f, 5.0f);
		npGLSurface (true, node, data);
	}

	glLineWidth(1.0f);	//zz

	if (node->selected || selectedRootNode)
	{
		glPushMatrix();

		if (selectedRootNode)
		{
			// slide down a little for scaling up to surround the pin
			glTranslatef (0.0f, 0.0f, -5.10f);
			glScalef (1.15f, 1.15f, 1.15f);

			if ( node->selected )
			{
				if( !data->io.gl.pickPass )
					glColor4f (1.0f, 1.0f, 0.0f, 0.8f);	// draw selected yellow
				npGLPrimitive (kNPprimitiveWirePin, 0.0f);
			}
			else if (node->branchLevel == 0)
			{
				if( !data->io.gl.pickPass )
					glColor4f ( 1.0f, 0.0f, 0.0f, 1.0f);	// current node red
				npGLPrimitive (kNPprimitiveWirePin, 0.0f);
			}

			if (node->branchLevel > 0 && !data->io.gl.pickPass)
			{
				if (nodeChild->selected)									//zz debug
					glColor4f (1.0f, 1.0f, 0.0f, 0.8f);	// draw selected yellow
				else
					glColor4f (1.0f, 0.0f, 0.0f, 1.0f);	// current node red
			}

			glTranslatef (0.0f, 0.0f, 5.0f);
			glScalef (1.0f, 1.0f, 0.66666667f);				// squish height, z 
			npGLPrimitive (kNPprimitiveWirePin, 0.0f);	// 2nd one will be above
		}
		else if (node->selected)
		{
			if( !data->io.gl.pickPass )
				glColor4f (1.0f, 1.0f, 0.0f, 0.8f);		// draw selected as yellow

			glTranslatef (0.0f, 0.0f, -5.10f);
			glScalef (1.05f, 1.05f, 1.05f);
			npGLPrimitive (kNPprimitiveWirePin, 0.0f);
		}

		glPopMatrix();
	}

	//if tagMode then calculate screen position and add to tags draw list
	if (node->tagMode)
	{
		glPushMatrix();
			glTranslatef (0.0f, 0.0f, -5.0f);
			npAddTagToDraw (node, data);
		glPopMatrix();
	}

	//should call DrawNode, that calls draw pin, move this to drawnodes...   debug, zz
	// recursively
	for (i=0; i < node->childCount; i++)
		DrawPinChild (node->child[i], dataRef);


	glPopMatrix();
}


// first draw child here, then break it up, draw nodes should iterate down, debug, zz
// the tree branches and take care of selected glows, etc...
// this routine should be generic regardless of branch level
//------------------------------------------------------------------------------
#define kNPinvertDeg 0.005555555555f	//equals 1/180

void DrawPinChild (NPnodePtr node, void* dataRef)
{
	int i = 0;
	int idRed = 0, idGrn = 0, idBlu = 0;

	pData data = (pData) dataRef;

	NPnodePtr parent = node->parent;
	NPpinPtr nodeData = (NPpinPtr) node->data;

	if (parent == NULL)								//take this out		debug zz	
		return;

	glPushMatrix();

	//Main Color for Rendering Pin
	if( data->io.gl.pickPass )
	{
		npIDtoRGB( node->id, &idRed, &idGrn, &idBlu );
		glColor4ub( idRed, idGrn, idBlu, 255 );	//ID of Node is encoded on the Pixel! In RGB Triplet   (Range: 0-  16.7 Million)
	}
	else
		glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

	// position based on parent topo type
	if (parent->topo == kNPtopoCube)
	{
		//if zero then calculate using id/6 + 1		//update to use parent->childIndex, debug zz
		if (node->facet == 0)
			i = node->id - 6 * (node->id / 6) + 1;	// +1 to start at 1
		else
			i = node->facet;

		// rotate orthogonal to face based on facet then translate
		switch (i) // 
		{
			case 1 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//pos x	front
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 2 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//neg x	back
				glRotatef (180.0f, 1.0f, 0.0f, 0.0f);
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 3 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//pos y	right
				glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 4 :
				glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		//neg y	left
				glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
				glRotatef (90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 5 : 									//pos z top, no rotation
			//	glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case 6 :
			//	glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);
				glRotatef (180.0f, 0.0f, 1.0f, 0.0f);	//neg z	bottom
				break;
			default : break;
		}
		glTranslatef (0.0f, 0.0f, kNPcubeLength);	//translate from center to cube surface, explain, debug zz
		glScalef (kNPinvertDeg * kNPcubeLength, kNPinvertDeg * kNPcubeLength, 
					kNPinvertDeg * kNPcubeLength); //scale face +/- 180 deg
		
		//now position node specific coordinates
		glTranslatef (node->translate.x, node->translate.y, node->translate.z);

		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
	//	glRotatef (node->rotate.z * kRADtoDEG, 0.0f, 0.0f, 1.0f);

		glScalef (90.0f / kNPcubeLength, 90.0f / kNPcubeLength, 90.0f / kNPcubeLength);	//scale node up to half parent size
		glScalef (node->scale.x, node->scale.y, node->scale.z);	//node scale
	}
	else if (parent->topo == kNPtopoSphere)
	{
	//	glRotatef (node->rotate.y * kRADtoDEG, 0.0f, 0.0f, 1.0f);	//CSV ver 1 debug zz
		//position
		glRotatef (node->translate.x + 180.0f, 0.0f, 0.0f, 1.0f);	//longitude, +180 so N = 0
		glRotatef (node->translate.y - 90.f, -1.0f, 0.0f, 0.0f);	//latitude

		//translate 1 unit to surface then convert node z local units
		glTranslatef ( 0.0f, 0.0f, 1.0f + node->translate.z / (4.0f*kRADtoDEG));
		
		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

		//scale 1/2 size of parent
		glScalef (node->scale.x * 0.25f, node->scale.y * 0.25f,
					node->scale.z * 0.25f);	//node scale
//		glScalef (node->scale.x * 0.3536f, node->scale.y * 0.3536f,
//					node->scale.z * 0.3536f);	//node scale
	}
	else if (parent->topo == kNPtopoPin )
		//|| ( parent->topo == 0 && parent->geometry == kNPglutWireTorus) )	//default
	{
		//position the node vertically on the pin, note using translate.x
		if (node->branchLevel == 1)
		{
			glTranslatef (0.0f, 0.0f, node->translate.x / 37.22f);

			if ( node->topo == kNPtopoPin)// || (node->topo == 0 ))
				glScalef (node->scale.x * 0.5f, node->scale.y * 0.5f, node->scale.z * 0.5f);
			else
				glScalef (node->scale.x, node->scale.y, node->scale.z);
		}
		else
		{
			glTranslatef (0.0f, 0.0f, kNPpinHeight + node->translate.x / 37.22f);

			//scale by node size and additional factor to reduce size
			if ( node->topo == kNPtopoPin)// || (node->topo == 0 ))
				glScalef (node->scale.x * 0.5f, node->scale.y * 0.5f, node->scale.z * 0.5f);
			else
				glScalef (node->scale.x, node->scale.y, node->scale.z);
		}

		//offset from center after scaling, preserves ring offset when scaling
		glTranslatef (node->translate.z * .008333333f, //kRADtoDEG,	//scale 180 to ground
					 0.0f, 0.0f); // node->translate.y / -37.22f, 0.0f); //locked y, makes more sense when swtiching topos

//		if (parent->topo == kNPtopoPin)
//			glRotatef (90.0f, 0.0f, 0.0f, 1.0f);

		//rotate
	//	glRotatef (90.0f, 0.0f, 0.0f ,1.0f);	//orient x = 0 to north
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
	}
	else if (node->branchLevel > 1 || parent->topo == kNPtopoTorus) //topo torus
	{
		//scale up 1.5X to match torus geometry size inner radius
		glScalef (kNPtorusRadius, kNPtorusRadius, kNPtorusRadius); 
		
		//position at torus outer radius, inside center of tube
		glRotatef (node->translate.x + 90.0f, 0.0f, 0.0f, 1.0f);
//		glRotatef (node->translate.x, 0.0f, 0.0f, 1.0f);		//longitude
		glTranslatef (1.0f, 0.0f, 0.0f);	//translate to center of tube

		//?
//		glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
		
		//treat null as torus, later make this depend on the geometry
		if (node->topo == kNPtopoTorus || node->topo == 0)
	//		|| node->geometry == kNPglutWireTorus
	//		|| node->geometry == kNPglutSolidTorus )
		{
			glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
			glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef (node->translate.y, 0.0f, 0.0f, 1.0f);	//latitude
			//translate.z based on scale.x and is converted from deg 57.29...
			glTranslatef (0.0f, 0.1591f * node->scale.x * node->translate.z 
						  / kRADtoDEG, 0.0f);
		}
		else
		{	
			glRotatef (90.0f, 0.0f, 1.0f, 0.0f);		
			glRotatef (node->translate.y, 0.0f, -1.0f, 0.0f);	//latitude

			glRotatef (90.0f, 0.0f, 0.0f, 1.0f);				//orient North
			//translate 1 unit to surface then convert node z local units
			//uses parent->ratio for torus inner radius and 1.5f for scaling factor
			if (node->topo == kNPtopoPin)// || node->geometry == kNPprimitiveSolidPin) // kNPtopoTorus zz
			{
				glTranslatef ( 0.0f, 0.0f,
					parent->ratio + (node->translate.z / kRADtoDEG) );
	//			glRotatef (node->translate.y, 0.0f, -1.0f, 0.0f);	//latitude
				glScalef (0.5f, 0.5f, 0.5f);	//results in 1/4 parent
			}
			else //if (node->topo == kNPtopoNull)
			{
				glTranslatef (0.0f, 0.0f, node->translate.z / kRADtoDEG);
//				glRotatef (node->translate.y, 0.0f, -1.0f, 0.0f);	//latitude
			}

		}
		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);

		//scale 1/2 size of parent, smaller is good for torus .5 / 1.5
		glScalef (node->scale.x * 0.33333333f, node->scale.y * 0.33333333f,
					node->scale.z * 0.33333333f);	//node scale

/*
		//position
		glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef (node->rotateVec.angle, 0.0f, 1.0f, 0.0f);		//x	debug zz
			
		glTranslatef ( (node->translate.x * node->scale.x) + 1.5f,
						node->translate.y * node->scale.y,
						0.0f );								// z not translated

		//orientation
		glRotatef (node->rotate.y * kRADtoDEG, 0.0f, 0.0f, 1.0f);		//y, debug zz
		glRotatef (node->rotate.z * kRADtoDEG, 1.0f, 0.0f, 0.0f);		//y, debug zz

		glScalef ( 0.5f * node->scale.x, 0.5f * node->scale.y, 0.5f * node->scale.z);
*/	
	}
//	else if(node->geometry == kNPprimitiveSolidPin)// process pins, DrawPin(node, dataRef);
//	{
//		DrawPin (false, node, dataRef);
//	}	
	else //assume primary torus with parent as pin
	{
		if (node->branchLevel == 1)
		{
			if (parent->topo == 0 || parent->topo == kNPtopoPin)
			{	
				glTranslatef (0.0f, 0.0f, node->translate.x / 37.22f);
				if (node->topo == kNPtopoPin)
					glScalef (0.5f, 0.5f, 0.5f);
			}
			else if (parent->topo == kNPtopoTorus)
			{
				glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
				//translate.z based on scale.x and is converted from deg 57.29...
				glTranslatef (0.3722f * node->scale.x * node->translate.z 
							  / kRADtoDEG, 0.0f, 0.0f);
				glRotatef (node->translate.y, 0.0f, 0.0f, 1.0f);	//latitude
			}
			else
				glTranslatef (node->translate.x * node->scale.x,
						node->translate.y * node->scale.y,
						node->translate.z );				// z is not scaled
		}
		else	
			glTranslatef (node->translate.x * node->scale.x,
						node->translate.y * node->scale.y,
						node->translate.z );				// z is not scaled
		
	//	glRotatef (node->rotate.y * kRADtoDEG, 0.0f, 0.0f, 1.0f);		//y, debug zz
	//	glRotatef (90.0f, 0.0f, 0.0f, 1.0f);	//90 deg orient North
	
		//orientation
		glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
		glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
		glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
		
		glScalef (node->scale.x, node->scale.y, node->scale.z);	//root child
	}


	// draw the node unless hidden
	if (!node->hide)
	{
		glLineWidth(node->lineWidth);

		if (node->branchLevel > 1)
		{
			if (node->topo == kNPtopoPin && node->geometry != kNPprimitiveSolidPin)
			{
				glTranslatef (0.0f, 0.0f, 5.0f);
				npGLSurface (true, node, data);
				glTranslatef (0.0f, 0.0f, -5.0f);
			}
			else
			{
				npGLSurface (true, node, data);
			//	glTranslatef (0.0f, 0.0f, 5.0f);
			}
		}
		else
			npGLSurface (true, node, data);	

		glLineWidth(1.0f);
	}

	// draw a selection status wireframes slightly scaled up on top...
	if (node->selected)
	{	
		glPushMatrix();
			// draw nodeID color on pickPass to allow picking on the wireframe
			if( !data->io.gl.pickPass )
				glColor4f (0.8f, 0.8f, 0.0f, 1.0f);			//draw yellow
			glScalef (1.03f, 1.03f, 1.03f);
			npGLPrimitive (kNPglutWireTorus, node->ratio);
		glPopMatrix();
	}

	// draw a wireframe slightly scaled up on top...
	if (node == data->map.selectedPinNode)
	{	
		glPushMatrix();
			// draw nodeID color on pickPass to allow picking on the wireframe
			if( !data->io.gl.pickPass )
			{
				if (node->freeze)
					glColor4f (0.0f, 1.0f, 1.0f, 0.6f);			//draw cyan
				else
					glColor4f (1.0f, 0.0f, 0.0f, 1.0f);		//draw red
			}
			glScalef (1.04f, 1.04f, 1.04f);
			npGLPrimitive (kNPglutWireTorus, node->ratio);
		glPopMatrix();
	}
	
	//if tagMode then calculate screen position and add to tags draw list
	if (node->tagMode)
		npAddTagToDraw(node, data);

	// draw the child nodes recursively
	for (i=0; i < node->childCount; i++)
		DrawPinChild (node->child[i], data);
		
	glPopMatrix();
}


//------------------------------------------------------------------------------
void DrawVideo (NPnodePtr node)
{
	return;
}


//------------------------------------------------------------------------------
void DrawGrid (NPnodePtr node, void* dataRef)
{
	int			i = 0;
	int			j = 0;
	int			k = 0;
	int			idRed = 0, idGrn = 0, idBlu = 0;
	float		gridOffset = -0.4f;			//obscured grid line workaround, debug zz
	GLfloat		x = 0.0f;
	GLfloat		y = 0.0f;
	GLfloat		z = 0.0f;
	NPfloatXYZ	length;

	pData data = (pData) dataRef;
	NPgridPtr grid = (NPgridPtr) node->data;
	NPnodePtr camNode = data->map.activeCam;

	//MB-TEXTURE
	static GLfloat sgenparams[] = { 1.0f,  0.0,  0.0,  0.0 };
	static GLfloat tgenparams[] = { 0.0,  1.0f,  0.0,  0.0 };
	//MB-END

	//if pickPass then don't draw grid
	if (data->io.gl.pickPass)// && data->map.nodeRootIndex != kNPnodeRootGrid) debug, zz
		return;

	//turn two-sided rendering of polygon faces on
	if (node->branchLevel > 0)
		glDisable (GL_CULL_FACE);

	glDisable (GL_LIGHTING);	//draw 100% ambient white
	
	glPushMatrix();				//restore after calling child nodes

	//if not pickPass then enable texture mapping
	if (!data->io.gl.pickPass)
	{
		glEnable ( GL_TEXTURE_2D );
		glBindTexture ( GL_TEXTURE_2D, node->textureID );
		//using GL_MODULATE instead of GL_DECAL to allow alpha with RGBA textures
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glDisable( GL_TEXTURE_GEN_S );
		glDisable( GL_TEXTURE_GEN_T );
		glTexGenfv( GL_S, GL_OBJECT_PLANE, sgenparams );
		glTexGenfv( GL_T, GL_OBJECT_PLANE, tgenparams );
	}
	else
	{	// if pickPass then no texturing and set color by id
		npIDtoRGB (node->id, &idRed, &idGrn, &idBlu);
		printf("r: %d   g: %d   b: %d\n", idRed, idGrn, idBlu);
		glColor4b (idRed, idGrn, idBlu, 255);		//draw quad color mapped as id
	}

	//used to calculate grid center based on segment count and spacing
	length.x = grid->spacing.x * node->segments.x;
	length.y = grid->spacing.y * node->segments.y;

	// position node
	glTranslatef (node->translate.x, node->translate.y, node->translate.z);

	glRotatef (node->rotate.y, 0.0f, 0.0f, -1.0f);
	glRotatef (node->rotate.x, -1.0f, 0.0f, 0.0f);
	glRotatef (node->rotate.z, 0.0f, 0.0f, -1.0f);
		
	glScalef (node->scale.x, node->scale.y, node->scale.z);

	//center based on length
	glTranslatef (length.x * -0.5f, length.y * -0.5f, 0.0f);


	//Draw Texture Quad
	if (node->textureID)	//allow for click on background tex's for mouse flying
	{
		//if not pickPass then enable and bind texture, set color to white
		if (!data->io.gl.pickPass)
		{
			if (node->branchLevel > 0)	//child grids use opacity for textures
				glColor4ub (255, 255, 255, node->color.a);
			else
				glColor4f (1.0f, 1.0f, 1.0f, 1.0f);	 //root grid is fully opaque
		}

		//draw the quad
		glBegin (GL_QUADS);
			glTexCoord2d(0.0,0.0); glVertex3d(0.0f,  0.0f, gridOffset);	 // center quad
			glTexCoord2d(1.0,0.0); glVertex3d(length.x, 0.0f, gridOffset);
			glTexCoord2d(1.0,1.0); glVertex3d(length.x, length.y, gridOffset);
			glTexCoord2d(0.0,1.0); glVertex3d(0.0f,  length.y, gridOffset);
		glEnd();

		glDisable ( GL_TEXTURE_2D );
	}

	//draw grid lines unless hidden
	if (!node->hide)
	{
		// if not pickPass then set the node color
		if (!data->io.gl.pickPass)
			glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);

		//draw grid lines on top of texture map
//		glDisable (GL_DEPTH_TEST);					//debug
		glLineWidth (node->lineWidth);

		//draw grid lines
		glBegin (GL_LINES);
			for (i=0; i <= node->segments.z; i++)
			{
				//need to do this before rest of grid lines, otherwise no showup
				//if currentNode then draw yellow selection box
				if (node == data->map.currentNode)
				{
					glColor4ub(255,255,0,192);	//yellow, 75% opacity

					glVertex3f (0.0f, 0.0f, z);			//side one
					glVertex3f (length.x, 0.0f, z);

					glVertex3f (length.x, 0.0f, z);		//two
					glVertex3f (length.x, length.y, z);

					glVertex3f (length.x, length.y, z);	//three
					glVertex3f (0.0f, length.y, z);

					glVertex3f (0.0f, length.y, z);		//four
					glVertex3f (0.0f, 0.0f, z);

					glVertex3f (length.x * 0.5f, 0.0f, z);	 //cross hairs
					glVertex3f (length.x * 0.5f, length.y, z);

					glVertex3f (0.0f, length.y * 0.5f, z);	 //cross hairs
					glVertex3f (length.x, length.y * 0.5f, z);

					// restore node color 
					glColor4ub (node->color.r, node->color.g, node->color.b, node->color.a);
				}

				for (j=0; j <= node->segments.x; j++)
				{
					glVertex3f (x, 0.0f, z);
					glVertex3f (x, length.y, z);
					x += grid->spacing.x;
				}
				for (j=0; j <= node->segments.y; j++)
				{
					glVertex3f (0.0f, y, z);
					glVertex3f (length.x, y, z);
					y += grid->spacing.y;
				}

				x = 0.0f;						//reset x and y
				y = 0.0f;
				z += grid->spacing.z;			//set the z for 3D stacked grids
			}
		glEnd();

		glLineWidth (1.0f);
//		glEnable (GL_DEPTH_TEST);
	}

	//turn two-sided rendering of polygon faces off
	if (node->branchLevel > 0)
		glEnable (GL_CULL_FACE);

	// if not pickPass then restore lighting
	if (!data->io.gl.pickPass)
		glEnable (GL_LIGHTING);

	//undo center based on length
	glTranslatef (length.x * 0.5f, length.y * 0.5f, 0.0f);

	//do not want scaling of root grid to effect children, single case
	if (node == data->map.node[kNPnodeRootGrid])
		glPopMatrix();				//restore for root grid only

	//recursively traverse and draw children
	for (i=0; i < node->childCount; i++)
		DrawGrid(node->child[i], data);

	//for all other nodes the transformations are inherited
	if (node != data->map.node[kNPnodeRootGrid])
		glPopMatrix();				// restore after traversing child nodes
}


//draw console GUI messages based on mode
//------------------------------------------------------------------------------
void npDrawConsole (void* dataRef)
{
	int i = 0;
	int index = 0;
	char* str = NULL;

	pData data = (pData) dataRef;
	pNPconsole console = (pNPconsole) &data->io.gl.hud.console;

	//exit to not draw if mode is null
	if (!console->mode)		//update to use the hide 
		return;

	glColor4ub(console->boxColor.r, console->boxColor.g, console->boxColor.b, console->boxColor.a);

	glBegin (GL_QUADS);
		glVertex2f(console->screen.x, console->screen.y);
		glVertex2f(console->screen.x + console->box.x, console->screen.y);
		glVertex2f(console->screen.x + console->box.x, console->screen.y + console->box.y);
		glVertex2f(console->screen.x,  console->screen.y + console->box.y);
	glEnd();

	glColor4ub(console->color.r, console->color.g, console->color.b, console->color.a);

	switch (console->mode)
	{
		case kNPconsoleModeOne :
			//offset to create text margin inside the box
			glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f);
			glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[console->lineIndex][0]);
			break;
		case kNPconsoleModeThree :
			glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f);
			glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[console->lineIndex][0]);
	
			//now print previous two lines above current line, handles buffer roll-over
			if (console->lineIndex > 1)
			{
				glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f + 15.0f);
				glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[console->lineIndex - 1][0]);
				glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f + 30.0f);
				glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[console->lineIndex - 2][0]);
			}
			else if (console->lineIndex > 0)
			{
				glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f + 15.0f);
				glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[0][0]);
				glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f + 30.0f);
				glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[kNPconsoleLineMax - 1][0]);
			}
			else	//index == 0
			{
				glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f + 15.0f);
				glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[kNPconsoleLineMax - 1][0]);
				glRasterPos2f (console->screen.x + 4.0f, console->screen.y + 7.0f + 30.0f);
				glutBitmapString (GLUT_BITMAP_9_BY_15, &console->line[kNPconsoleLineMax - 2][0]);
			}
			break;
		case kNPconsoleModeMax :
			index = console->lineIndex;
			for (i=0; i < kNPconsoleLineMax; i++) //kNPconsoleLineMax
			{
				str = &console->line[index--][0];
				glRasterPos2f (console->screen.x + 4.0f, 
								console->screen.y + 7.0f + 15.0f * (float)i);
				glutBitmapString (GLUT_BITMAP_9_BY_15, str);

				if (index < 0)
					index = kNPconsoleLineMax - 1;	//handles buffer roll-over
			}
			break;
		default : break;
	}
}

//------------------------------------------------------------------------------
void npDrawFPS (void* dataRef)
{
	float fps = 0.0f;

	pData data = (pData) dataRef;
	pNPtextTag tag = &data->io.gl.hud.fps;

	if (data->io.cycleDelta)
		fps = (float) (1.0 / data->io.cycleDelta);

	sprintf (tag->title, "FPS:%6.2f", fps);

	//updates the box size, we really only need to do this once, debug zz
	npUpdateTextTag (tag, dataRef);

	glPushMatrix();
		glTranslatef (data->io.gl.width * 0.5f - 105.0f,
					  data->io.gl.height * 0.5f - 25.0f, 0.0f);
		//now draw the tag
		npDrawTextTag (tag, data);
	glPopMatrix();
}

//------------------------------------------------------------------------------
void npDrawCompass (void* dataRef)
{
	static const NPubyteRGBA grey = {255,255,255,64};		//grey
	static const NPubyteRGBA green = {0,70,0,150};			//green
	static const NPubyteRGBA red = {255,0,0,180};			//red
	static const NPubyteRGBA yellow = {255,255,0,220};		//yellow
	pData data = (pData) dataRef;
	NPnodePtr node = data->map.currentNode;

	//pNPtextTag tag = &data->io.gl.hud.fps;
	NPtextTag heading;	//heading rotate.y
	NPtextTag tilt;		//tile rotate.x
	//NPtextTag roll;		//roll rotate.z
	NPtextTag tagX;	//latitude
	NPtextTag tagY;	//longitude
	NPtextTag tagZ;	//altitude
	NPtextTag mouseMode;
	NPtextTag toolMode;

	npInitTextTag (&heading, dataRef);
	npInitTextTag (&tilt, dataRef);
	//npInitTextTag (&roll, dataRef);
	npInitTextTag (&tagX, dataRef);
	npInitTextTag (&tagY, dataRef);
	npInitTextTag (&tagZ, dataRef);
	npInitTextTag (&mouseMode, dataRef);
	npInitTextTag (&toolMode, dataRef);

	sprintf (heading.title,	"Heading: %6.2f", node->rotate.y);
	sprintf (tilt.title,	"Tilt:    %6.2f", node->rotate.x);
	//sprintf (roll.title,	"Roll:   %6.2f", node->rotate.z);
//	sprintf (coordX.title,	"LONG X:%7.2f", node->translate.x);	//zz add display mode
//	sprintf (coordY.title,	"LAT  Y:%7.2f", node->translate.y);
//	sprintf (coordZ.title,	"ALT  Z:%7.2f", node->translate.z);

	if ( node->scaleRate.x || node->scaleRate.y || node->scaleRate.z 
		|| (node->branchLevel && data->io.mouse.buttonR 
			&& data->io.mouse.pickMode == kNPpickModePin))
	{
		sprintf (tagX.title,	"Scale X:%7.2f", node->scale.x);
		sprintf (tagY.title,	"Scale Y:%7.2f", node->scale.y);
		sprintf (tagZ.title,	"Scale Z:%7.2f", node->scale.z);
	}
	else
	{
		sprintf (tagX.title,	"Coord X:%7.2f", node->translate.x);
		sprintf (tagY.title,	"Coord Y:%7.2f", node->translate.y);
		sprintf (tagZ.title,	"Coord Z:%7.2f", node->translate.z);
	}

	if (data->io.axes.x)
		tagX.boxColor = green;
	if (data->io.axes.y)
		tagY.boxColor = green;
	if (data->io.axes.z)
		tagZ.boxColor = green;

	if (data->io.mouse.pickMode == kNPpickModeCamera)
	{
		strcpy (mouseMode.title, "mode: Camera");
		mouseMode.lineColor = red;
	}
	else if (data->io.mouse.pickMode == kNPpickModeGrid)
	{
		strcpy (mouseMode.title, "mode: Grid  ");
		mouseMode.lineColor = yellow;
	}
	else if (data->io.mouse.pickMode == kNPpickModePin)
	{
		strcpy (mouseMode.title, "mode: Pin   ");
		mouseMode.lineColor = grey;
	}
	else //assume default mode
	{
		strcpy (mouseMode.title, "mode: Default");
		mouseMode.lineColor = grey;
	}

	if (data->io.mouse.tool == kNPtoolHide)
		strcpy (toolMode.title, "tool: Hide  ");
	if (data->io.mouse.tool == kNPtoolInfo)
		strcpy (toolMode.title, "tool: Info  ");

	//updates the box size, we really only need to do this once, debug zz
	npUpdateTextTag (&heading, dataRef);
	npUpdateTextTag (&tilt, dataRef);
	//npUpdateTextTag (&roll, dataRef);
	npUpdateTextTag (&tagX, dataRef);
	npUpdateTextTag (&tagY, dataRef);
	npUpdateTextTag (&tagZ, dataRef);

	npUpdateTextTag (&mouseMode, dataRef);
	npUpdateTextTag (&toolMode, dataRef);


	glPushMatrix();
		glTranslatef (data->io.gl.width * -0.5f + 10.0f,
					  data->io.gl.height * 0.5f - 25.0f, 0.0f);
		//now draw the first tag
			npDrawTextTag (&heading, data);
		glTranslatef (0.0f, -21.0f, 0.0f);
			npDrawTextTag (&tilt, data);
	//	glTranslatef (0.0f, -21.0f, 0.0f);
	//		npDrawTextTag (&roll, data);
		glTranslatef (0.0f, -21.0f, 0.0f);
			npDrawTextTag (&tagX, data);
		glTranslatef (0.0f, -21.0f, 0.0f);
			npDrawTextTag (&tagY, data);
		glTranslatef (0.0f, -21.0f, 0.0f);
			npDrawTextTag (&tagZ, data);

		glTranslatef (0.0f, -24.0f, 0.0f);
			npDrawTextTag (&mouseMode, data);
		if (data->io.mouse.tool)
		{
			glTranslatef (0.0f, -21.0f, 0.0f);
			npDrawTextTag (&toolMode, data);
		}
	glPopMatrix();
}

// draw 2D overlays including grids, text, messages, highlight objects, etc...
//------------------------------------------------------------------------------
void npDrawHUD (void* dataRef)
{
	float sc;
	pData data = (pData) dataRef;
	pNPhud hud = (pNPhud) &data->io.gl.hud;

	glPushMatrix();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	//scale to Screen Coords
	sc = 63.067f / data->io.gl.height;	//63.067f for 35mm fov, upgrade for any fov, debug zz
	glScalef( sc, sc, sc );
	

//	if(!data->io.gl.pickPass) //update to allow picking HUD elements, debug zz
//		npDrawHUD (data);

	if (hud->drawTags)
		npDrawNodeTags(data);				// draws overlays, grids, text... //MB-LABEL

	//these hud items NOT drawn during pickPass
	if (!data->io.gl.pickPass)
	{
		if (hud->drawConsole)
		{
			//sbow these hud elements based on console mode
			if (hud->console.mode > kNPconsoleModeOne)
			{
				if (hud->drawFPS)
					npDrawFPS (data);

				if (hud->drawCompass)
					npDrawCompass (data);
			}

			npDrawConsole (data);
		}
	}

	glPopMatrix();
}


// sets the node RGB color based on index, preserves existing alpha value
//------------------------------------------------------------------------------
void SetIndexColor (NPubyteRGBA *color, int *colorIndex)
{	
	int index = *colorIndex;
									//kNPpaletteSize stored in nptypes.h
	static const GLubyte colorPallete[kNPpaletteSize][3] = {
											{50,  101, 101},
											{0,   255, 0},
											{255,   0, 0},
											{0,     0, 255},
											{255, 255, 0},
											{152,   0, 255},
											{255, 168, 0},
											{0,   255, 255},
											{255,   0, 255},
											{0,   153, 0},
											{185, 153, 102},
											{255, 180, 255},
											{0,   152, 255},
											{185, 255, 0},
											{152,   0, 0},
											{127, 127, 127},
											{127, 127, 255},
											{197,  82, 0},
											{0,     0, 0},
											{255, 255, 255}
											};
	//re-maps out of bounds colorIndex
	if (index >= kNPpaletteSize)
	{
		while (index >= kNPpaletteSize)
			index -= kNPpaletteSize;	//subtracts pallete sized increments
		*colorIndex = index;
	}
	else if (index < 0)
	{
		while (index < 0)
			index += kNPpaletteSize;
		*colorIndex = index;			//adds pallete sized increments		
	}

	//now set the color
	color->r = colorPallete[index][0];
	color->g = colorPallete[index][1];
	color->b = colorPallete[index][2];
}




//MB-Transp
//------------------------------------------------------------------------------
//Distance between two points
float npDistance( NPfloatXYZ vecA, NPfloatXYZ vecB )
{
	float x = vecA.x - vecB.x;
	float y = vecA.y - vecB.y;
	float z = vecA.z - vecB.z;
	
//	float len = x*x + y*y + z*z;
//	len = sqrtf( len );
//	return len;

	return sqrtf( x*x + y*y + z*z );
}

//------------------------------------------------------------------------------
//QSort Compare function
int comparePinsDistanceFromCamera (const void* a, const void* b)
{
	NPnodePtr pinA = *(NPnodePtr*)a;
	NPnodePtr pinB = *(NPnodePtr*)b;
	
	//optimized for the common case where neither a nor b is null
	if( pinA == NULL || pinB == NULL )	//Null element in pinA & pinB: Nothing or Grid or Camera
	{
		if( pinA != NULL )				//Null element in pinB: Nothing or Grid or Camera
			return +1;					//pinB is at beginning of list... therefore pinA is "greater than" pinB. Return +1
		if( pinB != NULL )				//Null element in pinA: Nothing or Grid or Camera
			return -1;					//pinA is at beginning of list... therefore pinA is    "less than" pinB. Return -1
		return 0;						//These are "equal". Both should be at the beginning of the list.
	}

	if( pinA->distFromCamera   >  pinB->distFromCamera )	//pinA is furter from camera than pinB.
		return -1;											//Therefore should be at the beginning of the list. ie "less than"... Return -1
	if( pinA->distFromCamera   <  pinB->distFromCamera )	//pinA is closer from camera than pinB.
		return +1;											//Therefore should be at the end of the list.    ie "greater than"... Return +1
	if( pinA->distFromCamera  ==  pinB->distFromCamera )	//pinA & pinB are exactly the same distance from the camera. (impossible-ish)
		return 0;											//Return 0;

	//return ( *(int*)a - *(int*)b );
	return -1;
}


//------------------------------------------------------------------------------
void npDrawSort(void* dataRef)
{
	int i = 0;

	pData data = (pData) dataRef;

	NPnodePtr node = NULL;
	NPnodePtr camNode = data->map.activeCam;

	int nodeRootCount = data->map.nodeRootCount;

	NPfloatXYZ camVec = camNode->translate;

	//update root node distance from camera
	for( i = 0; i < nodeRootCount; i++ )
	{
		if( i < kNPnodeRootPin )	//Nothing, Camera, Grid
		{
			data->map.sort[i] = NULL;
			continue;
		}
		
		node = data->map.node[i];
		node->distFromCamera = npDistance( camVec, node->translate );

		data->map.sort[i] = node;
	}

	//create z-sorted list, back to front
	//QuickSort based on Distance From Camera, back to front z-sorted
	qsort( data->map.sort, nodeRootCount, sizeof(NPnodePtr),
			comparePinsDistanceFromCamera );
}

//------------------------------------------------------------------------------
void npDrawNodes (void* dataRef)
{
	int i = 0;
	int selected = false;

	NPnodePtr node = NULL;
	pData data = (pData) dataRef;

	int count = data->map.nodeRootCount;

	//z-sort nodes, back to front for proper transparency
	npDrawSort(data);

	//first drawing grids causes improper transparency, debug zz
	//alternately could draw the grid lines first, then the texture after pins
	//this would cause pins to obscure grid lines
	//another approach is to turn on clipping for grid lines and offset, debug zz
//	DrawGrid (data->map.node[kNPnodeRootGrid], data);

	//draw all pin nodes, skip over camera and grids using kNPnodeRootPin
	for( i = kNPnodeRootPin; i < count; i++ )
	{		
		//draw root nodes back to front using z-sorted list
		node = data->map.sort[i];
		
		if (node == data->map.node[data->map.nodeRootIndex]) //use seletedIndex? debug zz
			selected = true;
		else
			selected = false;

		if (!node->hide)				//if not hidden draw node
			switch (node->type)
		{
			case kNodeDefault	: DrawDefault(node); break;
			case kNodeCamera	: DrawCamera (node); break;
			case kNodeSurface	: DrawSurface(node); break;
			case kNodePoints	: DrawPoints (node); break;
			case kNodePin		: DrawPin (selected, node, data); break;
			case kNodeVideo		: DrawVideo  (node); break;
			case kNodeGrid		: DrawGrid   (node, data); break;
		}
	}

	//draw grids last to allow objects behind transparent grids to be seen
	//update this to allow a specified draw order using the node->shader
	//for example, root grid should probably be drawn before nodes and
	//sub-grids after nodes,											 debug zz
	DrawGrid (data->map.node[kNPnodeRootGrid], data);
}


//MB-LABEL
//Project CURRENT Matrix state World position Coordinates onto the Screen
//offset allows tag to be placed relative to node center
//if Z is less then 0.0f or greater then 1.0f then object is behind camera.
//------------------------------------------------------------------------------
NPfloatXYZ npProjectWorldToScreen (const pNPfloatXYZ offset)
{	
	GLdouble screenX, screenY, screenZ;	//Screen Coordinates Output

	NPfloatXYZ screen;

	GLdouble model_matrix[16];
	GLdouble projection_matrix[16];
	GLint viewport[4];

	//Model Matrix Current
	glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
	
	//Projection Matrix Current
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

	//Viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Do the projection
	gluProject( offset->x, offset->y, offset->z, model_matrix, projection_matrix, 
				viewport, &screenX, &screenY, &screenZ );

	screen.x = (float)screenX;
	screen.y = (float)screenY;
	screen.z = (float)screenZ;

	//	objects behind the camera can have valid screen coordinates
	//	z is used to determine if object is behind the camera
	//	printf ("screen z: %f\n", screenZ);

	return screen;
}

