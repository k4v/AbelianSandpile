/** Copyright (C) {2015}  {Karthik Venugopal} **/

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

        void trigger(uint64_t max_iterations=0);
        void drawSandpile();

    protected:

    private:
        uint32_t** quarter_matrix;
        int edge_length;

        struct ImageData
        {
            int image_height;
            int image_width;
            uint8_t* data;
            size_t size;

            ImageData()
            {
                image_width = image_height = 0;
            }

            ImageData(int image_side)
            {
                image_height = image_side;
                image_width  = image_side;
            }
        } ppm_image;

        void generateImageData();
};

#endif // SANDPILEQUARTER_H
