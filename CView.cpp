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

#include "CView.h"
#include "Dimension.h"
#include <stdlib.h>	
#include <signal.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>

#define usleep(x) Sleep(x/1000)


 /**
  * sleep for specified time
  */

void CView::doSleep(unsigned microseconds)
{
	Sleep(microseconds);
}

#else
#include <unistd.h>

void CView::doSleep(unsigned microseconds)
{
	usleep(microseconds * 1000); // takes microseconds
}

#endif


CView::CView()
{
}

CView::CView(CUfo* _cUfo, CPlayer *_cPlayer, CAliens *_cAliens) :
	cUfo(_cUfo),
	cPlayer(_cPlayer),
	cAliens(_cAliens)
{
	/**
	 * initialize aliens ships
	 */
	alienShips[0] = CShip(",^,", ".-.", 200);
	alienShips[1] = CShip("_O-", "-O_", 150);
	alienShips[2] = CShip("-o-", "/o\\", 100);
	alienShips[3] = CShip("o=o", "o-o", 200);
	alienShips[4] = CShip("<O>", "<o>", 150);
	alienShips[5] = CShip("_x_", "-x-", 100);
	alienShips[6] = CShip("*^*", "o^o", 200);
	alienShips[7] = CShip("\\_/", "/~\\", 150);
	alienShips[8] = CShip("o o", "oo ", 100);
}

void CView::fieldClearElement(const int x, const int y)
{
	copywin(wEmpty, wBattleField, 0, 0, y, x, y, x, 0);
}

/**
 * initialize aliens sprites
 */
void CView::aliensInit()
{
	wAliens = newpad(ALIENS_MAX_NUMBER_Y * 2, ALIENS_MAX_NUMBER_X * 3);
	wclear(wAliens);
}

/**
 * clear aliens sprite
 */
void CView::aliensClear(const int x, const int y, const int wid, const int hgt)
{
	copywin(wEmpty, wBattleField, 0, 0, y, x, y + hgt, x + wid + 2, 0);
}

/**
 * initialize missile sprites
 */
void CView::aliensMissileInit()
{
	wAliensMissile = newpad(1, 1);		// new pad
	wclear(wAliensMissile);			// clear pad
	wattrset(wAliensMissile, COLOR_PAIR(CYAN));	// set color
	waddch(wAliensMissile, cAliens->GetAlienMissileSprite(0));			// set sprite
}

/**
 * clear aliens missiles sprite
 */
void CView::aliensMissilesClear()
{
	const CMissile *aliensMissiles = cAliens->GetAliensMissiles();
	for (int i = 0; i < ALIENS_MAX_MISSILES; i++) {
		if ((aliensMissiles + i)->GetMissileFired() == 1) {
			// clear old position
			fieldClearElement((aliensMissiles + i)->GetPosX(), (aliensMissiles + i)->GetPosY());
		}
	}
}

/**
 * display alien missiles
 */
void CView::aliensMissileDisplay() {
	CMissile *aliensMissiles = cAliens->GetAliensMissiles();
	for (int i = 0; i < ALIENS_MAX_MISSILES; i++) {
		// if the current missile is flying
		if ((aliensMissiles + i)->GetMissileFired() == 1) {
			// clear old position
			fieldClearElement((aliensMissiles + i)->GetPosX(), (aliensMissiles + i)->GetPosY() - 1);
			// display Missile at new position
			copywin(wAliensMissile, wBattleField, 0, 0, 
				(aliensMissiles + i)->GetPosY(), (aliensMissiles + i)->GetPosX(),
				(aliensMissiles + i)->GetPosY(), (aliensMissiles + i)->GetPosX(), 0);
		}
		else if ((aliensMissiles + i)->GetMissileFired() == 0) {
			(aliensMissiles + i)->SetMissileFired(-1);
			fieldClearElement((aliensMissiles + i)->GetPosX(), (aliensMissiles + i)->GetPosY());
		}
	}
}

/**
 * display aliens sprite
 */
void CView::aliensDisplay(const int x, const int y, const int wid, const int hgt)
{
	copywin(wAliens, wBattleField, 0, 0, y, x, y + hgt, x + wid + 2, 0);
}

/**
 * refresh aliens sprite
 */
