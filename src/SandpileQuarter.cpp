#include "SandpileQuarter.h"

SandpileQuarter::SandpileQuarter(const int edge_length)
{
    // cout<<"Creating matrix with edge "<<edge_length<<endl;
    this->edge_length = edge_length;

    quarter_matrix = new uint16_t*[edge_length];
    for(int i=0; i<edge_length; i++)
    {
        quarter_matrix[i] = new uint16_t[edge_length];
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

SandpileQuarter::trigger(int max_iterations=-1)
{

}
