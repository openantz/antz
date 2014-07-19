/* -----------------------------------------------------------------------------
*
*  npglut.c
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

#include "npglut.h"

#include "../npio.h"
#include "../npctrl.h"				//remove once npPostMsg is global, debug zz

#include "npgl.h"
#include "npkey.h"
#include "npmouse.h"

#include "../os/npos.h"

#include "../data/npmapfile.h"		//debug, zz


#define kWindowWidth 800
#define kWindowHeight 450
#define kWindowPositionX 40
#define kWindowPositionY 40


void npGlutInitGL();
void npGlutDrawGLScene(void);
void npGlutDrawGLSceneIdle(void);
void npGlutResizeGLScene(int Width, int Height);

void npGlutKeyDown (unsigned char key, int x, int y);
void npGlutKeyUp (unsigned char key, int x, int y);
void npGlutKeyDownSpecial (int key, int x, int y);
void npGlutKeyUpSpecial (int key, int x, int y);

//update to allow entering and leaving game mode without re-loading textures
// wglShareLists() function in MSW and glXCreateContext() under X Windows. zz
//------------------------------------------------------------------------------
void npInitGlut (int argc, char **argv, void* dataRef)
{
	GLboolean stereoSupport = false;
	int depth = 0;
	int result = 0;
	int gMainWindow = 0;

	pData data = (pData) dataRef;

	data->io.gl.fullScreen = true;			//S3D
//	data->io.gl.stereo = false;				//S3D

	glutInit (&argc, argv);

	glGetBooleanv(GL_STEREO, &stereoSupport);
	data->io.gl.stereo = stereoSupport;

	if (data->io.gl.stereo)
		printf ("stereo 3D support: YES\n");
	else
		printf ("stereo 3D support: NO\n");

	//stereo 3D pixel buffer
	if (data->io.gl.stereo)
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STEREO);
	else
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowPosition (kWindowPositionX, kWindowPositionY);
	glutInitWindowSize (kWindowWidth, kWindowHeight);
	data->io.gl.windowID = glutCreateWindow ("ANTz");
	
	glutSetWindow (data->io.gl.windowID);	//S3D
	glutHideWindow();						//S3D

/*	//stereo 3D and GameMode
	//if (data->io.gl.gameMode)
	printf("Attempting Game Mode, please ignore GLUT warnings\n");
	glutGameModeString("1920x1080:32@121");
	if ( glutEnterGameMode() == 0 )
	{
		glutGameModeString("1920x1080:32@120");
		if ( glutEnterGameMode() == 0 )
		{
			glutGameModeString("1920x1080:32@119");
			if ( glutEnterGameMode() == 0 )
			{
				glutGameModeString("1920x1080:32@60"); //does not specify refresh
				result = ( glutEnterGameMode() );
	}}}	//add more res... 1680x1050 1366x768 1280x720 1280x1024, 1024x768, also 100Hz and 110Hz
*/
//	wglSwapIntervalEXT(1);		//to remove frame tearing with VSYNC debug, zz
//	nposSwapInterval(1);		//move OS methods to npos.h, debug zz
//	glfwSwapInterval(1);		//may use the glfw libarry, not implememted, zz

	//register keyboard events with GLUT
	glutKeyboardFunc (npGlutKeyDown);
	glutKeyboardUpFunc (npGlutKeyUp);
	glutSpecialFunc (npGlutKeyDownSpecial);
	glutSpecialUpFunc (npGlutKeyUpSpecial);
	
	//register mouse events with GLUT
	glutMouseFunc (npMouseEvent);
	glutMotionFunc (npMouseMotion);

	glutDisplayFunc (npGlutDrawGLScene);
	glutIdleFunc (npGlutDrawGLSceneIdle);

	glutReshapeFunc (npGLResizeScene);
	
	//---
	npInitGL (dataRef);

	if (result == 0)
	{
		npPostMsg ("FullScreen Window", kNPmsgCtrl, dataRef);
		glutShowWindow ();
		glutFullScreen ();
//		data->io.gl.stereo = false;								//stereo 3D, debug zz
	}
	else
	{	//GameMode may be different then what we requested, so get the modes
		glutSetWindowTitle("ANTz - GameMode");

		data->io.gl.width = glutGameModeGet( GLUT_GAME_MODE_WIDTH );
		data->io.gl.height = glutGameModeGet( GLUT_GAME_MODE_HEIGHT );
		data->io.gl.pixelDepth = glutGameModeGet( GLUT_GAME_MODE_PIXEL_DEPTH );
		data->io.gl.refreshRate = (float)glutGameModeGet( GLUT_GAME_MODE_REFRESH_RATE );
		printf("FullScreen Game Mode: %dx%d:%d@%d\n",data->io.gl.width,data->io.gl.height,
						data->io.gl.pixelDepth, (int)data->io.gl.refreshRate);
		//stereo 3D, turn off stereo if reresh rate is too low, update this, zz
		if (data->io.gl.refreshRate >= 99)
			printf("Stereo 3D enabled\n");
		else
		{
			if (data->io.gl.stereo)
				printf("Stereo 3D disabled, low refresh rate: %dHz\n", 
					data->io.gl.refreshRate);
			data->io.gl.stereo = false;
		}
	}

	npPostMsg ("ANTz hosted at: www.openANTz.com", kNPmsgCtrl, dataRef);
	npPostMsg ("System Ready...", kNPmsgCtrl, dataRef);
	//glGetIntegerv (GL_TEXTURE_STACK_DEPTH, &depth); // GL_MODELVIEW_STACK_DEPTH
}


