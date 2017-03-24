/* -----------------------------------------------------------------------------
*
*  npgeometry.c
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

#include "npgeometry.h"

#include "npgldraw.h"

#include "../npglut.h"

#include "../file/npmodels.h"	//zz models

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
static GLint cube_dl;
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

//	cube_dl = npCreateCubeDL();

	return;
}

//------------------------------------------------------------------------------
void npCloseGLPrimitive (void* dataRef)
{
	return;
}

#define kNPc 0.707107f
//------------------------------------------------------------------------------
void npDrawCubeDL (void)
{
    glBegin(GL_QUADS);

	// Top
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -kNPc, -kNPc, kNPc );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  kNPc, -kNPc, kNPc );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  kNPc,  kNPc, kNPc );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -kNPc,  kNPc, kNPc );

	// Bottom
	glNormal3f( 0.0f, 0.0f, -1.0f );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  kNPc, -kNPc, -kNPc );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -kNPc, -kNPc, -kNPc );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -kNPc,  kNPc, -kNPc );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  kNPc,  kNPc, -kNPc );

	// North face
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  kNPc,  kNPc, -kNPc );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -kNPc,  kNPc, -kNPc );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -kNPc,  kNPc,  kNPc );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  kNPc,  kNPc,  kNPc );

	// South
	glNormal3f( 0.0f, -1.0f, 0.0f );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -kNPc, -kNPc, -kNPc );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  kNPc, -kNPc, -kNPc );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  kNPc, -kNPc,  kNPc );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -kNPc, -kNPc,  kNPc );

	// East
	glNormal3f( 1.0f, 0.0f, 0.0f );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( kNPc, -kNPc, -kNPc );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( kNPc,  kNPc, -kNPc );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( kNPc,  kNPc,  kNPc );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( kNPc, -kNPc,  kNPc );

	// West
	glNormal3f( -1.0f, 0.0f, 0.0f );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -kNPc,  kNPc, -kNPc );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -kNPc, -kNPc, -kNPc );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -kNPc, -kNPc,  kNPc );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -kNPc,  kNPc,  kNPc );

    glEnd( );
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
		|| node->geometry == kNPgeoCylinder
		|| node->geometry == kNPgeoCube ) // || geometry == kNPgeoCylinderWire
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
	i = displayList = glGenLists (kNPgeoMax);//kNPgeoCount);

	if( displayList == 0 )
	{
		npPostMsg("err 4488 - glGenLists() failed", kNPmsgErr, NULL);
		return 0;
	}

	// start list
	glNewList (i++, GL_COMPILE);
		glutWireCube (1.4142f);
	glEndList();

	glNewList (i++, GL_COMPILE);
		npDrawCubeDL ();
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

//zz models begin
#define kNPgeoUnitCube 2.0f	//zz move to nptypes.h
#define kNPdrawModelsMode 2	// NP_POINTS = 0, NP_LINES = 1 and NP_POLYGONS = 2

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	struct aiString str;
	float shininess, strength;
	int two_sided;
	int wireframe;
	int test = 0;
	unsigned int max;

	test = aiGetMaterialTexture(mtl, aiTextureType_NORMALS, 0, &str, NULL, NULL, NULL, NULL, NULL, NULL);
//	printf("\ntest : %d", test);

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}



//------------------------------------------------------------------------------
void npDrawModelsDL( struct aiMesh* mesh, int faceMode );
/// Display List (DL) code is optimized for performance.
/// We don't want anything extra inside the DL.
void npDrawModelsDL( struct aiMesh* mesh, int faceMode )
{
	//zz move this to npGetBoundingBox()
	// printf( "child mesh: %d  polygons: %d\n", z, mesh->mNumFaces );

	// skip if no verti
	if( mesh->mNumVertices == 0 )
		return;

	/// @todo could not access assimp mesh->HasNormals...
	//apply_material(scene->mMaterials[mesh->mMaterialIndex]);
	if( mesh->mNormals != NULL )
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT, 0, mesh->mNormals );
	}
	if( mesh->mColors[0] != NULL )
	{
		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( 4, GL_FLOAT, 0, mesh->mColors[0] );
	}
	if( mesh->mTextureCoords[0] != NULL )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(3, GL_FLOAT, 0, mesh->mTextureCoords[0]);
	}

	// vertextPtr should be last for performance
	glEnableClientState( GL_VERTEX_ARRAY );
//	glVertexPointer( 3, GL_FLOAT, 92, mesh->mVertices );	// 12 bytes / vertex
//	glDrawArrays( faceMode, 0, mesh->mNumVertices / 8 );		//draw mesh
//	glVertexPointer( 3, GL_FLOAT, 576, mesh->mVertices );	 // 4 
//	glDrawArrays( faceMode, 0, mesh->mNumVertices / 48 );		//draw mesh
	glVertexPointer( 3, GL_FLOAT, 0, mesh->mVertices );	 // 4 
	glDrawArrays( faceMode, 0, mesh->mNumVertices );		//draw mesh

	// finished drawing this mesh so disable the states
	glDisableClientState( GL_VERTEX_ARRAY );

	if( mesh->mNormals != NULL )
		glDisableClientState( GL_NORMAL_ARRAY );
	if( mesh->mColors[0] != NULL )
		glDisableClientState( GL_COLOR_ARRAY );
	if( mesh->mTextureCoords[0] != NULL  )
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

/*
	for(z = 0; z < node->mNumMeshes; z++)
	{
		mesh = scene->mMeshes[node->mMeshes[z]];
		//apply_material(scene->mMaterials[mesh->mMaterialIndex]);
		for(x = 0; x < (int)mesh->mNumFaces; x++)
		{
			face = &mesh->mFaces[x];
			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
*/
		
