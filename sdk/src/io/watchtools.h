/* -----------------------------------------------------------------------------
*
*  watchtools.h
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

// this file was added by Mark Buchanan

#include <stdio.h>

#define WATCH_ON
//#define WATCH_ONELINE




//MACROS

//Debug
#ifdef WATCH_ONELINE
	#define WATCH_SEP "\t\t"
#else
	#define WATCH_SEP "\n"
#endif

#ifdef WATCH_ON
	#define WATCH_BOOL(  b )	printf( "%s: %s%s", #b, b ? "YES" : "NO", WATCH_SEP );
	#define WATCH_INT(   i )	printf( "%s: %d%s", #i, (int)(i), WATCH_SEP );
	#define WATCH_UINT(  ui )	printf( "%s: %u%s",#ui, (unsigned int)(ui), WATCH_SEP );
	#define WATCH_FLOAT( f )	printf( "%s: %f%s", #f, f, WATCH_SEP );
	#define WATCH_STR(   s )	printf( "%s: %s%s", #s, s, WATCH_SEP );
	#define WATCH_PTR(   p )	printf( "%s: %p%s", #p, p, WATCH_SEP );
	#define WATCH_VEC(   v )	printf( "%s: % f % f % f%s", #v, v.x, v.y, v.z, WATCH_SEP );
	#define WATCH_VEC2(  v )	printf( "%s:\t[%.4f , %.4f]%s", #v, v.x, v.y,	WATCH_SEP );
	#define WATCH_FMT( fmt, arg )	{ printf( "%s: ", #arg ); printf( fmt, arg ); printf( WATCH_SEP ); }
	#define WATCH( arg )		{ printf( "%s", arg ); printf( WATCH_SEP ); }
	#define WATCH_FUNC			printf( "%s%s", __FUNCTION__, WATCH_SEP );
	#define WATCH_FUNCCOUNT		{	static int count = 0;	printf( "%s: %d%s", __FUNCTION__, count, WATCH_SEP );	count++;	}
	#define WATCH_END			printf( "\n" );
#else
	#define WATCH_BOOL(  b )	
	#define WATCH_INT(   i )	
	#define WATCH_UINT(  ui )
	#define WATCH_FLOAT( f )	
	#define WATCH_STR(   s )	
	#define WATCH_VEC(   v )	
	#define WATCH_FMT( fmt, arg )	
	#define WATCH_VEC2(  v )
	#define WATCH( arg )	
	#define WATCH_FUNC
	#define WATCH_FUNCCOUNT
	#define WATCH_END
#endif









