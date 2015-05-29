#include "SandpileQuarter.h"
#include <iostream>
#include <cstdio>

SandpileQuarter::SandpileQuarter(const int edge_length)
{
    cout<<"Creating matrix with edge "<<edge_length<<endl;
    this->edge_length = edge_length;

    quarter_matrix = new uint32_t*[edge_length];
    for(int i=0; i<edge_length; i++)
    {
        quarter_matrix[i] = new uint32_t[edge_length];
        fill(quarter_matrix[i], quarter_matrix[i]+edge_length, 0);
    }

    quarter_matrix[0][0] = 1<<LOG_CHIP_HEIGHT;
}

SandpileQuarter::~SandpileQuarter()
{
    for(int i=0; i<edge_length; i++)
    {
        delete[] quarter_matrix[i];
    }
    delete quarter_matrix;
}

void SandpileQuarter::trigger(uint64_t max_iterations)
{
    uint64_t iteration_count = 0;

    while(quarter_matrix[0][0] >= TOPPLE_HEIGHT)
    {
        if(max_iterations > 0 && iteration_count >= max_iterations)
            break;
        else
            iteration_count++;

        // Initial topple (iteration)
        quarter_matrix[0][0] -= 4;
        quarter_matrix[0][1] += 1;
        quarter_matrix[1][0] += 1;

        for(int i=0; i<edge_length; i++)
        {
            for(int j=0; j<edge_length; j++)
            {
                // We've already handled [0][0]
                if (i==0 && j==0)
                    continue;

                // Topple current cell
                if(quarter_matrix[i][j] >= TOPPLE_HEIGHT)
                {
                    quarter_matrix[i][j] -= 4;

                    // Target for toppled chips to settle
                    int t_i, t_j;

                    for(int k=-1; k<=1; k+=2)
                    {
                        for(int l=-1; l<=1; l+=2)
                        {
                            t_i = i+k;
                            t_j = j+l;

                            // These indices are cells in other quarters
                            if((t_i < 0) || (t_i >= edge_length) || (t_j < 0) || (t_i >= edge_length))
                                continue;

                            // Toppling to the origin in symmetric from all 4 quarters
                            if(t_i == 0 && t_j == 0)
                                quarter_matrix[t_i][t_j] += 4;

                            // Toppling to the top horizontal border in symmetric in the vertical direction
                            else if(t_i == 0 && t_j == j-1)
                                quarter_matrix[t_i][t_j] += 2;

                            // Toppling to the left vertical border in symmetric in the horizontal direction
                            else if(t_j == 0 && t_i == i-1)
                                quarter_matrix[t_i][t_j] += 2;

                            // All other cells are considered non-symmetric
                            else
                                quarter_matrix[t_i][t_j] += 1;
                        }
                    }
                }
            }
        }
    }

    cout<<"Topple completed in "<<iteration_count<<" iterations"<<endl;
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
    ppm_image = ImageData((2*edge_length) - 1);
    ppm_image.data = new uint8_t[3*ppm_image.image_width*ppm_image.image_height];

    // Save the image data as a 1D array
    for(int i=0; i<edge_length; i++)
    {
        for(int j=0; j<edge_length; j++)
        {
            // Set each of the 3 color components
            for(int k=0; k<3; k++)
            {
                // Creates this color scheme:
                // 0 -> 0   0   0
                // 1 -> 255 0   0
                // 2 -> 0   255 0
                // 3 -> 0   0   255
                int matrix_value = quarter_matrix[i][j];
                int color_component = (matrix_value == 0) ? 0 : (matrix_value == k+1 ? 255 : 0);
                ppm_image.data[3*(ppm_image.image_width*(edge_length+i-1) + edge_length+j-1) + k] = color_component;
            }
        }
    }
}
