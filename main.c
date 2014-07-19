/* -----------------------------------------------------------------------------
*
*  main.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2012 by Shane Saxon - contact@saxondigital.net
*
*  Additional code contributors are listed chronologically: Lucas Erickson, 
*  Mark Buchanan and Johannes Johannsen
*
*  Special thanks to Dave Warner, project synergist and conceptual architect.
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

#include "npdata.h"							//defines which OS to compile for
#include "npio.h"
#include "npctrl.h"
#include <signal.h> // debug, voidptr LINUX ONLY PROTOTYPE
#include "os/linux/nplinux.h"  // debug, voidptr LINUX ONLY PROTOTYPE

// MVC architecture variant where View becomes a subset of the IO group
// Model named Data
// View named IO, includes keyboard, mice, haptic devices, audio, video...
// Control named Ctrl, a pseudo state-machine based on physics, excludes IO
//------------------------------------------------------------------------------
int main (int argc, char **argv)
{
	int ver = 0;
	int err = 0;
	void* data = NULL;

	signal(SIGHUP, signal_callback_handler);	
	signal(SIGINT, signal_callback_handler);
	signal(SIGQUIT, signal_callback_handler);	
	signal(SIGILL, signal_callback_handler);	
	signal(SIGTRAP, signal_callback_handler);	
	signal(SIGABRT, signal_callback_handler);
	signal(SIGKILL, signal_callback_handler);
//	signal(SIGBUS, signal_callback_handler);
	signal(SIGFPE, signal_callback_handler);	
	signal(SIGUSR1, signal_callback_handler);
	signal(SIGSEGV, signal_callback_handler);	
	signal(SIGUSR2, signal_callback_handler);	
	signal(SIGPIPE, signal_callback_handler);	
	signal(SIGALRM, signal_callback_handler);	
	signal(SIGTERM, signal_callback_handler);
	signal(SIGCHLD, signal_callback_handler);	
	signal(SIGCONT, signal_callback_handler);	
	signal(SIGSTOP, signal_callback_handler);	
	signal(SIGTSTP, signal_callback_handler);	
	signal(SIGTTIN, signal_callback_handler);
	signal(SIGTTOU, signal_callback_handler);	
//	signal(SIGXCPU, signal_callback_handler);	
//	signal(SIGXFSZ, signal_callback_handler);		

        data = npInitData (argc, argv);			// Model
	npInitIO (data);						// View
	npInitCtrl (data);						// Control
	err = npAppLoop (data);

	npCloseCtrl (data);
	npCloseIO (data);
	npCloseData (data);


	return err;
}

