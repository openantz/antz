/* -----------------------------------------------------------------------------
*
*  npgeometry.c
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

#include "npgeometry.h"

#include "npgldraw.h"

#include "../npglut.h"

#define kTorusScreenSizeCount 4
#define kTorusThicknessCount 4

//Just temporary lde
#define TRUE 1

struct NPtorus {
	NPfloatXYZ*		vArray;
	GLint			vArraySize;
	GLint*			quadArray;
	GLint			quadArraySize;
	int				slices;
	int				segments;
	GLfloat			thickness;
};
typedef struct NPtorus NPtorus;
typedef struct NPtorus * NPtorusPtr;

struct NPcircle {
	NPfloatXY*		vArray;
	int				segments;
	GLfloat			radius;
};
typedef struct NPcircle NPcircle;
typedef struct NPcircle * NPcirclePtr;


void InitTorus (void* dataRef);


static GLint pin_dl;
static GLint pin_wire_dl;
static GLint torus_dl;
static GLint torus_wire_dl;
GLint primitiveDL;

GLuint CreatePinDL();
GLuint CreatePinWireDL();
void DrawPinDL();
void DrawPinWireDL();

GLuint CreateTorusDL();
GLuint CreateTorusWireDL();
void DrawTorusDL();
void DrawTorusWireDL(); 

void npDrawTorus (int geometry, GLfloat innerRadius);

void CreateTorus (NPtorusPtr torus);
void CreateCircle (NPcirclePtr circle);
void DeleteCircle (NPcirclePtr circle);

GLuint npCreatePrimitiveDL (void);


//------------------------------------------------------------------------------
void npInitGLPrimitive (void* dataRef)
{
	pin_dl = CreatePinDL();
	pin_wire_dl = CreatePinWireDL();
	torus_dl = CreateTorusDL();
	torus_wire_dl = CreateTorusWireDL();

	primitiveDL = npCreatePrimitiveDL();

	return;
}

//------------------------------------------------------------------------------
void npCloseGLPrimitive (void* dataRef)
{
	return;
}

//used only to draw outline wireframes
//------------------------------------------------------------------------------
void npGLPrimitive (int geometry, float ratio)
{
	glPushMatrix();									//is glPushMatrix necessary, zz debug

	//draw the object using the primitive DL offset by geometry index
	if (geometry == kNPgeoTorus || geometry == kNPgeoTorusWire)
		npDrawTorus (geometry, ratio);
	else
		glCallList (primitiveDL + geometry);

	glPopMatrix();
}

//used to draw all geometry primitive types
//------------------------------------------------------------------------------
void npGLSurface (bool texture, pNPnode node, void* dataRef)
{
	pData data = (pData) dataRef;

	glPushMatrix();									//is glPushMatrix necessary, zz debug

	glLineWidth (node->lineWidth);		//for wireframe objects

	// turn on texture coordinates specific for each primitive type
	if (texture)
		npGLTexture (node, dataRef);

	if (node->geometry == kNPgeoTorus)
		npDrawTorus (node->geometry, node->ratio);
	else
	{
		//slide down 1 unit to center cylinder								//zz debug
		if ( ( node->geometry == kNPgeoCylinder || node->geometry == kNPgeoCylinderWire
			   || node->geometry == kNPgeoCone || node->geometry == kNPgeoConeWire )
	//		&& !( node->topo == kNPtopoRod || node->type == kNodeLink ) )
				||  ( node->topo != kNPtopoPin 
						&& ( node->geometry == kNPgeoPin 
							|| node->geometry == kNPgeoPinWire ) ) )
			glTranslatef (0.0f, 0.0f, -kNPoffsetUnit);
	//	else if (node->geometry == kNPgeoPinWire)
	//		glTranslatef (0.0f, 0.0f, -kNPoffsetPin);

		//draw the object using the primitive DL offset by geometry index
		if (data->io.gl.pickPass)
		{									
			//if odd add 1 to get a solid
			//logic forces draws wireframe as solid during pickPass
			//also an exception for pin which is out of order in geometry list, zz debug
			if (node->geometry == kNPgeoPinWire)
				glCallList (primitiveDL + kNPgeoPin);
			else if (node->geometry % 2 == 0 && node->geometry != kNPgeoPin)
				glCallList (primitiveDL + node->geometry + 1);
			else
				glCallList (primitiveDL + node->geometry);
		}
		else
			glCallList (primitiveDL + node->geometry);
	}
	// turn off texturing
	if (texture && node->textureID)					//dual param for pickPass, debug zz
		glDisable ( GL_TEXTURE_2D );

	glLineWidth (1.0f);				//reset to default of 1.0f line thickness, zz debug

	glPopMatrix();
}


//MB-TEXTURE
//------------------------------------------------------------------------------
void npGLTexture (pNPnode node, void* dataRef)
{
//	const GLfloat sgenparams[] = { 1.0f, 0.0f, 0.0f, 0.0f };
//	const GLfloat tgenparams[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	const GLfloat sgenparams[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat tgenparams[] = { 0.0f, 1.0f, 0.0f, 0.0f };

	pData data = (pData) dataRef;

	//exit if no texture or pickPass, picking requires no texture mapping
	if (!node->textureID || data->io.gl.pickPass)
		return;

	glEnable ( GL_TEXTURE_2D );
	glBindTexture ( GL_TEXTURE_2D, node->textureID );

	//use different texturing for the gluSphere
	if ( node->geometry == kNPgeoSphere // || geometry == kNPgeoSphereWire		//zz debug
		|| node->geometry == kNPgeoCylinder ) // || geometry == kNPgeoCylinderWire
	{
		glDisable( GL_TEXTURE_GEN_S );	//prevents intermittent texture anomally
		glDisable( GL_TEXTURE_GEN_T );
	}
	else
	{
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
			// metal appearance
			// glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
			// glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );

		glEnable( GL_TEXTURE_GEN_S );
		glEnable( GL_TEXTURE_GEN_T );

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

			//rather sharp setting, but probably best for mapping
			//for video use GL_LINEAR_MIPMAP_NEAREST when the angle is close 
			//to perpendicular less artifacts and a bit more blurry
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);		//zz debug, add mipmapping...
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//zz debug, add mipmapping...
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexGenfv( GL_S, GL_OBJECT_PLANE, sgenparams );
		glTexGenfv( GL_T, GL_OBJECT_PLANE, tgenparams );
	}
}


// creates a set of toroids and loads them into GPU memory
//------------------------------------------------------------------------------
void InitTorus (void* dataRef)
{
	int i = 0, j = 0;
	NPtorusPtr torus = NULL;

	torus = (NPtorusPtr) malloc (sizeof(NPtorus));
	if (torus == NULL)
	{
		printf ("\n 4220 error malloc failed cannot write file \n");
		return;
	}

	// 5x5, 7x5, 12x6, 16x6, 24x8, 32x10, 48x12, 64x16, 96x24, 128x32, 196x48
	for ( i = 0; i < kTorusScreenSizeCount; i++)
		for ( j = 0; j < kTorusThicknessCount; j++)
		{
			switch (i)
			{
				case 0 : torus->slices = 5; torus->segments = 5; break;
				case 1 : torus->slices = 5; torus->segments = 7; break;
				case 2 : torus->slices = 6; torus->segments = 12; break;
				case 3 : torus->slices = 6; torus->segments = 16; break;
				case 4 : torus->slices = 8; torus->segments = 24; break;
				case 5 : torus->slices = 10; torus->segments = 32; break;
				case 6 : torus->slices = 12; torus->segments = 48; break;
				case 7 : torus->slices = 16; torus->segments = 64; break;
				case 8 : torus->slices = 24; torus->segments = 96; break;
				case 9 : torus->slices = 32; torus->segments = 128; break;
				case 10 : torus->slices = 48; torus->segments = 196; break;
				case 11 : torus->slices = 64; torus->segments = 256; break;
			}
			
			torus->thickness =	(GLfloat) j * 100.0f / 
								(GLfloat) (kTorusThicknessCount - 1);

			CreateTorus (torus);
			// load torus to GPU and deallocate memory
			// we could save time by allocating enough memory for all torus's at once
			// or enough for largest torus then de-allocating all at once
		}
}


// creates a single torus of specified slices, stacks and thickness
//------------------------------------------------------------------------------
void CreateTorus (NPtorusPtr torus)
{
/*	int i=0, j=0;

	NPcirclePtr ring = (NPcirclePtr) malloc (sizeof(NPcircle));
	NPcirclePtr hoops = (NPcirclePtr) malloc (torus->slices * sizeof(NPcircle));

	torus->vArraySize = torus->slices * torus->segments;
	torus->vArray = (NPfloatXYZPtr) malloc (torus->vArraySize * sizeof(NPfloatXYZ));

	torus->quadArraySize = torus->vArraySize * 4;
	torus->quadArray = (GLint*) malloc (torus->quadArraySize * sizeof(GLint));

	// create our tube ring
	ring->radius = torus->thickness;		// 1.0 is no hole in the donut
	ring->segments = torus->slices;
	CreateCircle (ring);

	// create our hoops, the length segments of the tube
	for (i=0; i < torus->slices; i++)
	{
		hoops[i].radius = 1.0f + ring->vArray[i].x;
		hoops[i].segments = torus->segments;
		CreateCircle (&hoops[i]);
	}

	// build the vertex array for the torus
	for (i=0; i < torus->segments; i++)
		for (j=0; j < torus->slices; j++)
		{
			torus->vArray[i*torus->slices + j].x = hoops[j].vArray[i].x;		//the hoop provides x and y
			torus->vArray[i*torus->slices + j].y = hoops[j].vArray[i].y;
			torus->vArray[i*torus->slices + j].z = ring->vArray[j].y;		//the ring provides the z
		}

	// build the the torus polygon array
	for (i=0; i < torus->segments; i++)
		for (j=0; j < torus->slices; j++)
		{	
			if ( i == (torus->segments - 1) )		// the last torus segment
			{
				if ( j == (torus->slices - 1) )		// the last polygon
				{
					torus->quadArray[i][j][0] = (i * torus->slices) + j;
					torus->quadArray[i][j][1] = (i * torus->slices);
					torus->quadArray[i][j][2] = 0;
					torus->quadArray[i][j][3] = j;
				}
				else
				{
					torus->quadArray[i][j][0] = (i * torus->slices) + j;
					torus->quadArray[i][j][1] = (i * torus->slices) + (j + 1);
					torus->quadArray[i][j][2] = (j + 1);
					torus->quadArray[i][j][3] = j;
				}
			}
			else
			{
				if ( j == (torus->slices - 1) )		// the last polygon in a segment
				{
					torus->quadArray[i][j][0] = (i * torus->slices) + j;
					torus->quadArray[i][j][1] = (i * torus->slices);
					torus->quadArray[i][j][2] = ((i + 1) * torus->slices);
					torus->quadArray[i][j][3] = ((i + 1) * torus->slices) + j;
				}
				else
				{
					torus->quadArray[i][j][0] = (i * torus->slices) + j;
					torus->quadArray[i][j][1] = (i * torus->slices) + (j + 1);
					torus->quadArray[i][j][2] = ((i + 1) * torus->slices) + (j + 1);
					torus->quadArray[i][j][3] = ((i + 1) * torus->slices) + j;
				}
			}

	DeleteCircle (ring);
	for (i=0; i < torus->slices; i++)
		DeleteCircle (&hoops[i]);
*/
}