void npModelsTreeRecursion(struct aiScene* scene, struct aiNode* node, int faceMode, void* dataRef);
/// @todo move to npgeometry.c
void npModelsTreeRecursion(struct aiScene* scene, struct aiNode* node, int faceMode, void* dataRef)
//-----------------------------------------------------------------------------
{
//	pNPassimp assimp = data->io.assimp;
//	struct aiMatrix4x4 m = node->mTransformation;
//	struct aiFace* face = NULL;
//	GLenum face_mode = 0;

	pData data = (pData) dataRef;

	struct aiMesh* mesh = NULL;
	unsigned int i = 0, x = 0, z = 0;
	
	/// Loop through all meshes belonging to this models node and draw them
	for( z = 0; z < node->mNumMeshes; z++ )
	{
		mesh = scene->mMeshes[node->mMeshes[z]];
		npDrawModelsDL( mesh, faceMode );
	}

	/// Recursively traverse models tree to draw all children
	for (i = 0; i < node->mNumChildren; i++)
		npModelsTreeRecursion( scene, node->mChildren[i], faceMode, dataRef );
}


void npBoxGen(struct aiScene* scene, struct aiNode* node, pNPbox bBox ,void* dataRef);
void npBoxGen(struct aiScene* scene, struct aiNode* node, pNPbox bBox ,void* dataRef)
{
	struct aiMesh* mesh = NULL;
	struct aiVector3D v;
	
	unsigned int m = 0;
	int vNum = 0;
	int vI;

	if(!node)
		return;

	if(scene->mRootNode == node && node->mMeshes)
	{
		bBox->xH = scene->mMeshes[node->mMeshes[m]]->mVertices[0].x;
		bBox->yH = scene->mMeshes[node->mMeshes[m]]->mVertices[0].y;
		bBox->zH = scene->mMeshes[node->mMeshes[m]]->mVertices[0].z;
		
		bBox->xL = scene->mMeshes[node->mMeshes[m]]->mVertices[0].x;
		bBox->yL = scene->mMeshes[node->mMeshes[m]]->mVertices[0].y;
		bBox->zL = scene->mMeshes[node->mMeshes[m]]->mVertices[0].z;
	}
	else if(scene->mRootNode == node && node && (node->mMeshes == NULL))
	{
		for(m = 0; m < node->mNumChildren; m++)
		{
			if(node->mChildren[m]->mNumMeshes > 0)
			{
				bBox->xH = scene->mMeshes[node->mChildren[m]->mMeshes[0]]->mVertices[0].x;
				bBox->yH = scene->mMeshes[node->mChildren[m]->mMeshes[0]]->mVertices[0].y;
				bBox->zH = scene->mMeshes[node->mChildren[m]->mMeshes[0]]->mVertices[0].z;
				
				bBox->xL = scene->mMeshes[node->mChildren[m]->mMeshes[0]]->mVertices[0].x;
				bBox->yL = scene->mMeshes[node->mChildren[m]->mMeshes[0]]->mVertices[0].y;
				bBox->zL = scene->mMeshes[node->mChildren[m]->mMeshes[0]]->mVertices[0].z;
				break;
			}
		}

	}

	for(m = 0; m < node->mNumMeshes; m++)
	{
		mesh = scene->mMeshes[node->mMeshes[m]];
		vNum = mesh->mNumVertices;
		for(vI = 0; vI < vNum; vI++)
		{
			v = mesh->mVertices[vI];
			if(v.x > bBox->xH)
				bBox->xH = v.x;

			if(v.y > bBox->yH)
				bBox->yH = v.y;

			if(v.z > bBox->zH)
				bBox->zH = v.z;

			if(v.x < bBox->xL)
				bBox->xL = v.x;

			if(v.y < bBox->yL)
				bBox->yL = v.y;

			if(v.z < bBox->zL)
				bBox->zL = v.z;
			//vec->
		}
	//	node->mT
	}

	if(node->mChildren)
	{
		for(m = 0; m < node->mNumChildren; m++)
			npBoxGen(scene, node->mChildren[m], bBox, dataRef);
	}
}

