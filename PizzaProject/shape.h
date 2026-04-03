#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "utils.h"

class Shape
{
public:
    Shape(): vertices(), indices() {}
    virtual ~Shape() = default;

    std::vector<float> get_vertices()
    {
        return vertices;
    }
    std::vector<unsigned int> get_indices()
    {
        return indices;
    }

    bool has_indices() { return !indices.empty(); }

protected:
    std::vector <float> vertices;
    std::vector <unsigned int> indices;
};

class Circle : public Shape
{
public:
    Circle(const unsigned int& in_points, const float& in_radius = 1.0f):
        n_points(in_points), radius(in_radius), Shape()
    {
        create_circle();
    }
    
private:
    unsigned int n_points;
    float radius;

    void create_circle()
    {
        float step = 360.0 / float(n_points);
        
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        for (int i = 0; i <= n_points; i++)
        {
            float ang = utils::sexagesimal_to_radian(i * step);
            float x = radius * std::cos(ang);
            float y = radius * std::sin(ang);
            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
    }
};

class CircularSector : public Shape
{
public:
    CircularSector(const unsigned int& in_points,
                    const float& in_start, const float& in_end,
                    const float& in_radius = 1.0f):
        n_points(in_points), radius(in_radius), start_angle(in_start), end_angle(in_end), Shape()
    {
        create_segment();
    }

    
private:
    unsigned int n_points;
    float radius, start_angle, end_angle;

    void create_segment()
    {
        float range = end_angle - start_angle;
        float step = range / float(n_points);
        
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        for (int i = 0; i <= n_points; i++)
        {
            float ang = utils::sexagesimal_to_radian(start_angle+ (i * step));
            float x = radius * std::cos(ang);
            float y = radius * std::sin(ang);
            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
    }
};

#endif