//------------------------------------------------------------------------------
void npCloseGlut (void* dataRef)
{
	npCloseGL (dataRef);

	return;
}


//------------------------------------------------------------------------------
void npAppLoopGlut (void* dataRef)
{
	npInitGlut (0, 0, dataRef);
	glutMainLoop();
}


//------------------------------------------------------------------------------
void npGlutDrawGLScene(void) 
{
	double deltaTime = 0.0;
	double currentTime = 0.0;
	double targePeriod = 0.0;

	pData data = (pData) npGetDataRef();
		
	//update data, positions, physics, user input
	npUpdateCtrl (data);

	//init time on first run
	npGLDrawScene (data);

	glFlush();									//vsync, not yet functional, debug zz
	glFinish();

	//calculate the delta time using previously stored time
	currentTime = nposGetTime();
	deltaTime = currentTime - data->io.time;	//add clock roll-over handling, debug zz

	//if extra time left over then sleep for remainder of cycle
	if (deltaTime < data->io.cyclePeriod)
		nposSleep (data->io.cyclePeriod - deltaTime);

	glutPostRedisplay();		//supposed to help with window freezing,	debug zz
								//issue with mouse hitting edge of screen
	glutSwapBuffers();

//	glFinish();					//vsync

	//update the locally stored time, used when calculating the delta time
	nposUpdateTime (data);
}

//------------------------------------------------------------------------------

void npGlutDrawGLSceneIdle(void)
{
//	npGlutDrawGLScene();

	return;
}

