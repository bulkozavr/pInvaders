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

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <windows.h>
#include "CGame.h"

std::vector<CGame*> CGame::m_CGame;

CGame::CGame()
{
}

CGame::CGame(CAliens* _cAliens, CBunker *_cBunker, 
	CPlayer* _cPlayer, CView* _cView, CUfo* _cUfo) :
	cAliens(_cAliens),
	cBunker(_cBunker),
	cPlayer(_cPlayer),
	cView(_cView),
	cUfo(_cUfo)
{
	resetPlayerParams();

	weite = 0;
	skill_level = 1;
	status = GAME_HIGHSCORE;
}

void CGame::doScoring(const int alienType)
{
	const int points[] = { 500, 200, 150, 100 };   	// 0: ufo, 1:red, 2:green, 3:blue

	score += points[alienType];		// every alien type does different scoring points

	// every 6000 pts player gets a new live
	if (score % 6000 == 0) {
		lives++;
	}

	drawscore();	// display score
}

void CGame::drawscore()
{
	cView->statusDisplay(level, score, lives);
}

/**
 * evaluate command line parameters
 */
void CGame::evaluateCommandLine(int argc, char **argv)
{

	// -l : set skill level
	if (argc == 3 && strcmp(argv[1], "-l") == 0) {
		if (argv[2][0] >= '0' && argv[2][0] <= '9') {
			skill_level = argv[2][0] - 48;
		}
		else {
			argc = 2;
		}
	}

	// -gpl : show GNU GPL
	if (argc == 2 && strcmp(argv[1], "-gpl") == 0) {
		cGlobals.showGpl();
	} 

	// -i : show CONTROLS
	if (argc == 2 && strcmp(argv[1], "-i") == 0) {
		cGlobals.showControls();
		exit(1);
	}

	// wrong command line: show usage
	if (argc == 2 || (argc == 3 && strcmp(argv[1], "-l") != 0)) {
		cGlobals.showVersion();
		cGlobals.showUsage();
		exit(1);
	}
}

/**
 * timer
 * this method is executed every 1 / FPS seconds
 */
VOID CALLBACK CGame::handleTimer() {
	static int aliens_move_counter = 0;
	static int aliens_shot_counter = 0;
	static int player_shot_counter = 0;
	static int ufo_move_counter = 0;
	static int title_animation_counter = 0;
	static int game_over_counter = 0;

	CGame *cGame = m_CGame.at(0);

	switch (cGame->status) {

	case GAME_NEXTLEVEL:    // go to next level

		cGame->level++;	// increase level

		cGame->initLevel();	// initialize level

		cGame->cView->playerClear();
		cGame->cPlayer->playerMoveLeft();		// move player for visualize
		cGame->cView->playerDisplay();

		aliens_move_counter = 0;
		aliens_shot_counter = 0;
		player_shot_counter = 0;
		ufo_move_counter = 0;

		cGame->speedOfAliens();

		// change status and start game!
		cGame->status = GAME_LOOP;

	case GAME_LOOP:   	 // do game handling
		// clear old position of aliens
		cGame->cView->aliensClear(cGame->cAliens->GetPosX(), cGame->cAliens->GetPosY(),
			cGame->cAliens->GetRight(), cGame->cAliens->GetBottom());

		// move aliens
		if (aliens_move_counter == 0 && cGame->cAliens->aliensMove(cGame->level) == 1) {
			// aliens reached player
			cGame->lives = 0;
			cGame->status = GAME_OVER;
		}

		if (cGame->cAliens->aliensReachedBunkers()) {
			cGame->cBunker->bunkerClear();
			cGame->cView->bunkersClear();	// clear bunkers sprites
		}

		// display remaining aliens with animation
		cGame->cView->aliensRefresh(cGame->level);
		// display aliens at new position
		cGame->cView->aliensDisplay(cGame->cAliens->GetPosX(), cGame->cAliens->GetPosY(),
			cGame->cAliens->GetRight(), cGame->cAliens->GetBottom());

		// move player missile			
		if (player_shot_counter == 0 && 
			cGame->playerMoveMissile() == 1) {
			// no aliens left
			cGame->status = GAME_NEXTLEVEL;
		}

		// move aliens' missiles
		if (aliens_shot_counter == 0 &&
			cGame->cAliens->aliensMissileMove(cGame->skill_level, cGame->cPlayer, cGame->cBunker) == 1) {
			// player was hit
			cGame->lives--;			// player looses one life
			cGame->drawscore();	        // draw score
			cGame->cView->refreshScreen();
			// display some explosion graphics
			cGame->cView->playerExplode();
			if (cGame->lives == 0) {		// if no lives left ...
				cGame->status = GAME_OVER;		// ... exit game
			}
		}
		cGame->cView->aliensMissileDisplay();

		// move ufo
		if (ufo_move_counter == 0 && 
			cGame->cUfo->ufoCanBeShown(cGame->cAliens->GetPosY())) {
			cGame->cView->ufoClear();
			if (cGame->cUfo->ufoMoveLeft()){	// move it one position to the left
				cGame->cView->ufoRefresh();
				cGame->cView->ufoDisplay();
			}
		}

		if (aliens_shot_counter++ >= 5) { aliens_shot_counter = 0; }     // speed of alien shot
		if (player_shot_counter++ >= 1) { player_shot_counter = 0; }     // speed of player shot
		if (aliens_move_counter++ >= cGame->weite) { aliens_move_counter = 0; } // speed of aliend
		if (ufo_move_counter++ >= 3) { ufo_move_counter = 0; }           // speed of ufo

		cGame->cView->refreshScreen();
		break;

	case GAME_PAUSED:    // game is paused
		cGame->cView->pauseDisplay();
		break;

	case GAME_OVER:      // game over
		if (game_over_counter == 100) {
			cGame->cView->battleFieldClear();
			cGame->status = GAME_HIGHSCORE;
			game_over_counter = 0;
		}
		else {
			cGame->cView->gameOverDisplay();
			game_over_counter++;
		}
		break;

	case GAME_EXIT:      // exit game
		cGame->finish(0);
		break;

	case GAME_HIGHSCORE: // display highscore
		if (title_animation_counter == 0) {
			cGame->cView->titleScreenDisplay();
		}

		if (title_animation_counter++ >= 6) {
			title_animation_counter = 0;
		} // speed of animation
		break;
	}
}

