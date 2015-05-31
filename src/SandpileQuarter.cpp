/** Copyright (C) {2015}  {Karthik Venugopal} **/

#include "SandpileQuarter.h"
#include <cstdio>

using namespace std;

SandpileQuarter::SandpileQuarter()
{
    // Create a 1x1 matrix
    quarter_matrix = {{1<<LOG_CHIP_HEIGHT}};
    max_rows = 1;
    max_cols = 1;
}

SandpileQuarter::~SandpileQuarter()
{
    quarter_matrix.clear();
}

void SandpileQuarter::trigger(uint64_t max_iterations)
{
    cout<<"Beginning sandpile topple on 2^"<<LOG_CHIP_HEIGHT<<" chips"<<endl;

    uint64_t iteration_count = 0;
    bool cell_overflow = (quarter_matrix[0][0] >= TOPPLE_HEIGHT);

    while(cell_overflow && (max_iterations <= 0 || iteration_count < max_iterations))
    {
        cell_overflow = false;
        iteration_count++;

        if(quarter_matrix[0][0] >= TOPPLE_HEIGHT)
        {
            // Initial topple (iteration)
            cell_overflow |= addValueAtIndex(0, 0, -4);
            cell_overflow |= addValueAtIndex(0, 1, 1);
            cell_overflow |= addValueAtIndex(1, 0, 1);
        }

        for(uint16_t i=0; i<quarter_matrix.size(); i++)
        {
            for(uint16_t j=0; j<quarter_matrix[i].size(); j++)
            {
                // We've already handled [0][0]
                if (i==0 && j==0)
                    continue;

                // Topple current cell
                if(quarter_matrix[i][j] >= TOPPLE_HEIGHT)
                {
                    cell_overflow |= addValueAtIndex(i, j, -4);

                    for(int8_t k=-1; k<=1; k+=2)
                    {
                        // Topple to cells surrounding current cell
                        if(i+k >= 0)
                        {
                            cell_overflow |= toppleToTarget(i, j, i+k, j);
                        }
                        if(j+k >= 0)
                        {
                            cell_overflow |= toppleToTarget(i, j, i, j+k);
                        }
                    }
                }
            }
        }
    }

    cout<<"Topple completed in "<<iteration_count<<" iterations"<<endl;
    cout<<"Sandpile is of size ("<<(2*max_rows -1)<<", "<<(2*max_cols -1)<<")"<<endl;
}

bool SandpileQuarter::toppleToTarget(uint16_t i, uint16_t j, uint16_t t_i, uint16_t t_j)
{
    //cout<<"Toppling "<<i<<", "<<j<<" to "<<t_i<<", "<<t_j<<": ";

    // Toppling to the origin is symmetric from all 4 quarters
    //if(t_i == 0 && t_j == 0)
    //    return addValueAtIndex(t_i, t_j, 2);
    // Toppling to the top horizontal border in symmetric in the vertical direction
    if(t_i == 0 && t_j == j)
        return addValueAtIndex(t_i, t_j, 2);
    // Toppling to the left vertical border in symmetric in the horizontal direction
    else if(t_j == 0 && t_i == i)
        return addValueAtIndex(t_i, t_j, 2);
    // All other cells are considered non-symmetric
    else
        return addValueAtIndex(t_i, t_j, 1);
}

bool SandpileQuarter::addValueAtIndex(uint16_t x, uint16_t y, int value_change)
{
    //cout<<"Adding value at "<<x<<" "<<y<<" "<<value_change<<endl;
    if(x >= quarter_matrix.size())
    {
        // If x is an index more than 1+current max index, it is an error
        // TODO: Verify this
        if(x > quarter_matrix.size())
        {
            cerr<<"Possible index exception in algorithm. Trying to access index "<<x<<" on a matrix of "<<quarter_matrix.size()<<" rows";
            exit(1);
        }

        // Create a new vector with 1 element initialized to 0. Insert at index x
        quarter_matrix.push_back({0});
    }

    // Adjust row vector to required number of columns
    if(quarter_matrix[x].size() <= y)
        quarter_matrix[x].resize(y+1, 0);

    quarter_matrix[x][y] += value_change;

    // Updated maximum row and column values
    max_rows = quarter_matrix.size();
    max_cols = max(max_cols, (uint16_t)(y+1));

    return (quarter_matrix[x][y] >= TOPPLE_HEIGHT);
}

void SandpileQuarter::printQuarterMatrix()
{
    for(uint32_t i=0; i<quarter_matrix.size(); i++)
    {
        for(uint32_t j=0; j<quarter_matrix[i].size(); j++)
        {
            cout<<quarter_matrix[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void SandpileQuarter::drawSandpile()
{
    generateImageData();

    // Write image to file
    FILE* image_file = fopen("sandpile_result.ppm", "w");
    if (image_file == 0)
    {
        perror ("Error opening file");
        exit(1);
    }

    size_t n = 0;
    n += fprintf(image_file, "P6\n# Abelian sandpile\n%d %d\n%d\n",
                 ppm_image.image_width, ppm_image.image_height, 0xFF);
    n += fwrite(ppm_image.data, 1, ppm_image.image_width * ppm_image.image_height * 3, image_file);

    cout<<"Image created with "<<n<<" bytes";
}

void SandpileQuarter::generateImageData()
{
    // Create a new image array here each pixel is represented by 3 bytes.
    // This corresponds to a square image of side (2*edge_length - 1)
    ppm_image = ImageData((2*max_cols)-1, (2*max_rows)-1);
    ppm_image.data = new uint8_t[3*ppm_image.image_width*ppm_image.image_height];

    // Save the image data as a 1D array
    for(uint16_t i=0; i<max_rows; i++)
    {
        for(uint16_t j=0; j<max_cols; j++)
        {
            int matrix_value = (quarter_matrix[i].size() > j) ? quarter_matrix[i][j] : 0;

            // Set each of the 3 color components
            for(uint8_t k=0; k<3; k++)
            {
                uint8_t color_component = 0;

                // Creates this color scheme:
                // 0 -> 0   0   0
                // 1 -> 255 0   0
                // 2 -> 0   255 0
                // 3 -> 0   0   255
                if(matrix_value >= TOPPLE_HEIGHT)
                    color_component = 255;
                else
                    color_component = (matrix_value == 0) ? 0 : (matrix_value == k+1 ? 255 : 0);

                ppm_image.data[3*(ppm_image.image_width*(max_rows-1 -i) + (max_cols-1 -j)) + k] = color_component;
                ppm_image.data[3*(ppm_image.image_width*(max_rows-1 -i) + (max_cols-1 +j)) + k] = color_component;
                ppm_image.data[3*(ppm_image.image_width*(max_rows-1 +i) + (max_cols-1 -j)) + k] = color_component;
                ppm_image.data[3*(ppm_image.image_width*(max_rows-1 +i) + (max_cols-1 +j)) + k] = color_component;
            }
        }
    }
}