// creates a circle of specified number of points and radius
// centered on the origin (0,0) starting at 12 o'clock going CW
// really should be part of GL, glCircle!, lol
//------------------------------------------------------------------------------
void CreateCircle (NPcirclePtr circle)
{
/*	int i = 0;
	GLfloat angle = 0.0, deltaAngle = 0.0f;

	circle->vArray = (NPfloatXY) malloc (cirlce->segments * sizeof(NPfloatXY));


	if (circle->segments)
		deltaAngle = (2.0 * kPI) / circle->segments;
	
	// go around the circle, 
	for (angle = 0.0f; angle <= 1.99999f * kPI; angle += deltaAngle)
	{
//		circle->vArray[i]->x = circle->radius * (GLfloat)sin(angle);
//		circle->vArray[i].y = circle->radius * (GLfloat)cos(angle);
		i++;
	}
*/
}


//------------------------------------------------------------------------------
void DeleteCircle (NPcirclePtr circle)
{
	free (circle->vArray);
	free (circle);
}


// draws a torus of specified size and thickness, stacks and slices determined
// by size in pixels on screen calculated by screen size and z distance
// radius from center of the donut to the center of the tube, thickness 0-100%
//------------------------------------------------------------------------------
void npDrawTorus (int geometry, GLfloat innerRadius)
{
	int torusID = 0;

	// torusID = screenSize + (thickness / numberOfSizes);

	// necessary for backwards compatibility of CSV files prior to Nov 2011
	if (innerRadius == 0.0f)
		innerRadius = kNPdefaultRatio;

	// optimize first with display list for innerRadius using ratio
	// then for screen size based on z depth
	// torusID); debug zz update to support DL array of different ratios, both solid and wireframe
	// calls a DL if the inner radius is 0.0 or 0.1, otherwise draws in direct mode, debug zz
	if (innerRadius == kNPdefaultRatio )
		glCallList (primitiveDL + geometry);
	else
	{
		if (geometry == kNPgeoTorusWire)
			npGLWireTorus (kNPtorusRadius * innerRadius, kNPtorusRadius, 7, 16);	//	7, 16);
		else
			npGLSolidTorus(kNPtorusRadius * innerRadius, kNPtorusRadius, 7, 16);	//	7, 16);
	}
}