void CView::aliensRefresh(const int level)
{
	static int frame = 0;
	const int colors[] = { RED, GREEN, BLUE, RED, YELLOW, WHITE, WHITE, YELLOW, RED };
	const int FRAMES_PER_SEC = 18;

	int *pAliens = cAliens->GetAlienBlock();

	wclear(wAliens);						// clear pad
	wattrset(wAliens, COLOR_PAIR(RED));		// set color

	frame++;						// next frame
	frame %= FRAMES_PER_SEC;

	int k, row, sprite; // used for animation; mod 8 == 0: sprite1, mod 8 == 1: sprite2
	int c = 0;
	int alienType = 0;
	// draw alien if there is one
	for (row = 0; row < ALIENS_MAX_NUMBER_Y * 2; row++) {
		for (k = 0; k < ALIENS_MAX_NUMBER_X; k++) {
			if ((row % 2) == 0) {			// display aliens every even row
				alienType = *(pAliens + c * (ALIENS_MAX_NUMBER_X)+k); 	// get type of alien //alienBlock[c][k]

				if (alienType != 0) {		// if there is an alien to display
					// this logic for slowing display aliens animation
					if (frame % FRAMES_PER_SEC < FRAMES_PER_SEC / 2) sprite = 0;
					else sprite = 1;

					wattrset(wAliens, COLOR_PAIR(colors[alienType - 1]));		   // set color
					waddstr(wAliens, alienShips[alienType - 1 + (3 * ((level - 1) % 3))].GetSprite(sprite));

					if (alienType > 4) {
						*(pAliens + c * ALIENS_MAX_NUMBER_X + k) = (alienType + 1) % 9;
					} // todo: what's that? If alien_type > 4 then do a modulo operation???
				}
				else {
					waddstr(wAliens, "   ");	// no alien
				}

			}
			else {
				waddstr(wAliens, "   ");		// no aliens at odd rows
			}
		}
		if ((row % 2) == 1) { c++; } // goto next row at alienblock
	}
}

/**
 * initialize battlefield
 */
void CView::battleFieldInit()
{
	wEmpty = newpad(SCREENHEIGHT, SCREENWIDTH);
	wclear(wEmpty);

	wBattleField = newwin(SCREENHEIGHT, SCREENWIDTH, 0, 0);	// new window
	wclear(wBattleField);						// clear window
	mvwin(wBattleField, 0, 0);					// move window
}

/**
 * clear battlefield
 */
void CView::battleFieldClear()
{
	copywin(wEmpty, wBattleField, 0, 0, 0, 0, SCREENHEIGHT - 1, SCREENWIDTH - 1, 0);
}

/**
 * initialize bunkers sprites
 */
void CView::bunkersInit()
{
	wBunkers = newpad(BUNKERHEIGHT, BUNKERWIDTH);		// new pad data
	wclear(wBunkers);
}

/**
 * clear bunkers sprite
 */
void CView::bunkersClear()
{
	copywin(wEmpty, wBattleField, 0, 0, BUNKERY, BUNKERX, 
		BUNKERY + BUNKERHEIGHT - 1, BUNKERX + BUNKERWIDTH - 1, 0);
}

/**
 * display bunkers sprite
 * needs pointer to bunker-array
 */
void CView::bunkersDisplay(const char *pBunker)
{
	wclear(wBunkers);
	wattrset(wBunkers, COLOR_PAIR(CYAN));
	int l, k;
	for (l = 0; l < BUNKERHEIGHT; l++) {
		for (k = 0; k < BUNKERWIDTH; k++) {
			waddch(wBunkers, *(pBunker + (l * (BUNKERWIDTH + 1)) + k));
		}
	}

	copywin(wBunkers, wBattleField, 0, 0, BUNKERY, BUNKERX, 
		BUNKERY + BUNKERHEIGHT - 1, BUNKERX + BUNKERWIDTH - 1, 0);
}

/**
 * do proper cleanup
 */
void CView::finish(int sig)
{
	endwin();	// <curses.h> reset terminal into proper non-visual mode
	exit(0);
}

/**
 * initialize pdCurses
 */
