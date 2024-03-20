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

#ifndef _CUFO_H_
#define _CUFO_H_

#include "CObject.h"

class CUfo : public CObject
{
private:
	/*static*/ bool fShowUfo;
	const int NUMBER_OF_SPRITES = 4;

	const char ufoSprites[4][6] = { "<o o>", "<oo >", "<o o>", "< oo>" };

public:

	CUfo();

	void ufoReset();
	const bool ufoCanBeShown(const int);
	bool ufoMoveLeft();
	const bool ufoHitCheck(const int shotX, const int shotY);
	const char* GetSprite(const int);

	virtual ~CUfo();
};

#endif