// Draw Pin, ice-cream cone shape, width is a 10% the cone height
// the icecream dome sticks up an additional 10% so total height is 110% height
//------------------------------------------------------------------------------
void DrawPinDL() 
{
	glPushMatrix();															//zzoff
	glTranslatef (0.0f, 0.0f, kNPoffsetPin);
	glRotatef (180.0f, 0.0f, 1.0f, 0.0f);
	npGLSolidCone (kNPoffsetPin * 0.1f, kNPoffsetPin, 10, 1);

	glRotatef (-180.0f, 0.0f, 1.0f, 0.0f);	//undo rotation n save on pushMatrix
	npGLSolidSphere (kNPoffsetPin * 0.1f, 10, 10);
	glPopMatrix();															//zzoff
}

// Draw Pin, ice-cream cone shape, width is a 10% the cone height
// the icecream dome sticks up an additional 10% so total height is 110% height
//------------------------------------------------------------------------------
void DrawPinWireDL() 
{
	glPushMatrix();															//zzoff
	glTranslatef (0.0f, 0.0f, kNPoffsetPin);
	glRotatef (180.0f, 0.0f, 1.0f, 0.0f);
	npGLWireCone (kNPoffsetPin * 0.1f, kNPoffsetPin, 10, 1);

	glRotatef (-180.0f, 0.0f, 1.0f, 0.0f);	//undo rotation n save on pushMatrix
	npGLWireSphere (kNPoffsetPin * 0.1f, 10, 10);
	glPopMatrix();															//zzoff
}