void CView::graphicEngineInit() {
	(void)signal(SIGINT, finish);	// <signal.h> on signal "SIGINT" call method "finish"
	(void)initscr();		// <curses.h> do initialization work 

	keypad(stdscr, TRUE);		// <curses.h> enable keypad for input
	(void)nonl();			// <curses.h> disable translation return/ newline for detection of return key
	(void)cbreak();		// <curses.h> do not buffer typed characters, use at once
	(void)noecho();		// <curses.h> do not echo typed characters
	start_color();			// <curses.h> use colors
	(void)curs_set(0);		// <curses.h> hide cursor
	init_pair(RED, COLOR_RED, COLOR_BLACK);		// <curses.h> define color-pair
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);	// <curses.h> define color-pair
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);	// <curses.h> define color-pair
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);	// <curses.h> define color-pair
	init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);	// <curses.h> define color-pair
	init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);	// <curses.h> define color-pair
	init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);	// <curses.h> define color-pair

	// initialize sprites 
	battleFieldInit();
	playerInit();
	playerMissileInit();
	aliensInit();
	aliensMissileInit();
	bunkersInit();
	ufoInit();
	gameOverInit();
	statusInit();
	pauseInit();
	titleScreenInit();
}

/**
 * initialize gameover graphic
 */
void CView::gameOverInit()
{
	// init game-over banner
	wGameOver = newpad(13, 31);
	wclear(wGameOver);
	wattrset(wGameOver, COLOR_PAIR(WHITE));
	waddstr(wGameOver, "                               ");
	waddstr(wGameOver, "  #####   ####  ##   ## ###### ");
	waddstr(wGameOver, " ##      ##  ## ####### ##     ");
	waddstr(wGameOver, " ## ###  ###### ## # ## #####  ");
	waddstr(wGameOver, " ##  ##  ##  ## ##   ## ##     ");
	waddstr(wGameOver, "  #####  ##  ## ##   ## ###### ");
	waddstr(wGameOver, "                               ");
	waddstr(wGameOver, "  ####  ##   ## ###### ######  ");
	waddstr(wGameOver, " ##  ## ##   ## ##     ##   ## ");
	waddstr(wGameOver, " ##  ##  ## ##  #####  ######  ");
	waddstr(wGameOver, " ##  ##  ## ##  ##     ##  ##  ");
	waddstr(wGameOver, "  ####    ###   ###### ##   ## ");
	waddstr(wGameOver, "                               ");
}

/**
 * display game over graphic
 */
void CView::gameOverDisplay()
{
	int x = (SCREENWIDTH / 2) - (31 / 2);
	int y = (SCREENHEIGHT / 2) - (13 / 2);
	copywin(wGameOver, wBattleField, 0, 0, y, x, y + 12, x + 30, 0);
	prefresh(wGameOver, 0, 0, y, x, y + 12, x + 30);
}

/**
 * initialize pause
 */
void CView::pauseInit()
{
	wPause = newpad(1, 7);
	wclear(wPause);
}

/**
 * display pause
 */
void CView::pauseDisplay()
{
	int xOffset = (SCREENWIDTH / 2);

	wclear(wPause);
	wattrset(wPause, COLOR_PAIR(YELLOW));
	waddstr(wPause, "PAUSE");

	copywin(wPause, wBattleField, 0, 0, SCREENHEIGHT - 1, xOffset + 33, SCREENHEIGHT - 1, xOffset + 40, 0);
	prefresh(wPause, 0, 0, SCREENHEIGHT - 1, xOffset + 33, SCREENHEIGHT - 1, xOffset + 40);
}

/**
 * clear pausefield
 */
void CView::pauseClear()
{
	int xOffset = (SCREENWIDTH / 2);
	copywin(wEmpty, wBattleField, 0, 0, SCREENHEIGHT - 1, xOffset + 33, SCREENHEIGHT - 1, xOffset + 40, 0);
}


/**
 * initialize player sprites
 */
void CView::playerInit()
{
	wPlayer = newpad(1, PLAYERWIDTH);       // new pad with appropriate size
	wclear(wPlayer);			// clear pad
	wattrset(wPlayer, COLOR_PAIR(YELLOW));	// set color
	waddstr(wPlayer, cPlayer->GetSprite());	        // set sprite
}

/**
 * clear player sprite
 */
void CView::playerClear()
{
	const int x = cPlayer->GetPosX();
	const int y = cPlayer->GetPosY();
	copywin(wEmpty, wBattleField, 0, 0, y, x, y, x + PLAYERWIDTH - 1, 0);
}

