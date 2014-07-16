/* -----------------------------------------------------------------------------
*
*  ofxantz.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2013 by Shane Saxon - contact@saxondigital.net
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

#ifndef NP_OFXANTZ_H_
#define NP_OFXANTZ_H_

/* -----------------------------------------------------------------------------
*  Open Frameworks (OFX) support is NOT yet implemented!!!            //zz debug
*  http://www.openframeworks.cc
*  file structure has been updated in preparation for becoming and OFX-Addon
* --------------------------------------------------------------------------- */

#ifdef __cplusplus
	extern "C" {
#endif

void npInitOFX (void* data);		/* may update default init from pref file */

void npCloseOFX (void* data);

void npUpdateOFX (void* dataRef);

void npViewerOFX (void* dataRef);

#ifdef __cplusplus
	}
#endif
#endif