//------------------------------------------------------------------------------
void DrawTorusDL() 
{
	npGLSolidTorus (kNPtorusRadius * 0.1f, kNPtorusRadius, 7, 16); //zzhp
}


//------------------------------------------------------------------------------
void DrawTorusWireDL() 
{
	npGLWireTorus (kNPtorusRadius * 0.1f, kNPtorusRadius, 7, 16);
}


//------------------------------------------------------------------------------
GLuint CreatePinDL()
{
	GLuint displayList;

	// Create the id for the list
	displayList = glGenLists(1);

	// start list
	glNewList (displayList, GL_COMPILE);

	// call the function that contains the rendering commands
	DrawPinDL();

	// endList
	glEndList();

	return (displayList);
}


//------------------------------------------------------------------------------
GLuint CreatePinWireDL()
{
	GLuint displayList;

	// Create the id for the list
	displayList = glGenLists(1);

	// start list
	glNewList (displayList, GL_COMPILE);

	// call the function that contains the rendering commands
	DrawPinWireDL();

	// endList
	glEndList();

	return (displayList);
}


//------------------------------------------------------------------------------
GLuint CreateTorusDL()
{
	GLuint displayList;

	// Create the id for the list
	displayList = glGenLists(1);

	// start list
	glNewList (displayList, GL_COMPILE);

	// call the function that contains the rendering commands
	DrawTorusDL();

	// endList
	glEndList();

	return (displayList);
}