void CGame::finish(int sig)
{
	endwin();
	cGlobals.showGplShort();

	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "=========================================================================\n");
	fprintf(stderr, "\n");

	fprintf(stderr, "Final score: %7.7ld, Final level: %2.2d\nFinal rating... ", score, level);
	if (lives > 0)
		fprintf(stderr, "Quitter\n\n");
	else if (score < 5000)
		fprintf(stderr, "Alien Fodder\n\n");
	else if (score < 7500)
		fprintf(stderr, "Easy Target\n\n");
	else if (score < 10000)
		fprintf(stderr, "Barely Mediocre\n\n");
	else if (score < 12500)
		fprintf(stderr, "Shows Promise\n\n");
	else if (score < 15000)
		fprintf(stderr, "Alien Blaster\n\n");
	else if (score < 20000)
		fprintf(stderr, "Earth Defender\n\n");
	else if (score > 19999)
		fprintf(stderr, "Supreme Protector\n\n");

	cGlobals.showVersion();
	exit(0);
}

/**
 * initialize level: reset attributes of most units
 */
void CGame::initLevel()
{
	cView->playerClear();
	// if missile was fired clear that position
	if (cPlayer->cMissile.GetMissileFired() == 1) {
		cView->fieldClearElement(cPlayer->cMissile.GetPosX(), cPlayer->cMissile.GetPosY());
	}
	cPlayer->playerReset();
	cView->playerDisplay();

	cView->aliensClear(cAliens->GetPosX(), cAliens->GetPosY(),
		cAliens->GetRight(), cAliens->GetBottom());
	cAliens->aliensReset();
	cView->aliensMissilesClear();
	cAliens->aliensMissilesReset();
	cAliens->render(level);
	cView->ufoClear();
	cUfo->ufoReset();
	cBunker->bunkersReset();
	// display bunkers sprite
	cView->bunkersDisplay(cBunker->GetBunkerSprites());
	drawscore();
}

/**
 * move player missile and do alien/bunker hit testing
 * return value - 0: still some aliens left, 1: no aliens left
 */
const int CGame::playerMoveMissile() {
	int fNoAliensLeft = 0;	// return value
	int alienTypeHit = 0;

	// only do movements if there is a missile to move
	if (cPlayer->cMissile.GetMissileFired() == 1) {
		// clear old missile position
		cView->fieldClearElement(cPlayer->cMissile.GetPosX(), cPlayer->cMissile.GetPosY());

		cView->playerDisplay();
		// todo: check if this can be removed later if missile is fired, 
		//the middle of player is cleared
		cPlayer->cMissile.Move(); // move missile

		// if missile hits a bunker
		if (cBunker->IsVisible() &&
			cBunker->bunkersHit(cPlayer->cMissile.GetPosX(), cPlayer->cMissile.GetPosY())) {
			// clear element of bunker
			cView->fieldClearElement(cPlayer->cMissile.GetPosX(), cPlayer->cMissile.GetPosY());
			// reload player missile
			cPlayer->playerReloadMissile();
			return fNoAliensLeft;
		}

		// if missile hits an alien (TODO)
		alienTypeHit = cAliens->aliensHitCheck(cPlayer->cMissile.GetPosX(), cPlayer->cMissile.GetPosY());
		if (alienTypeHit != 0) {

			doScoring(alienTypeHit);			// increase score
			cPlayer->playerReloadMissile();		// reload missile

			cView->aliensClear(cAliens->GetPosX(), cAliens->GetPosY(),
				cAliens->GetRight(), cAliens->GetBottom()); // clear old posiiton of aliens

			cAliens->render(level);

			if (cAliens->GetShipNum() == 0) {
				fNoAliensLeft = 1;
			}

			speedOfAliens();
			return fNoAliensLeft;
		}

		// if missile hits ufo
		if (cUfo->ufoHitCheck(cPlayer->cMissile.GetPosX(), cPlayer->cMissile.GetPosY())) {
			cView->ufoClear();
			cUfo->ufoReset();
			doScoring(UFO_ALIEN_TYPE);
			cPlayer->playerReloadMissile();
			return fNoAliensLeft;
		}

		// if missile out of battlefield
		if (cPlayer->cMissile.GetPosY() < 0) {
			cPlayer->playerReloadMissile();				// reload missile
		}
		else {
			// display missile at new position
			cView->playerMissileDisplay();
		}
	}

	return fNoAliensLeft;
}