/**
 * let player explode
 */
void CView::playerExplode() {
	playerExplosionDisplay();
	playerDisplay();
}

/**
 * display player sprite
 */
void CView::playerDisplay() {
	const int x = cPlayer->GetPosX();
	const int y = cPlayer->GetPosY();
	copywin(wPlayer, wBattleField, 0, 0, y, x, y, x + PLAYERWIDTH - 1, 0);
}

/**
 * some explosion animation
 */
void CView::playerExplosionDisplay()
{
	const int FRAMES_PER_SEC = 16;
	const char playerExplosionChars[] = { "@~`.,^#*-_=\\/%{}" };
	const int x = cPlayer->GetPosX();
	const int y = cPlayer->GetPosY();

	WINDOW* wPlayerExplosion;
	wPlayerExplosion = newpad(1, PLAYERWIDTH);		// new pad
	wattrset(wPlayerExplosion, COLOR_PAIR(YELLOW));	// set color

	int t, s;
	for (t = 0; t < 5; t++) { 			// 5 frames
		wclear(wPlayerExplosion);	// clear pad
		for (s = 0; s < PLAYERWIDTH; s++) {
			waddch(wPlayerExplosion, playerExplosionChars[rand() % FRAMES_PER_SEC]);	// sprite
		}

		copywin(wPlayerExplosion, wBattleField, 0, 0, y, x, y, x + PLAYERWIDTH - 1, 0); 	// display explostion
		// refresh to display explosion frames
		prefresh(wPlayerExplosion, 0, 0, y, x, y, x + PLAYERWIDTH - 1);
		doSleep(100);		// play animation not too fast
	}

} // todo: kann man bestimmt noch besser machen.
  // todo: Я уверен, что вы можете сделать лучше.

/**
 * initialize missile sprites
 */
void CView::playerMissileInit()
{
	wPlayerMissile = newpad(1, 1);		// new pad with appropriate size
	wclear(wPlayerMissile);			// clear pad
	wattrset(wPlayerMissile, COLOR_PAIR(WHITE));	// set color
	waddch(wPlayerMissile, cPlayer->cMissile.GetSprite());		// set sprite
}

/**
 * display missile sprite
 */
void CView::playerMissileDisplay()
{
	const int x = cPlayer->cMissile.GetPosX();
	const int y = cPlayer->cMissile.GetPosY();
	copywin(wPlayerMissile, wBattleField, 0, 0, y, x, y, x, 0);
}

/**
 * refresh screen so that modified graphic buffers get visible
 */
void CView::refreshScreen()
{
	redrawwin(wBattleField); // needed to display graphics properly at startup on some terminals
	//wrefresh(wBattleField);
	wnoutrefresh(wBattleField);
	doupdate();
}

/**
 * initialize scores
 */
void CView::statusInit()
{
	wStatus = newpad(1, 55);
	wclear(wStatus);
}

/**
 * display scores
 */
void CView::statusDisplay(const int level, const int score, const int lives)
{
	char strStatus[55];
	// "Level: 01 Score: 0001450 Lives: /-\ /-\ /-\ /-\ /-\ "
	// "1234567890123456789012345678901234567890123456789012"

	sprintf_s(strStatus, "Level: %2.2d Score: %2.7d Lives: ", level, score);

	wclear(wStatus);
	wattrset(wStatus, COLOR_PAIR(RED));
	waddstr(wStatus, strStatus);

	// show maximal five lives
	int t;
	for (t = 1; ((t <= 5) && (t < lives)); t++) {
		waddstr(wStatus, "/-\\ ");
	}

	int xOffset = (SCREENWIDTH / 2) - 24;
	copywin(wStatus, wBattleField, 0, 0, SCREENHEIGHT - 1, xOffset, SCREENHEIGHT - 1, xOffset + 54, 0);
}

/**
 * initialize title screen
 */
void CView::titleScreenInit()
{
	wTitleScreen = newpad(SCREENHEIGHT, SCREENWIDTH);
	wclear(wTitleScreen);
}

/**
 * clear title screen
 */
void CView::titleScreenClear()
{
	battleFieldClear();
}

/**
 * display title screen
 */
