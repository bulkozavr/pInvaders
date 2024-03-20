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

#include "CPlayer.h"
#include "Dimension.h"


CPlayer::CPlayer()
{
	cMissile.SetSprite(missileSprite);
	cMissile.SetSpeed(-1);
}

/**
 * initialize player attributes
 */
void CPlayer::playerReset() {
	posY = PLAYERPOSY;	// set vertical Position
	posX = 0;		// set horizontal Position
	speed = 1;
	cMissile.SetMissileFired(0);
	cMissile.SetX(0);
	cMissile.SetY(0);
}

/**
 * move player left
 */
void CPlayer::playerMoveLeft() {
	// check if space between player and border of screen is big enough 
	if (posX > speed) {
		// desired movement is possible
		posX -= speed;
	}
	else {
		// space too small, move to left-most position
		posX = 0;
	}
}

/**
 * move player right
 */
void CPlayer::playerMoveRight() {
	// check if space between player and border of screen is big enough 
	if (posX < SCREENWIDTH - PLAYERWIDTH - speed) {
		// desired movement is possible
		posX += speed;
	}
	else {
		// space too small, move to right-most position
		posX = SCREENWIDTH - PLAYERWIDTH;
	}
}

/**
 * toggle turbo mode on (if key is kept pressed)
 */
void CPlayer::playerTurboOn() {
	speed = 2;
}

/**
 * toggle turbo mode off (if key is kept pressed)
 */
void CPlayer::playerTurboOff() {
	speed = 1;
}

/**
 * player was hit by an alien shot
 */
const int CPlayer::playerHitCheck(const int hostileShotX, const int hostileShotY) {
	int fPlayerWasHit = 0;

	// if shot reached vertikal position of player
	if (hostileShotY == PLAYERPOSY) {
		// if shot hits player
		if (hostileShotX >= posX && hostileShotX <= posX + PLAYERWIDTH - 1) {
			fPlayerWasHit = 1;		// set return value
		}
	}

	return fPlayerWasHit;				// return if player was hit
}

/**
 * Launch Missile
 */
void CPlayer::playerLaunchMissile() {
	// only launch missile if no other is on its way
	if (cMissile.GetMissileFired() == 0) {
		cMissile.SetMissileFired(1);	// missile is on its way
		cMissile.SetX(posX + PLAYERWIDTH / 2); // launched from the middle of player...
		cMissile.SetY(PLAYERPOSY);	// ...at same horizontal position
	}
}

/**
 * Reload Missile
 */
void CPlayer::playerReloadMissile() {
	cMissile.SetMissileFired(0);	// no player missile flying
}

/**
 * get image for display
 */
const char* CPlayer::GetSprite() {
	return sprite;
}

CPlayer::~CPlayer()
{
}
