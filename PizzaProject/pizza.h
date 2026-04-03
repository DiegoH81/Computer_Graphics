#ifndef PIZZA_H
#define PIZZA_H

#include <vector>
#include <string>

#include "shape.h"
#include "color.h"

class DrawInfo
{
public:
    unsigned int start_vertex, vertex_count, slice_id;
    float x, y;
    std::string type;
    Color *color;
    bool use_indices;

    DrawInfo(unsigned int in_start_vertex, unsigned int in_vertex_count,
            std::string in_type, bool in_use_indices,
            unsigned int in_slice_id, Color* in_Color = nullptr):
        start_vertex(in_start_vertex), vertex_count(in_vertex_count), type(in_type), use_indices(in_use_indices), color(in_Color),
        x(0.0f), y(0.0f), slice_id(in_slice_id)
    {}
};

class PizzaSlice
{
public:
    float start_angle, end_angle;
    CircularSector inner_slice, outward_slice;
    std::vector<Shape*> toppings;

    PizzaSlice(const float& in_start_angle, const float& in_end_angle, const float& radius_inner, const float& radius_outward):
        start_angle(in_start_angle), end_angle(in_end_angle),
        inner_slice(30, start_angle, end_angle, radius_inner),
        outward_slice(30, start_angle, end_angle, radius_outward),
        toppings()
    {}

    ~PizzaSlice()
    {
        for (auto &s: toppings)
            delete s;

        toppings.clear();
    }

};

class Pizza
{
public:
    std::vector<float> vertices;
    std::vector<DrawInfo> info;

    Pizza(unsigned int in_slices):
        n_slices(in_slices), slices(), vertices(), info(),
        outward_color(157, 90, 18, true), inner_color(231, 175, 70, true)
    {
        float step = 360.0 / float(n_slices);
        for (int c = 0; c < n_slices; c++)
            slices.push_back(PizzaSlice(c * step, (c + 1) * step, 0.65f, 0.70f));
    }

    void create_vertices()
    {
        vertices.clear();
        info.clear();

        std::vector<float> outward_vertices;
        std::vector<float> inward_vertices;
        unsigned int o_v_count = 0, i_v_count = 0;


        for (auto &it : slices)
        {
            std::vector<float> i_vertices = it.inner_slice.get_vertices();
            std::vector<float> o_vertices = it.outward_slice.get_vertices();
            
            outward_vertices.insert(outward_vertices.end(), o_vertices.begin(), o_vertices.end());
            o_v_count = o_vertices.size() / 3;

            
            inward_vertices.insert(inward_vertices.end(), i_vertices.begin(), i_vertices.end());
            i_v_count = i_vertices.size() / 3;
        }


        unsigned int count = 0;
        // Outward
        vertices.insert(vertices.end(), outward_vertices.begin(), outward_vertices.end());
        for (int i = 0; i < n_slices; i++)
        {
            info.push_back(DrawInfo(count, o_v_count, "OUTWARD SLICE", false, i, &outward_color));
            count += o_v_count;
        }
        
        // Inward
        vertices.insert(vertices.end(), inward_vertices.begin(), inward_vertices.end());
        for (int i = 0; i < n_slices; i++)
        {
            info.push_back(DrawInfo(count, i_v_count, "INWARD SLICE", false, i, &inner_color));
            count += i_v_count;
        }
    }

    void move_slice(int slice_index, float mov_x, float mov_y)
    {
        for (auto &m_i : info)
        {
            if (m_i.slice_id == slice_index)
            {
                m_i.x += mov_x;
                m_i.y += mov_y;
            }
        }
    }

    void render(ShaderList& shaders)
    {
        for (auto &m_i : info)
        {
            auto &color = m_i.color;
            shaders.set_vec3("UNIQUE", "color", color->r, color->g, color->b);
            shaders.set_vec2("UNIQUE", "offset", m_i.x, m_i.y);
            glDrawArrays(GL_TRIANGLE_FAN, m_i.start_vertex, m_i.vertex_count);
        }

    }

private:
    unsigned int n_slices;
    std::vector<PizzaSlice> slices;
    Color outward_color, inner_color;
};

#endif