void CView::titleScreenDisplay()
{
	static int frame = 0;
	static int alien_type = 0;

	const int COLORS_EMOUNT = 3;
	const int colors[COLORS_EMOUNT] = { RED, GREEN, BLUE/*, RED, GREEN, BLUE, RED, GREEN, BLUE */};

	// Font: smslant
	WINDOW *wTitleText;
	wTitleText = newpad(5, 42);
	wclear(wTitleText);
	wattrset(wTitleText, COLOR_PAIR(YELLOW));
	waddstr(wTitleText, "         ____                 __          ");
	waddstr(wTitleText, "   ___  /  _/__ _  _____  ___/ /__ _______");
	waddstr(wTitleText, "  / _ \\_/ // _ \\ |/ / _ `/ _  / -_) __(_-<");
	waddstr(wTitleText, " / /__/___/_//_/___/\\_,_/\\_,_/\\__/_/ /___/");
	waddstr(wTitleText, "/_/   ");
	frame++;

	WINDOW *wTitleAliens;
	wTitleAliens = newpad(8, 11);
	wclear(wTitleAliens);

	char buffer[12];
	snprintf(buffer, sizeof(buffer), "%s = 500", cUfo->GetSprite(frame));
	wattrset(wTitleAliens, COLOR_PAIR(MAGENTA));
	waddstr(wTitleAliens, buffer);
	if ((frame = frame % 60) == 0) {
		alien_type = 0;
	}
	else if (frame == 20) {
		alien_type = 3;
	}
	else if (frame == 40) {
		alien_type = 6;
	}

	int i;
	for (i = alien_type; i < alien_type + 3; i++) {
		waddstr(wTitleAliens, "           ");
		snprintf(buffer, sizeof(buffer), "%s   = %d", alienShips[i].GetSprite(frame % 2), 
			alienShips[i].GetScore());
		wattrset(wTitleAliens, COLOR_PAIR(colors[i % COLORS_EMOUNT]));
		waddstr(wTitleAliens, buffer);
	}

	WINDOW *wStartText;
	wStartText = newpad(1, 20);
	wclear(wStartText);
	wattrset(wStartText, COLOR_PAIR(RED));
	waddstr(wStartText, "Press SPACE to start");

	int x, y;

	x = (SCREENWIDTH / 2) - (44 / 2);
	y = 0;
	copywin(wTitleText, wTitleScreen, 0, 0, y, x, y + 4, x + 44, 0);

	x = (SCREENWIDTH / 2) - (11 / 2);
	y = 8;
	copywin(wTitleAliens, wTitleScreen, 0, 0, y, x, y + 6, x + 10, 0);

	x = (SCREENWIDTH / 2) - (20 / 2);
	y = SCREENHEIGHT - 2;
	copywin(wStartText, wTitleScreen, 0, 0, y, x, y, x + 19, 0);

	copywin(wTitleScreen, wBattleField, 0, 0, 0, 0, SCREENHEIGHT - 1, SCREENWIDTH - 1, 0);

	prefresh(wTitleScreen, 0, 0, 0, 0, SCREENHEIGHT - 1, SCREENWIDTH - 1);
}

/**
 * initialize ufo sprite
 */
void CView::ufoInit()
{
	wUfo = newpad(1, UFOWIDTH);	     // new pad with appropriate size
	wclear(wUfo);    		     // clear pad
	wattrset(wUfo, COLOR_PAIR(MAGENTA)); // set color
}

/**
 * clear ufo sprite
 */
void CView::ufoClear()
{
	copywin(wEmpty, wBattleField, 0, 0, cUfo->GetPosY(), cUfo->GetPosX(),
		cUfo->GetPosY(), cUfo->GetPosX() + UFOWIDTH - 1, 0);
}

/**
 * display ufo sprite
 */
void CView::ufoDisplay()
{
	copywin(wUfo, wBattleField, 0, 0, cUfo->GetPosY(), cUfo->GetPosX(),
		cUfo->GetPosY(), cUfo->GetPosX() + UFOWIDTH - 1, 0);
}

/**
 * set actual sprite for ufo animation
 */
void CView::ufoRefresh()
{
	static int frame = 0;

	wclear(wUfo);
	wattrset(wUfo, COLOR_PAIR(MAGENTA));
	waddstr(wUfo, cUfo->GetSprite(frame));

	frame++;
}

CView::~CView()
{
}
