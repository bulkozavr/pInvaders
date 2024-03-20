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

#ifndef _CGAME_H_
#define _CGAME_H_


#include "CAliens.h"
#include "CBunker.h"
#include "CGlobals.h"
#include "CPlayer.h"
#include "CUfo.h"
#include "CView.h"
#include <vector>

class CGame
{
private:
	static VOID CALLBACK handleTimer();

	enum {
		GAME_LOOP = 1,
		GAME_NEXTLEVEL = 2,
		GAME_PAUSED = 3,
		GAME_OVER = 4,
		GAME_EXIT = 5,
		GAME_HIGHSCORE = 6
	};

	enum {
		UFO_ALIEN_TYPE   = 0,
		RED_ALIEN_TYPE   = 1,
		GREEN_ALIEN_TYPE = 2,
		BLUE_ALIEN_TYPE  = 3 
	};

	int level;
	int lives;
	long score;
	int skill_level;
	int status; // status handled in timer
	int weite;

	CAliens *cAliens;
	CBunker *cBunker;
	CGlobals cGlobals;
	CPlayer *cPlayer;
	CUfo *cUfo;
	CView *cView;
	
	void drawscore();
	void finish(const int sig);
	void initLevel();
	const int playerMoveMissile();
	void resetPlayerParams();
	void speedOfAliens();

public:
	CGame();
	CGame(CAliens*, CBunker *, CPlayer*, CView*, CUfo*);
	static std::vector<CGame*> m_CGame;

	void doScoring(const int alienType);
	void evaluateCommandLine(int argc, char **argv);
	void readInput();
	void setUpTimer(CGame*);

	virtual ~CGame();
};

#endif
