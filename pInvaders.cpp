/**
 * pInvaders - a space invaders clone for pdCurses
 * Copyright (C) 2002-2003 Dettus
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * homepage: http://ninvaders.sourceforge.net
 * mailto: ninvaders-devel@lists.sourceforge.net
 *
 */

#include <stdio.h>
#include <curses.h>
#include <windows.h>

#include "CGame.h"

int main(int argc, char **argv)
{
	CUfo *cUfo = new CUfo();
	CPlayer *cPlayer = new CPlayer();
	CBunker *cBunker = new CBunker();
	CAliens *cAliens = new CAliens();
	CView *cView = new CView(cUfo, cPlayer, cAliens);

	CGame cGame(cAliens, cBunker, cPlayer, cView, cUfo);

	cGame.evaluateCommandLine(argc, argv);	// evaluate command line parameters
	cView->graphicEngineInit();			// initialize graphic engine

	// set up timer/ game handling
	cGame.setUpTimer(&cGame);
	MSG msg;

	// read keyboard input
	do {
		GetMessage(&msg, NULL, 0, 0);
		DispatchMessage(&msg);
		cGame.readInput();
	} while (0 == 0);

	getchar();

	delete cUfo;
	delete cPlayer;
	delete cAliens;
	delete cBunker;
	delete cView;

	return 0;
}
