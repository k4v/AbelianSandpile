#include <iostream>
#include <math.h>

#include "SandpileQuarter.h"

using namespace std;

int main()
{
    int max_edge_length = 1 + LOG_CHIP_HEIGHT/((int)(-floor(-1*log(TOPPLE_HEIGHT)/log(2))));    // Length of any edge on the quarter matrix
    SandpileQuarter sandpileQuarter = SandpileQuarter(max_edge_length);

    return 0;
}
