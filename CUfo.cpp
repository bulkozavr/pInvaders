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

#include <stdlib.h>
#include "CUfo.h"
#include "Dimension.h"



CUfo::CUfo() {
	fShowUfo = false;
	speed = -1;
}

/**
 * initialize ufo attributes
 */
void CUfo::ufoReset() {
	fShowUfo = false;                   // do not show ufo
	posY = UFOPOSY;	        // set vertical Position
	posX = SCREENWIDTH - UFOWIDTH;// set horizontal Position
}

/**
 * check if the first screen line is free for the ufo and
 * display it randomly
 */
const bool CUfo::ufoCanBeShown(const int aliensPosY) {
	if (aliensPosY > 0 && fShowUfo == 0) { // aliens one line down
		if ((rand() % 200) == 0) {
			fShowUfo = true;
		}
	}

	return fShowUfo;
}

/**
 * move ufo and check if it reached the left screen border
 */
bool CUfo::ufoMoveLeft() {
	// check if space between ufo and screen border is big enough 
	if (posX > 0) {
		// desired movement is possible
		posX += speed;
		return true;
	}
	ufoReset();
	return false;
}

/**
 * check if ufo was hit by player and delete it from screen
 */
const bool CUfo::ufoHitCheck(const int shotX, const int shotY) {
	bool fUfoWasHit = false;

	// if shot reached vertical position of ufo
	if (shotY == UFOPOSY) {
		// if shot hits ufo
		if (shotX >= posX && shotX <= posX + UFOWIDTH - 1) {
			fUfoWasHit = true;
		}
	}

	return fUfoWasHit;
}

/**
 * get frame for display
 */
const char* CUfo::GetSprite(const int sprite) {
	return ufoSprites[sprite % NUMBER_OF_SPRITES];
}


CUfo::~CUfo()
{
}
