#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "utils.h"

class Shape
{
public:
    float c_x, c_y, c_z;

    Shape(const float& in_cx = 0.0f, const float& in_cy = 0.0f, const float& in_cz = 0.0f):
        vertices(), indices(), c_x(in_cx), c_y(in_cy), c_z(in_cz) {}
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
            float ang = utils::ang_to_rad(i * step);
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
                    const float& in_ox = 0.0f,
                    const float& in_oy = 0.0f):
        n_points(in_points), radius(in_radius), start_angle(in_start), end_angle(in_end), Shape()
    {
        float range = end_angle - start_angle;
        float step = range / float(n_points);
        
        vertices.push_back(in_ox); vertices.push_back(in_oy); vertices.push_back(0.0f);

        for (int i = 0; i <= n_points; i++)
        {
            float ang = utils::ang_to_rad(start_angle+ (i * step));
            float x = in_ox + radius * std::cos(ang);
            float y = in_oy + radius * std::sin(ang);
            
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0f);
        }

        float mid_angle = start_angle + (end_angle - start_angle) / 2.0f;
        float mid_radius = radius / 2.0f;

        float ang = utils::ang_to_rad(mid_angle);
        c_x = in_ox + mid_radius * std::cos(ang);
        c_y = in_oy + mid_radius * std::sin(ang);

    }

    
private:
    unsigned int n_points;
    float radius, start_angle, end_angle;
};

class Rectangle : public Shape
{
public:
    Rectangle(const float& in_height,
              const float& in_width,
              const float& in_angle = 0.0f,
              const float& in_cx = 0.0f,
              const float& in_cy = 0.0f):
        Shape(in_cx, in_cy)
    {
        float x_mid = in_width / 2.0f;
        float y_mid = in_height / 2.0f;


        std::vector<float> l_x = {-x_mid, x_mid, x_mid, -x_mid};
        std::vector<float> l_y = {y_mid, y_mid, -y_mid, -y_mid};
        
        float ang = utils::ang_to_rad(in_angle);
        float cos_a = std::cos(ang);
        float sin_a = std::sin(ang);

        for (int i = 0; i < 4; i++)
        {
            auto &x = l_x[i];
            auto &y = l_y[i];

            float rot_x = x * cos_a - y * sin_a;
            float rot_y = x * sin_a + y * cos_a;

            vertices.push_back(rot_x + in_cx); vertices.push_back(rot_y + in_cy); vertices.push_back(0.0f);
        }
    }
};

class Elipse : public Shape
{
public:
    Elipse(const unsigned int& in_points,
           const float& in_height,
           const float& in_width,
           const float& in_angle = 0.0f,
           const float& in_cx = 0.0f,
           const float& in_cy = 0.0f):
        Shape(in_cx, in_cy)
    {
        float ang = utils::ang_to_rad(in_angle);
        float cos_a = std::cos(ang);
        float sin_a = std::sin(ang);

        vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);

        float step = 360.0 / float(in_points);

        for (int i = 0; i <= in_points; i++)
        {
            float ang_step = utils::ang_to_rad(i * step);

            float x = std::cos(ang_step) * in_width;
            float y = std::sin(ang_step) * in_height;

            vertices.push_back(x); vertices.push_back(y); vertices.push_back(0.0f);
        }

        for (int i = 0; i <= in_points + 1; i++)
        {
            auto &x = vertices[3 * i];
            auto &y = vertices[3 * i + 1];

            float rot_x = x * cos_a - y * sin_a;
            float rot_y = x * sin_a + y * cos_a;

            x = rot_x + in_cx; y = rot_y + in_cy;
        }
    }
};

#endif