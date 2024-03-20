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

#ifndef _CMISSILE_H_
#define _CMISSILE_H_

#include "CObject.h"

class CMissile : public CObject
{
private:
	int missileFired; // 0: missile not running; 1: missile running; -1: do not show anymore
	char sprite;

public:
	CMissile();

	void SetX(int coord) {
		posX = coord;
	}
	void SetY(int coord) {
		posY = coord;
	}

	void Move() {
		posY += speed;
	}

	void SetMissileFired(const int _missleFired) {
		missileFired = _missleFired;
	}

	const int GetMissileFired() const {
		return missileFired;
	}

	void SetSpeed(int _speed) {
		speed = _speed;
	}

	void SetSprite(char _sprite) {
		sprite = _sprite;
	}
	char GetSprite() {
		return sprite;
	}

	virtual ~CMissile();
};

#endif
