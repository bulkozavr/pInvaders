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

#ifndef _CALIENS_H_
#define _CALIENS_H_

#include "Dimension.h"
#include "CObject.h"
#include "CBunker.h"
#include "CMissile.h"
#include "CPlayer.h"

class CAliens : public CObject
{
private: 

	const char missileSprite = ':';

	int shipnum;

	int right;
	int left;
	int bottom;

	int lowest_ship[ALIENS_MAX_NUMBER_X];
	CMissile aliensMissile[ALIENS_MAX_MISSILES];
	int alienBlock[ALIENS_MAX_NUMBER_Y][ALIENS_MAX_NUMBER_X];
	int alienshotnum;

public:
	CAliens();

	bool aliensReachedBunkers();
	const int aliensHitCheck(const int shotX, const int shotY);
	void aliensReset();
	void aliensMissilesReset();
	const int aliensMove(const int);
	const int aliensMissileMove(const int skill_level, CPlayer*, CBunker *);
	const int GetShipNum() const {
		return shipnum;
	}
	const int GetRight() const {
		return right;
	}
	const int GetBottom()const {
		return bottom;
	}
	void render(const int);

	int* GetAlienBlock() {
		return &alienBlock[0][0];
	}

	CMissile* GetAliensMissiles() {
		return &aliensMissile[0];
	}

	char GetAlienMissileSprite(int index) {
		return aliensMissile[index].GetSprite();
	}

	virtual ~CAliens();
};

#endif
