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

#pragma once

#ifndef _CVIEW_H_
#define _CVIEW_H_

#include "curses.h"
#include "CUfo.h"
#include "CPlayer.h"
#include "CAliens.h"
#include "CShip.h"

class CView
{
private:
	static void finish(int sig);

	// colors
	enum {
		RED = 1,
		GREEN = 2,
		YELLOW = 3,
		BLUE = 4,
		CYAN = 5,
		MAGENTA = 6,
		WHITE = 7
	};

	CShip alienShips[9];

	WINDOW *wBattleField;
	WINDOW *wEmpty;
	WINDOW *wScores;

	WINDOW *wPlayer;
	WINDOW *wPlayerMissile;
	WINDOW *wAliens;
	WINDOW *wAliensMissile;
	WINDOW *wBunkers;
	WINDOW *wGameOver;
	WINDOW *wUfo;
	WINDOW *wStatus;
	WINDOW *wPause;
	WINDOW *wTitleScreen;

	CUfo *cUfo;
	CPlayer *cPlayer;
	CAliens *cAliens;

	void battleFieldInit();
	void doSleep(unsigned);
	void pauseInit();
	void statusInit();
	void titleScreenInit();

	void playerExplosionDisplay();

public:
	CView();
	CView(CUfo* , CPlayer*, CAliens*);

	void fieldClearElement(const int x, const int y);

	void aliensInit();
	void aliensClear(const int x, const int y, const int wid, const int hgt);
	void aliensMissilesClear();
	void aliensDisplay(const int x, const int y, const int wid, const int hgt);
	void aliensMissileInit();
	void aliensMissileDisplay();
	void aliensRefresh(const int level);

	void battleFieldClear();
	void bunkersInit();
	void bunkersClear();
	void bunkersDisplay(const char *pBunker);
	void gameOverInit();
	void gameOverDisplay();
	void graphicEngineInit();

	void playerInit();
	void playerClear();
	void playerDisplay();
	void playerExplode();
	void playerMissileInit();
	void playerMissileDisplay();

	void titleScreenClear();
	void titleScreenDisplay();

	void ufoInit();
	void ufoClear();
	void ufoDisplay();
	void ufoRefresh();

	void statusDisplay(const int, const int, const int);
	void refreshScreen();

	void pauseDisplay();
	void pauseClear();

	virtual ~CView();
};

#endif
