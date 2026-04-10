#ifndef DRAW_INFO_H
#define DRAW_INFO_H


#include <string>
#include <vector>

#include "color.h"
#include "matrix.h"

class DrawInfo
{
public:
    unsigned int start_vertex, vertex_count, 
                 start_indice, indices_count,
                 id;
    float c_x, c_y, c_z;
    std::string type; 
    Color *color;
    Matrix_4 model;
    bool use_indices;

    DrawInfo(unsigned int in_start_vertex, unsigned int in_vertex_count,
            std::string in_type, bool in_use_indices,
            unsigned int in_id, Color* in_Color,
            float in_cx, float in_cy, float in_cz,
            unsigned int in_start_indice = 0, unsigned int in_indices_count = 0):
        start_vertex(in_start_vertex), vertex_count(in_vertex_count), type(in_type), use_indices(in_use_indices), color(in_Color),
        model(), id(in_id), c_x(in_cx), c_y(in_cy), c_z(in_cz),
        start_indice(in_start_indice), indices_count(in_indices_count)
    {}
};

#endif