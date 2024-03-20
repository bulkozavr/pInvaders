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

#include <ctime>
#include <stdlib.h>
#include "CAliens.h"


CAliens::CAliens()
{
	for (int i = 0; i < ALIENS_MAX_MISSILES; i++) {
		aliensMissile[i].SetSprite(missileSprite);
		aliensMissile[i].SetSpeed(1);
	}
}

/**
 * initialize aliens attributes
 */
void CAliens::aliensReset() {
	// three different types of aliens [5], [10]
	const int level[ALIENS_MAX_NUMBER_Y][ALIENS_MAX_NUMBER_X] = {
		{1,1,1,1,1,1,1,1,1,1},
		{2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2},
		{3,3,3,3,3,3,3,3,3,3},
		{3,3,3,3,3,3,3,3,3,3}
	};

	// reset alien position
	posX = 0;
	posY = 0;
	right = 0;
	bottom = 0;
	left = 0;
	speed = 1;

	int i, j;
	// copy level-array to enemy-array 
	for (i = 0; i < ALIENS_MAX_NUMBER_Y; i++) {
		for (j = 0; j < ALIENS_MAX_NUMBER_X; j++) {
			alienBlock[i][j] = level[i][j];
		}
	}
}

void CAliens::aliensMissilesReset() {
	for (int i = 0; i < ALIENS_MAX_MISSILES; i++) {
		aliensMissile[i].SetMissileFired(-1);
		aliensMissile[i].SetX(0);  // start with zero values
		aliensMissile[i].SetY(0);  // start with zero values
	}
	alienshotnum = 1;	    // one missile at the same time
	aliensMissile[0].SetMissileFired(1);
	aliensMissile[0].SetX(5);	    // x position of first alienshot
	aliensMissile[0].SetY(1);	    // y position of first alienshot
}

/**
 * move aliens
 */
const int CAliens::aliensMove(const int level) {
	int fReachedPlayer = 0; 				// return value

	posX += speed;			// move aliens left/ right

	// when aliens reached left or right screen-border
	if (posX == (BUNKERWIDTH + BUNKERX - 5 - right) || posX == (BUNKERX + left)) {

		posY++;				// move aliens downwards

		// aliens reached player
		if (posY == SCREENHEIGHT - 2 - bottom) {
			fReachedPlayer = 1;		// set return value
		}

		speed *= -1;		  // change direction of aliens' movements
	}

	return fReachedPlayer;				  // return if aliens reached player
}

/**
 * test if aliens have reached the
 * bottom of the windows or the bunkers.
 */
bool CAliens::aliensReachedBunkers() {
	// aliens reached bunkers //funzt nicht ganz: todo
	return (posY == BUNKERY - bottom);
}

/**
 * move aliens' missiles and do player/bunker hit testing
 * a zero value in alienshotx indicates that the appropriate missile is loaded, but not fired
 */
const int CAliens::aliensMissileMove(const int skill_level, CPlayer* cPlayer, CBunker* cBunker) {
	static int alienshot_counter = 0;

	int shootThreshold;

	// calculate threshold when next missile should be fired
	// it is done here to save calculation time in for-instruction 
	shootThreshold = (skill_level * 8) * (shipnum + 2);
	alienshot_counter += 10;

	// loop all possible missiles
	int i, tmp;
	int fPlayerWasHit = 0;
	srand(static_cast<unsigned int>(time(0)));
	for (i = 0; i < ALIENS_MAX_MISSILES; i++) {

		// if the current missile is flying we should do movements
		if (aliensMissile[i].GetMissileFired() == 1) {
			// if missile hit the bunkers	
			if (cBunker->IsVisible() &&
				cBunker->bunkersHit(aliensMissile[i].GetPosX(), aliensMissile[i].GetPosY())) {
				aliensMissile[i].SetMissileFired(0);		// value of zero reloads missile
			}
			// check if player was hit by an alien missile
			else if (cPlayer->playerHitCheck(aliensMissile[i].GetPosX(), aliensMissile[i].GetPosY()) == 1) {
				// value -1 needs because we don't want display missile after
				// hit player. Player's animation will erase missile
				aliensMissile[i].SetMissileFired(-1);
				fPlayerWasHit = 1;
			}
			else if (aliensMissile[i].GetPosY() == SCREENHEIGHT - 2) {
				aliensMissile[i].SetMissileFired(0);	// reload missile	
			} else {
				aliensMissile[i].Move();			// move missile downwards
			}
		}
		else {					// missile not launched yet

		 // start new missile if counter says so
			if (alienshot_counter > shootThreshold && shipnum > 0) {
				// only shot if there's an alien left
				alienshot_counter = 0;				// reset counter				
				// randomly select one of the ...
				do {		// ...aliens at the bottom of ...
					tmp = rand() % ALIENS_MAX_NUMBER_X;	// ...a column to launch missile
				} while (lowest_ship[tmp] == -1);
				aliensMissile[i].SetMissileFired(1);
				aliensMissile[i].SetY(posY + lowest_ship[tmp]);		// set y position of missile
				aliensMissile[i].SetX(posX + tmp * 3);			// set x position of missile
			}
		} // if 
	} // for

	return fPlayerWasHit;
}

/**
 * check if missile hit an alien
 */
const int CAliens::aliensHitCheck(const int shotX, const int shotY) {
	int alienType = 0;
	// if missile is within alien-rectangle 
	if (shotX >= posX && shotX <= posX + ALIENS_MAX_NUMBER_X * 3 - 1
		&& shotY >= posY && shotY <= posY + (ALIENS_MAX_NUMBER_Y - 1) * 2) {
		// calculate the ship that was hit
		int shipX = (shotX - posX) / 3;
		int shipY = (shotY - posY) / 2;
		// if there is still a ship at this position
		alienType = alienBlock[shipY][shipX];
		if (alienType != 0) {
			alienBlock[shipY][shipX] = 0;	// delete alien ship
		}
	}
	return alienType; 	// returns 0 if no alien was hit, else returns type-code of alien
}

/**
 * display alien animation, display remaining parts of aliens
 */
void CAliens::render(const int level) {
	// calculate left, right, bottom, lowest_ship	
	left = 1;
	right = -1;
	bottom = -1;
	shipnum = 0;
	int k, row;
	for (k = 0; k < 10; k++) {
		lowest_ship[k] = -1;
	}

	int c = 0;
	for (row = 0; row < ALIENS_MAX_NUMBER_Y * 2; row++) {
		if ((row % 2) == 0) {
			for (k = 0; k < ALIENS_MAX_NUMBER_X; k++) {
				if (alienBlock[c][k] != 0) {
					lowest_ship[k] = row;
					shipnum++;
					if (left == 1 || -k > left) { left = -k; }
					if (right == -1 || k > right) { right = k; }
					if (bottom == -1 || c > bottom) { bottom = c; }
				}
			}
		}
		else {
			c++;
		}
	}
	bottom = bottom * 2;	// every 2nd row is an empty row
	left = left * 3; // alien sprite is 3 chars wide
	right = right * 3; // alien sprite is 3 chars wide
}

CAliens::~CAliens()
{
}