/**
 * reads input from keyboard and do action
 */
void CGame::readInput()
{
	int ch;
	static int lastmove;
	if (_kbhit()) {

		ch = getch();		// get key pressed

		switch (status) {

		case GAME_PAUSED:
			if (ch == 'p') {
				status = GAME_LOOP;
				cView->pauseClear();
			}
			break;

		case GAME_HIGHSCORE:

			if (ch == ' ') {
				cView->titleScreenClear();
				resetPlayerParams();
				status = GAME_NEXTLEVEL;
			}
			else if (ch == 'q') {	// quit game
				status = GAME_EXIT;
			}
			break;

		case GAME_OVER:
			break; // don't do anything

		default:
			if (ch == 'l' || ch == KEY_RIGHT) {	// move player right
				if (lastmove == 'l') {
					cPlayer->playerTurboOn();	// enable Turbo
				}
				else {
					cPlayer->playerTurboOff();	// disable Turbo
				}
				cView->playerClear();
				cPlayer->playerMoveRight();		// move player
				cView->playerDisplay();
				lastmove = 'l';			// remember last move for turbo mode
			}
			else if (ch == 'h' || ch == KEY_LEFT) {	// move player left
				if (lastmove == 'h') {
					cPlayer->playerTurboOn();	// enable Turbo
				}
				else {
					cPlayer->playerTurboOff();	// disable Turbo
				}
				cView->playerClear();
				cPlayer->playerMoveLeft();		// move player
				cView->playerDisplay();

				lastmove = 'h';			// remember last move for turbo mode
			}
			else if (ch == 'k' || ch == ' ') {	// shoot missile
				cPlayer->playerLaunchMissile();
			}
			else if (ch == 'p') {			// pause game until 'p' pressed again
			 // set status to game paused
				status = GAME_PAUSED;
			}
			else if (ch == 'W') {			// cheat: goto next level
				status = GAME_NEXTLEVEL;
			}
			else if (ch == 'L') {			// cheat: one more live
				lives++;
				drawscore();
			}
			else if (ch == 'q') {	// quit game
				status = GAME_EXIT;
			}
			else {		// disable turbo mode if key is not kept pressed
				lastmove = ' ';
			}

		} // switch
	}
}

void CGame::resetPlayerParams() {
	level = 0;      // reset level
	score = 0;      // reset score
	lives = 3;      // restore lives
}

/**
 * set up timer
 */
void CGame::setUpTimer(CGame* _cGame)
{
	if (_cGame != nullptr)
		m_CGame.push_back(_cGame);
	else
		exit(1);
#ifdef _WIN32
	SetTimer(NULL, 0, 10, (TIMERPROC)(&handleTimer));
#else
	const int FPS = 50;
	struct timeval myTimer;
	myTimer.it_value.tv_sec = 0;
	myTimer.it_value.tv_usec = 1000000 / FPS;
	myTimer.it_interval.tv_sec = 0;
	myTimer.it_interval.tv_usec = 1000000 / FPS;
	setitimer(ITIMER_REAL, &myTimer, NULL);

	struct sigaction myAction;
	myAction.sa_handler = &handleTimer;
	myAction.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &myAction, NULL);

	struct timeval myTimer;
	myTimer.tv_sec = 0;
	myTimer.tv_usec = 1000000 / FPS;
	SetTimer(NULL, &myTimer, 0, (TIMERPROC)&handleTimer);
#endif
}

void CGame::speedOfAliens() {
	// speed of aliens
	weite = (cAliens->GetShipNum() + (skill_level * 10) - (level * 5) + 5) / 10;
	if (weite < 0) {
		weite = 0;
	}
}

CGame::~CGame()
{
}