//------------------------------------------------------------------------------
GLuint CreateTorusWireDL()
{
	GLuint displayList;

	// Create the id for the list
	displayList = glGenLists(1);

	// start list
	glNewList (displayList, GL_COMPILE);

	// call the function that contains the rendering commands
	DrawTorusWireDL();

	// endList
	glEndList();

	return (displayList);
}

//------------------------------------------------------------------------------
void DrawSphereWireDL()
{
	GLUquadricObj *sphere=NULL;

	sphere = gluNewQuadric();
	gluQuadricDrawStyle (sphere, GLU_LINE);	//GLU_LINE
	gluQuadricTexture (sphere, TRUE);
	gluQuadricNormals (sphere, GLU_FLAT);

	//Making a display list
	// mysphereID = glGenLists(1);
	// glNewList(mysphereID, GL_COMPILE);

	glPushMatrix();							//update to not use a pushMatrix, zz debug
		glRotatef (180.0f, 0.0f, 0.0f, 1.0f);	//orient world texture for coord 0,0
		gluSphere (sphere, 1.0, 24, 12);
	glPopMatrix();

	// glEndList();		
	// gluDeleteQuadric(sphere);				``````````````//zz debug, should use this
}

//------------------------------------------------------------------------------
void DrawSphereDL()
{
	GLUquadricObj *sphere = NULL;

	sphere = gluNewQuadric();
	gluQuadricDrawStyle (sphere, GLU_FILL);
	gluQuadricTexture (sphere, TRUE);
	gluQuadricNormals (sphere, GLU_SMOOTH);

	//Making a display list
	// mysphereID = glGenLists(1);
	// glNewList(mysphereID, GL_COMPILE);

	glPushMatrix();							//update to not use a pushMatrix, zz debug
		glRotatef (180.0f, 0.0f, 0.0f, 1.0f);	//orient world texture for coord 0,0
		gluSphere (sphere, 1.0, 24, 12);		//replace glu with custom optimized
	glPopMatrix();

	// glEndList();
	// gluDeleteQuadric(sphere);						//zz debug, should use this
}

