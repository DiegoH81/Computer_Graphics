#ifndef DRAW_INFO_H
#define DRAW_INFO_H


#include <string>
#include <vector>

#include "color.h"
#include "matrix.h"

class DrawInfo
{
public:
    unsigned int start_vertex, vertex_count, slice_id;
    std::string type;
    Color *color;
    Matrix_4 model;
    bool use_indices;

    DrawInfo(unsigned int in_start_vertex, unsigned int in_vertex_count,
            std::string in_type, bool in_use_indices,
            unsigned int in_slice_id, Color* in_Color = nullptr):
        start_vertex(in_start_vertex), vertex_count(in_vertex_count), type(in_type), use_indices(in_use_indices), color(in_Color),
        model(), slice_id(in_slice_id)
    {}
};

#endif