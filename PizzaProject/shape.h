#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "utils.h"

class Shape
{
public:
    Shape(const float& in_cx = 0.0f, const float& in_cy = 0.0f):
        vertices(), indices(), c_x(in_cx), c_y(in_cy) {}
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
    float c_x, c_y;
    std::vector <float> vertices;
    std::vector <unsigned int> indices;
};

class Circle : public Shape
{
public:
    Circle(const unsigned int& in_points, const float& in_radius = 1.0f,
                                          const float& in_cx = 0.0f,
                                          const float& in_cy = 0.0f):
        n_points(in_points), radius(in_radius), Shape(in_cx, in_cy)
    {
        create_circle();
    }
    
private:
    unsigned int n_points;
    float radius;

    void create_circle()
    {
        float step = 360.0 / float(n_points);
        
        vertices.push_back(c_x); vertices.push_back(c_y); vertices.push_back(0.0f);

        for (int i = 0; i <= n_points; i++)
        {
            float ang = utils::sexagesimal_to_radian(i * step);
            float x = c_x + radius * std::cos(ang);
            float y = c_y + radius * std::sin(ang);
            
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0f);
        }
    }
};

class CircularSector : public Shape
{
public:
    CircularSector(const unsigned int& in_points,
                    const float& in_start,
                    const float& in_end,
                    const float& in_radius = 1.0f,
                    const float& in_cx = 0.0f,
                    const float& in_cy = 0.0f):
        n_points(in_points), radius(in_radius), start_angle(in_start), end_angle(in_end), Shape(in_cx, in_cy)
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
        
        vertices.push_back(c_x); vertices.push_back(c_y); vertices.push_back(0.0f);

        for (int i = 0; i <= n_points; i++)
        {
            float ang = utils::sexagesimal_to_radian(start_angle+ (i * step));
            float x = c_x + radius * std::cos(ang);
            float y = c_y + radius * std::sin(ang);
            
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0f);
        }
    }
};

class Rectangle : public Shape
{
public:
    Rectangle(const float& in_height, // Proportion width over length
              const float& in_width,
              const float& in_cx = 0.0f,
              const float& in_cy = 0.0f):
        Shape(in_cx, in_cy)
    {
        float x_mid = in_width / 2.0f;
        float y_mid = in_height / 2.0f;

        vertices.push_back(in_cx - x_mid); vertices.push_back(in_cy + y_mid); vertices.push_back(0.0f);
        vertices.push_back(in_cx + x_mid); vertices.push_back(in_cy + y_mid); vertices.push_back(0.0f);
        vertices.push_back(in_cx + x_mid); vertices.push_back(in_cy - y_mid); vertices.push_back(0.0f);
        vertices.push_back(in_cx - x_mid); vertices.push_back(in_cy - y_mid); vertices.push_back(0.0f);
    }
};

#endif