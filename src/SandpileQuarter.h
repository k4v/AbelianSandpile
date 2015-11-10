/** Copyright (C) {2015}  {Karthik Venugopal} **/

#ifndef SANDPILEQUARTER_H
#define SANDPILEQUARTER_H

#include <algorithm>
#include <cstdint>
#include <vector>

#include "Constants.h"

class SandpileQuarter
{
    public:
        SandpileQuarter();
        virtual ~SandpileQuarter();

        void trigger(uint64_t max_iterations=0);
        void drawSandpile();

    protected:

    private:
        std::vector<std::vector<uint32_t>> quarter_matrix;
        uint16_t max_rows;
        uint16_t max_cols;

        bool addValueAtIndex(uint16_t x, uint16_t y, int value_change);
        bool toppleToTarget(uint16_t x, uint16_t y, uint16_t t_x, uint16_t t_y);

        void printQuarterMatrix();

        struct ImageData
        {
            uint16_t image_height;
            uint16_t image_width;
            uint8_t* data;
            size_t size;

            ImageData()
            {
                image_width = image_height = 0;
            }

            ImageData(uint16_t img_height, uint16_t img_width)
            {
                image_height = img_height;
                image_width  = img_width;
            }
        } ppm_image;

        void generateImageData();
};

#endif // SANDPILEQUARTER_H