//fullscreen GameMode creates a new GL context /which requires re-registering
//for event callbacks and re-loading texture maps
//------------------------------------------------------------------------------
void npglFullscreen (void* dataRef)
{
	int result = 0;		//fullscreen window used only if gamemode fails

	pData data = (pData) dataRef;

	if (data->io.gl.fullScreen)
	{
		if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)		//stereo 3D
		{
			glutLeaveGameMode();
			glutSetWindow (data->io.gl.windowID);
            glutShowWindow ();

			glutKeyboardFunc (npGlutKeyDown);
			glutKeyboardUpFunc (npGlutKeyUp);
			glutSpecialFunc (npGlutKeyDownSpecial);
			glutSpecialUpFunc (npGlutKeyUpSpecial);


			//register mouse events with GLUT
			glutMouseFunc (npMouseEvent);
			glutMotionFunc (npMouseMotion);

			glutDisplayFunc (npGlutDrawGLScene);
			glutIdleFunc (npGlutDrawGLSceneIdle);

			glutReshapeFunc (npGLResizeScene);
			//---
			npInitGL (data);
		}

		glutPositionWindow (kWindowPositionX, kWindowPositionY);
		glutReshapeWindow (kWindowWidth, kWindowHeight);

		data->io.gl.fullScreen = false;
		printf("Exit FullScreen\n");
	}
	else
	{
/*		glutSetWindow (	data->io.gl.windowID);
		glutHideWindow ();

		//Game Mode with stereo 3D
		if (data->io.gl.stereo)
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STEREO); // stereo display mode for glut
		else
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

		//stereo 3D and GameMode
		printf("Attempting Game Mode, please ignore GLUT warnings\n");
		glutGameModeString("1920x1080:32@121");
		if ( glutEnterGameMode() == 0 )
		{
			glutGameModeString("1920x1080:32@120");
			if ( glutEnterGameMode() == 0 )
			{
				glutGameModeString("1920x1080:32@119");
				if ( glutEnterGameMode() == 0 )
				{
					glutGameModeString("1920x1080:32@60"); //does not specify refresh
					result = ( glutEnterGameMode() );
		}}}

		//register keyboard events with the new GameMode GL context
		glutKeyboardFunc (npGlutKeyDown);
		glutKeyboardUpFunc (npGlutKeyUp);
		glutSpecialFunc (npGlutKeyDownSpecial);
		glutSpecialUpFunc (npGlutKeyUpSpecial);
		
		//register mouse events with GLUT
		glutMouseFunc (npMouseEvent);
		glutMotionFunc (npMouseMotion);

		glutDisplayFunc (npGlutDrawGLScene);
		glutIdleFunc (npGlutDrawGLSceneIdle);
		glutReshapeFunc (npGLResizeScene);
		
		//---
		npInitGL (data);
*/
		if (result == 0)	//fullscreen window used only if gamemode fails
		{
			printf("FullScreen Window\n");
			glutShowWindow ();
			glutFullScreen ();
			//data->io.gl.stereo = false;							//stereo 3D, debug zz
		}
		else
		{	//GameMode may be different then what we requested, so get the modes
			glutSetWindowTitle("ANTz - GameMode");

			data->io.gl.width = glutGameModeGet( GLUT_GAME_MODE_WIDTH );
			data->io.gl.height = glutGameModeGet( GLUT_GAME_MODE_HEIGHT );
			data->io.gl.pixelDepth = glutGameModeGet( GLUT_GAME_MODE_PIXEL_DEPTH );
			data->io.gl.refreshRate = (float)glutGameModeGet( GLUT_GAME_MODE_REFRESH_RATE );
			printf("FullScreen Game Mode: %dx%d:%d@%d\n",data->io.gl.width,data->io.gl.height,
							data->io.gl.pixelDepth, (int)data->io.gl.refreshRate);
			//stereo 3D, turn off stereo if reresh rate is too low, update this, zz
			if (data->io.gl.refreshRate >= 99)
			{
				printf("Stereo 3D enabled\n");
				data->io.gl.stereo = true;
			}
			else
			{
				printf("Stereo 3D disabled, low refresh rate\n");
				data->io.gl.stereo = false;
			}
		}

		data->io.gl.fullScreen = true;
	}
}


//------------------------------------------------------------------------------
void npGlutKeyDown (unsigned char key, int x, int y) 
{
	npKeyGlut (key, x, y, kGlutKeyDown, glutGetModifiers());
}

//------------------------------------------------------------------------------
void npGlutKeyUp (unsigned char key, int x, int y) 
{
	npKeyGlut (key, x, y, kGlutKeyUp, glutGetModifiers());
}

