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

#include "CBunker.h"

// clear bunkers
void CBunker::bunkerClear() {
	int cx, cy;
	for (cy = 0; cy < BUNKERHEIGHT; cy++) {
		for (cx = 0; cx < BUNKERWIDTH; cx++) {
			bunker[cy][cx] = BLANK;
		}
	}
	visible = false;
}

/**
 * check if missile hit an element of bunker
 */
bool CBunker::bunkersHit(const int shotX, const int shotY) {
	bool fBunkerWasHit = false;
	// if missile is within bunker-rectangle
	if (shotX >= BUNKERX && shotX < BUNKERX + BUNKERWIDTH
		&& shotY >= BUNKERY && shotY < BUNKERY + BUNKERHEIGHT) {
		// calculate the element of the bunker that was hit
		int adjY = shotY - BUNKERY;
		int adjX = shotX - BUNKERX;
		// if there is still an element
		if (bunker[adjY][adjX] == BRICK) {
			bunker[adjY][adjX] = BLANK;	// delete element
			fBunkerWasHit = true; 		// bunker was hit!
		}
	}
	return fBunkerWasHit;
}

/**
 * initialize bunkers attributes
 */
void CBunker::bunkersReset() {
	// set position of bunker sprites. user graphical char "bunkers" for better visual overview
	// and set according to this the bunker-array
	const char bunkers[BUNKERHEIGHT][BUNKERWIDTH + 1] = {
		"        ###                 ###                 ###                 ###         ",
		"       #####               #####               #####               #####        ",
		"      #######             #######             #######             #######       ",
		"      ##   ##             ##   ##             ##   ##             ##   ##       "
	};
	//   12345678901234567890123456789012345678901234567890123456789012345678901234567890
	// 80 characters wide

	int i, j;
	// copy graphical "bunkers" to binary "bunker"
	for (i = 0; i < BUNKERHEIGHT; i++) {
		for (j = 0; j < BUNKERWIDTH; j++) {
			bunker[i][j] = bunkers[i][j];
		}
	}
	visible = true;
}

char * CBunker::GetBunkerSprites() {
	return &bunker[0][0];
}

CBunker::~CBunker()
{
}