/// @todo change to npCreateModelsDL( pNPmodels scene, pNPgeolist geolist, void* dataRef)
void npModelStoreDL(struct aiScene* scene, pNPgeolist geo, void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgl gl = &data->io.gl;
	pNPassimp assimp = (pNPassimp)data->io.assimp;
	NPbox bBox;

	float dX = 0.0f, dY = 0.0f, dZ = 0.0f;	//bounding box dimensions
	float cX = 0.0f, cY = 0.0f, cZ = 0.0f;	//center offset
	float rX = 0.0f, rY = 0.0f, rZ = 0.0f;	//rotate
	float sX = 0.0f, sY = 0.0f, sZ = 0.0f;	//scale

	float normalize = 1.0f;	//default scale value is 1.0f for no change
	int faceMode = 0; // GL_POINTS, lines, polygons

	printf( "Preparing 3D Models...\n" );

	/// If model has 0 scale then normalize the scale and set the model center.
	/// Perhaps set to zero in the np_models table or file loaded directly.
	if( geo->scale.x == 0.0f || geo->scale.y ==  0.0f || geo->scale.z ==  0.0f )
	{
		//printf( "find model scene bounding box\n" );
		npBoxGen( scene, scene->mRootNode, &bBox, dataRef );

		printf( "bounding box min XYZ: %f %f %f \n  max XYZ: %f %f %f\n",
				bBox.xL, bBox.yL, bBox.zL, bBox.xH, bBox.yH, bBox.zH );

		/// Calculate the bounding box dimensions dX, dY, dZ
		dX = (float)fabs( bBox.xH - bBox.xL );
		dY = (float)fabs( bBox.yH - bBox.yL );
		dZ = (float)fabs( bBox.zH - bBox.zL );

		/// Find the geometric center point of entire models scene
		cX = (bBox.xH + bBox.xL) / 2.0f;
		cY = (bBox.yH + bBox.yL) / 2.0f;
		cZ = (bBox.zH + bBox.zL) / 2.0f;

		printf( "model center %f %f %f \n", cX, cY, cZ ); 

		/// Calculate the scale factor using the largest axis length
		if( dX > 0.0f || dY > 0.0f || dZ > 0.0f )
		{
			if( dX > dY && dX > dZ )
				normalize =  kNPgeoUnitCube / dX;
			if( dY > dZ )
				normalize = kNPgeoUnitCube / dY;
			else
				normalize = kNPgeoUnitCube / dZ;
				
			printf( "normalized model scale: %f\n", normalize );
		}
		else
			normalize = 1.0f;	// model is a singularity, don't scale

		/// Normalize the model scene with uniform scale
		sX = sY = sZ = normalize;
	}
	else
	{
		printf( "Center, Rotate and Scale using np_models table\n" );

		cX = geo->center.x;
		cY = geo->center.y;
		cZ = geo->center.z;
		sX = geo->scale.x;
		sY = geo->scale.y;
		sZ = geo->scale.z;
	}
	
	// we don't currently have a procedure to auto-orient the object
	// so we just use whatever is in from the CSV table
	rX = geo->rotate.x;
	rY = geo->rotate.y;
	rZ = geo->rotate.z;

	/// @todo add support for models face mode, ie: QUAD_STRIP POLYGON...
	// set the mode prior to entering the recursion cycle
	// GL_POINTS GL_LINES LINE_STRIP QUAD QUAD_STRIP POLYGON
	if( kNPdrawModelsMode == 0 )	//NP_POLYGONS // mesh->mFaces != NULL && mesh->mNumFaces > 0 )
		faceMode = GL_POINTS;			//draw points
	else if( kNPdrawModelsMode == 1)// shader == kNPshaderLines )
		faceMode = GL_LINES;			//draw lines
	else
		faceMode = GL_TRIANGLES;		//draw faces

	// Start the display list, keep everything fast from here
	glNewList( gl->dl + geo->geometryId, GL_COMPILE );
		glScalef( sX, sY, sZ );					///< Set the models scale
		glTranslatef( -cX, -cY, -cZ );			///< Inverse center model
	
		// orient models using KML coords system, note the Y-X-Z process order
		if( rY != 0.0f )
			glRotatef( rY, 0.0f, 0.0f, -1.0f );		//heading
		if( rX != 0.0f )
			glRotatef( rX, -1.0f, 0.0f, 0.0f );		//tilt
		if( rZ != 0.0f )
			glRotatef( rZ, 0.0f, 0.0f, -1.0f );		//roll
		
		// detect first mesh face type and use it to set GL_POLY... face mode
		/// @todo support multiple face modes within a single mesh
		/// @todo import texture materials

	/// Start recursion loop, draws all objects in this models scene graph
	npModelsTreeRecursion( scene, scene->mRootNode, faceMode, data );

	glEndList();
}