//------------------------------------------------------------------------------
void npGlutKeyDownSpecial (int key, int x, int y) 
{
	npKeyGlut (key, x, y, kGlutKeyDownSpecial, glutGetModifiers());
}

//------------------------------------------------------------------------------
void npGlutKeyUpSpecial (int key, int x, int y) 
{
	npKeyGlut (key, x, y, kGlutKeyUpSpecial, glutGetModifiers() );
}


//------------------------------------------------------------------------------
void npGLSolidSphere (GLdouble radius, GLint slices, GLint stacks)
{
	glutSolidSphere (radius, slices, stacks);
}

//------------------------------------------------------------------------------
void npGLWireSphere (GLdouble radius, GLint slices, GLint stacks)
{
	glutWireSphere (radius, slices, stacks);
}

//------------------------------------------------------------------------------
void npGLSolidCone (GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	glutSolidCone (base, height, slices, stacks);
}

//------------------------------------------------------------------------------
void npGLWireCone (GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	glutWireCone (base, height, slices, stacks);
}

//------------------------------------------------------------------------------
void npGLSolidTorus (GLdouble innerRadius, GLdouble outerRadius, 
					GLint slices, GLint stacks)
{
	glutSolidTorus (innerRadius, outerRadius, slices, stacks);
}

//------------------------------------------------------------------------------
void npGLWireTorus (GLdouble innerRadius, GLdouble outerRadius, 
					GLint slices, GLint stacks)
{
	glutWireTorus (innerRadius, outerRadius, slices, stacks);
}

//------------------------------------------------------------------------------
void npGlutPrimitive (int primitive)
{
	switch (primitive)
	{
	case kNPglutWireCube :
		glScalef (0.6f, 0.6f, 0.6f); 
		glutWireCube(2.0f);
		glScalef (1.666667f, 1.666667f, 1.666667f);
		break;
	case kNPglutSolidCube :
		glScalef (0.6f, 0.6f, 0.6f);
		glutSolidCube(2.0f);
		glScalef (1.666667f, 1.666667f, 1.666667f);
		break;
	case kNPglutWireSphere : glutWireSphere( 1.0f, 24, 12); break;//15, 15 ); break;
	case kNPglutSolidSphere : glutSolidSphere( 1.0f, 24, 12 ); break;

	case kNPglutWireCone : glutWireCone( 1.0f, 2.0f, 24, 1 ); break;
	case kNPglutSolidCone : glutSolidCone( 1.0f, 2.0f, 24, 1 ); break;

	case kNPglutWireTorus : glutWireTorus(0.15f, 1.5f, 7, 16); break;
	case kNPglutSolidTorus : glutSolidTorus(0.15f, 1.5f, 7, 16); break;

	case kNPglutWireDodecahedron :
		glScalef (0.6f, 0.6f, 0.6f);
		glutWireDodecahedron();
		glScalef (1.666667f, 1.666667f, 1.666667f);
		break;
	case kNPglutSolidDodecahedron :
		glScalef (0.6f, 0.6f, 0.6f);
		glutSolidDodecahedron();
		glScalef (1.666667f, 1.666667f, 1.666667f);
		break;
	case kNPglutWireOctahedron : glutWireOctahedron(); break;
	case kNPglutSolidOctahedron : glutSolidOctahedron(); break;
	case kNPglutWireTetrahedron : glutWireTetrahedron(); break;
	case kNPglutSolidTetrahedron : glutSolidTetrahedron(); break;
	case kNPglutWireIcosahedron : glutWireIcosahedron(); break;
	case kNPglutSolidIcosahedron : glutSolidIcosahedron(); break;

	case kNPglutWireTeapot : glutWireTeapot( 2.0f ); break;
	case kNPglutSolidTeapot : glutSolidTeapot( 2.0f ); break;

	default : glutWireTetrahedron(); break;
	}
}