//------------------------------------------------------------------------------
void DrawCylinderWireDL()
{
	GLUquadricObj *gluObject = NULL;

	gluObject = gluNewQuadric();
	gluQuadricDrawStyle (gluObject, GLU_LINE);
	gluQuadricTexture (gluObject, TRUE);		//faster without texture coords, zz debug
	gluQuadricNormals (gluObject, GLU_SMOOTH);

	//Making a display list
	// mysphereID = glGenLists(1);
	// glNewList(mysphereID, GL_COMPILE);

	glPushMatrix();							//update to not use a pushMatrix, zz debug
		glRotatef (180.0f, 0.0f, 0.0f, 1.0f);	//orient world texture for coord 0,0
		gluCylinder (gluObject, 1.0, 1.0, 2.0, 24, 1);
	glPopMatrix();

	// glEndList();
	// gluDeleteQuadric(sphere);				``````````````//zz debug, should use this
}
//------------------------------------------------------------------------------
void DrawCylinderDL()
{
	GLUquadricObj *gluObject = NULL;

	gluObject = gluNewQuadric();
	gluQuadricDrawStyle (gluObject, GLU_FILL);
	gluQuadricTexture (gluObject, TRUE);
	gluQuadricNormals (gluObject, GLU_SMOOTH);

	//Making a display list
	// mysphereID = glGenLists(1);
	// glNewList(mysphereID, GL_COMPILE);

	glPushMatrix();								//update to not use a pushMatrix, zz debug
		glRotatef (180.0f, 0.0f, 0.0f, 1.0f);	//orient world texture for coord 0,0
		gluCylinder (gluObject, 1.0, 1.0, 2.0, 24, 1);
		glRotatef (180.0f, 1.0f, 0.0f, 0.0f);	//flips the disk over
		gluDisk (gluObject, 0.0, 1.0, 24, 1);	//draw bottom disk
		glRotatef (180.0f, 0.0f, 1.0f, 0.0f);	//flip to right-side up
		glTranslatef (0.0f, 0.0f, 2.0f);		//translate to the top
		gluDisk (gluObject, 0.0, 1.0, 24, 1);	//draw top disk
	glPopMatrix();

	// glEndList();
	// gluDeleteQuadric(sphere);				``````````````//zz debug, should use this
}

//------------------------------------------------------------------------------
void DrawConeDL()
{
	GLUquadricObj *gluObject = NULL;

	gluObject = gluNewQuadric();
	gluQuadricDrawStyle (gluObject, GLU_FILL);
	gluQuadricTexture (gluObject, TRUE);		//does not shade nor texture well for a cone, zz debug
	gluQuadricNormals (gluObject, GLU_SMOOTH);

	//Making a display list
	// mysphereID = glGenLists(1);
	// glNewList(mysphereID, GL_COMPILE);

	glPushMatrix();								//update to not use a pushMatrix, zz debug
		glRotatef (180.0f, 0.0f, 0.0f, 1.0f);	//orient world texture for coord 0,0
		gluCylinder (gluObject, 1.0, 0.0, 2.0, 24, 1);	//draw cone
		glRotatef (180.0f, 1.0f, 0.0f, 0.0f);	//flips the disk over
		gluDisk (gluObject, 0.0, 1.0, 24, 1);	//draw bottom disk
	glPopMatrix();

	// glEndList();
	// gluDeleteQuadric(sphere);				``````````````//zz debug, should use this
}

//------------------------------------------------------------------------------
GLuint npCreatePrimitiveDL(void)
{
	GLuint displayList = 0, i = 0;


	// Create the id for the list
	i = displayList = glGenLists (kNPgeoCount);

	// start list
	glNewList (i++, GL_COMPILE);
		glutWireCube (1.4142f);
	glEndList();

	glNewList (i++, GL_COMPILE);
		glutSolidCube (1.4142f);
	glEndList();

	glNewList (i++, GL_COMPILE);
		DrawSphereWireDL();					//using GLU for proper tex mapping
	glEndList();

	glNewList (i++, GL_COMPILE);
		DrawSphereDL();						//using GLU for proper tex mapping
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoConeWire);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoCone);		//GLU not very good cone tex mapping
	glEndList();
	
	glNewList (i++, GL_COMPILE);
		npGLWireTorus (kNPtorusRadius * 0.1f, kNPtorusRadius, 7, 16);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGLSolidTorus (kNPtorusRadius * 0.1f, kNPtorusRadius, 7, 16);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoDodecahedronWire);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoDodecahedron);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoOctahedronWire);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoOctahedron);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoTetrahedronWire);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoTetrahedron);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoIcosahedronWire);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npGlutPrimitive (kNPgeoIcosahedron);
	glEndList();

	glNewList (i++, GL_COMPILE);
		DrawPinDL();
	glEndList();

	glNewList (i++, GL_COMPILE);
		DrawPinWireDL();
	glEndList();

	glNewList (i++, GL_COMPILE);
		DrawCylinderWireDL();
	glEndList();

	glNewList (i++, GL_COMPILE);
		DrawCylinderDL();
	glEndList();
	
	return (displayList);
}

