#ifndef INDICES_INFO_H
#define INDICES_INFO_H

class IndicesInfo
{
public:
    unsigned int start_indice, count, draw_mode;
    IndicesInfo(unsigned int in_start_indice, unsigned int in_indices_count, unsigned int in_draw_mode):
        start_indice(in_start_indice), count(in_indices_count), draw_mode(in_draw_mode)
    {}
};

#endif