void npInitGeoListPrimitives(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgl gl = &data->io.gl;
//	pNPgeoList geoList = &data->io.gl.geoList;
	pNPgeolist geolist = data->io.gl.geolist;
	GLuint displayList = 0, i = 0;

	gl->numPrimitives = 0;
	//geoList->numPrimitives = 0;

	// Create the id for the list
	gl->dl = 0;
	i = gl->dl = glGenLists(4096); // kNPgeoCount
//	printf("geoList->DL : %u", geoList->DL);

	if( gl->dl == 0 )
	{
		npPostMsg("err 4488 - glGenLists() failed", kNPmsgErr, NULL);
		return;
	}

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

	gl->numPrimitives = i - gl->dl;
//	printf("\ndisplayList : %u", displayList);
//	printf("geoList->numPrimitives : %d", geoList->numPrimitives);
//	printf("\ngl->numPrimitives : %d", gl->numPrimitives);

	return;
}

void npInitGeoList(void* dataRef)
{
	pData data = (pData) dataRef;
	pNPgl gl = &data->io.gl;
//	pNPgeoList geoList = &data->io.gl.geoList;
	pNPgeolist geolist = data->io.gl.geolist;
	pNPgeolist p_geo = data->io.gl.geolist;
	int i = 0;

	gl->modelId = 0;
//	gl->geoLock = true;
	npGeolistLock(dataRef);
	npGeolistSetLen(0, dataRef);
	npGeolistSetX(0, dataRef);
	gl->numModels = 0;
	gl->numPrimitives = 0;

	npInitGeoListPrimitives(dataRef);

	gl->texmapCount = 0;
	for(i = 0; i < 100; i++)
	{
		gl->extMap[i] = 0;
		gl->extMapMe[i] = NULL;
	}

//	geoList->DL = glGenLists(kNPgeoListMax);
	for(i = 0; i < kNPgeoListMax; i++)
	{
		p_geo = &data->io.gl.geolist[i];
		p_geo->extTexId = 0;
		p_geo->loaded = 0;
		p_geo->geometryId = 0;
		p_geo->modelFile[0] = '\0';
		p_geo->modelId = 0;
		p_geo->modelPath[0] = '\0';
		p_geo->modelTextureFile[0] = '\0';
		p_geo->modelTexturePath[0] = '\0';
		p_geo->name[0] = '\0';
		p_geo->textureId = 0;
		/*
		geolist->modelFile[0] = '\0';
		geolist->name[i] = '\0';
		geolist->textureId = 0;
		*/
	}

	return;
}

//zz models end

