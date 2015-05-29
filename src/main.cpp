/**
    This is an implementation of an Abelian sandpile.

    This program starts with a pre-determined amount
    of sandgrains (chips) in a cell and examines the
    result of toppling the sandpile according to the
    Abelian sandpile model. An image will be written
    to the disk of the final result.

    Copyright (C) {2015}  {Karthik Venugopal}

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
**/

#include <iostream>
#include <math.h>

#include "SandpileQuarter.h"

using namespace std;

int main()
{
    int max_edge_length = 1 + LOG_CHIP_HEIGHT/((int)(-floor(-1*log(TOPPLE_HEIGHT)/log(2))));    // Length of any edge on the quarter matrix

    SandpileQuarter sandpileQuarter = SandpileQuarter(max_edge_length);
    sandpileQuarter.trigger();
    sandpileQuarter.drawSandpile();

    return 0;
}
