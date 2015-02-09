/* -----------------------------------------------------------------------------
*
*  npvideo.h
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

#ifndef NPVIDEO_H_
#define NPVIDEO_H_

/*! @todo add Video IO support for live capture, record and playback. <br>
*		Calls upon OS specific and hardware specific libraries. <br>
*		Provides video pipeline between the GPU and CPU.					  */


void npInitVideo (void* dataRef);
void npCloseVideo (void* dataRef);
void npUpdateVideo (void* dataRef);


#endif

