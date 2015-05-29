#ifndef SANDPILEQUARTER_H
#define SANDPILEQUARTER_H

#include <algorithm>
#include <cstdint>
#include "Constants.h"

using namespace std;

class SandpileQuarter
{
    public:
        SandpileQuarter(const int edge_length);
        virtual ~SandpileQuarter();

        trigger(int max_iterations);
        draw_sandpile();

    protected:

    private:
        uint16_t** quarter_matrix;
        int edge_length;
};

#endif // SANDPILEQUARTER_H
