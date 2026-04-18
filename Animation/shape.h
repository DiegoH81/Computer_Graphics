#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "matrix.h"
#include "utils.h"
#include "indices_info.h"

Color base_color(70, 130, 180, true);

class Shape
{
public:
	int id;
    Matrix_4 model;
    bool has_faces, has_edges, has_points;

    float c_x, c_y, c_z;

    std::vector <float> vertices;
    std::vector <unsigned int> indices;

    std::vector <IndicesInfo> info_faces,
                              info_edges,
                              info_points;

    Shape(const float& in_cx = 0.0f, const float& in_cy = 0.0f, const float& in_cz = 0.0f,

         const bool& in_has_faces = false,
         const bool& in_has_edges = false,
         const bool& in_has_points = false):
        vertices(), indices(), c_x(in_cx), c_y(in_cy), c_z(in_cz),

        has_faces(in_has_faces),
        has_edges(in_has_edges),
        has_points(in_has_points),

        model(),

		id(0)
		{}
    
    void draw(ShaderList& shaders)
    {
		shaders.set_mat4("UNIQUE", "model", model);
		
        if (has_faces)
        {
            for (auto &face: info_faces)
            {
				auto &color = face.color;
				shaders.set_vec3("UNIQUE", "color", color->r, color->g, color->b);
				
				
                if (!face.uses_EBO)
                    glDrawArrays(face.draw_mode, face.start_indice, face.count);
                else
                    glDrawElements(face.draw_mode, face.count, GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * face.start_indice));
            }
            
        }

        if (has_edges)
        {
            for (auto &edge: info_edges)
            {
				auto &color = edge.color;
				shaders.set_vec3("UNIQUE", "color", color->r, color->g, color->b);
				shaders.set_mat4("UNIQUE", "model", model);
				
                if (!edge.uses_EBO)
                    glDrawArrays(edge.draw_mode, edge.start_indice, edge.count);
                else
                    glDrawElements(edge.draw_mode, edge.count, GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * edge.start_indice));
            }
        }

        if (has_points)
        {
            for (auto &point: info_points)
            {
				auto &color = point.color;
				shaders.set_vec3("UNIQUE", "color", color->r, color->g, color->b);
				shaders.set_mat4("UNIQUE", "model", model);
				
                if (!point.uses_EBO)
                    glDrawArrays(point.draw_mode, point.start_indice, point.count);
                else
                    glDrawElements(point.draw_mode, point.count, GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * point.start_indice));
            }
        }
    }

    void set_face_color(int in_id, Color* in_color)
    {
        if (in_id < 0 || in_id >= info_faces.size() || in_color == nullptr)
            return;

        info_faces[in_id].color = in_color;
    }

    void set_edge_color(int in_id, Color* in_color)
    {
        if (in_id < 0 || in_id >= info_edges.size() || in_color == nullptr)
            return;

        info_edges[in_id].color = in_color;
    }

    void set_point_color(int in_id, Color* in_color)
    {
        if (in_id < 0 || in_id >= info_points.size() || in_color == nullptr)
            return;

        info_points[in_id].color = in_color;
    }

    virtual ~Shape() = default;

protected:
    virtual void add_edges(Color *in_color) {};
    virtual void add_points(Color *in_color) {};
};


class Circle : public Shape
{
public:
    Circle(const unsigned int& in_points, const float& in_radius = 1.0f,
           const float& in_cx = 0.0f, const float& in_cy = 0.0f,
           const bool& in_has_faces = false,
           const bool& in_has_edges = false
        
        ):
        n_points(in_points), radius(in_radius), Shape(in_cx, in_cy, 0.0f,
                                                      in_has_faces,
                                                      in_has_edges)
    {
        create_circle(&base_color);

        if (has_edges)
            add_edges(&base_color);
    }

private:
    unsigned int n_points;
    float radius;

    void create_circle(Color *in_color)
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

        int v_count = vertices.size() / 3;
        info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, false, in_color));
    }

    void add_edges(Color* in_color) override
    {
        int v_count = (vertices.size() / 3) - 1;

        info_edges.push_back(IndicesInfo(1, v_count, GL_LINE_LOOP, false, in_color));
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
                    const float& in_oy = 0.0f,
					const bool& in_has_faces = false,
                    const bool& in_has_edges = false):
        n_points(in_points), radius(in_radius), start_angle(in_start), end_angle(in_end),
        Shape(0.0f, 0.0f, 0.0f, 
              in_has_faces,
              in_has_edges )
    {
        create_sector(in_ox, in_oy, &base_color);        

        if (has_edges)
            add_edges(&base_color);
    }

    
private:
    unsigned int n_points;
    float radius, start_angle, end_angle;

    void create_sector(const float& in_ox, const float& in_oy, Color *in_color)
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



        int v_count = vertices.size() / 3;
        info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, false, in_color));
    }

    void add_edges(Color* in_color) override
    {
        int v_count = vertices.size() / 3;

        info_edges.push_back(IndicesInfo(0, v_count, GL_LINE_LOOP, false, in_color));
    }
};

class Rectangle : public Shape
{
public:
    Rectangle(const float& in_height,
              const float& in_width,
              const float& in_cx = 0.0f,
              const float& in_cy = 0.0f,
              const bool& in_has_faces = false,
              const bool& in_has_edges = false,
              const bool& in_has_points = false):
        Shape(in_cx, in_cy, 0.0f,
              in_has_faces,
              in_has_edges,
              in_has_points)
    {
        create_rectangle(in_height, in_width, &base_color);

        if (has_edges)
            add_edges(&base_color);
        if (has_points)
            add_points(&base_color);
    }

private:
    void create_rectangle(float in_height, float in_width, Color *in_color)
    {
        float x_mid = in_width / 2.0f;
        float y_mid = in_height / 2.0f;


        std::vector<float> l_x = {-x_mid, x_mid, x_mid, -x_mid};
        std::vector<float> l_y = {y_mid, y_mid, -y_mid, -y_mid};
        
        

        for (int i = 0; i < 4; i++)
        {
            vertices.push_back(l_x[i] + c_x); vertices.push_back(l_y[i] + c_y); vertices.push_back(0.0f);
        }

        int v_count = vertices.size() / 3;
        info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, false, in_color));
    }


    void add_edges(Color* in_color) override
    {
        int v_count = vertices.size() / 3;

        info_edges.push_back(IndicesInfo(0, v_count, GL_LINE_LOOP, false, in_color));
    }

    void add_points(Color* in_color) override
    {
        int v_count = vertices.size() / 3;

        for (int i = 0; i < v_count; i++)
            info_points.push_back(IndicesInfo(i, 1, GL_POINTS, false, in_color));
    }

};

class Elipse : public Shape
{
public:
    Elipse(const unsigned int& in_points,
           const float& in_height,
           const float& in_width,
           const float& in_cx = 0.0f,
           const float& in_cy = 0.0f,
           const bool& in_has_faces = false,
           const bool& in_has_edges = false):
        Shape(in_cx, in_cy, 0.0f,

              in_has_faces,
              in_has_edges)
    {
        create_elipse(in_height, in_width, in_points, &base_color);
        
        if (has_edges)
            add_edges(&base_color);

    }

private:
    void create_elipse(float in_height, float in_width, int in_points, Color *in_color)
    {
        vertices.push_back(c_x); vertices.push_back(c_y); vertices.push_back(0.0f);

        float step = 360.0 / float(in_points);

        for (int i = 0; i <= in_points; i++)
        {
            float ang_step = utils::ang_to_rad(i * step);

            float x = std::cos(ang_step) * in_width;
            float y = std::sin(ang_step) * in_height;

            vertices.push_back(c_x + x); vertices.push_back(c_y + y); vertices.push_back(0.0f);
        }

        int v_count = vertices.size() / 3;
        info_faces.push_back(IndicesInfo(0, v_count, GL_TRIANGLE_FAN, false, in_color));
    }

    void add_edges(Color* in_color) override
    {
        int v_count = (vertices.size() / 3) - 1;

        info_edges.push_back(IndicesInfo(1, v_count, GL_LINE_LOOP, false, in_color));
    }
};

class Pyramid : public Shape
{
public:
    Pyramid(const float& in_height,
            const float& in_base,
            const float& in_cx = 0.0f,
            const float& in_cy = 0.0f,
            const float& in_cz = 0.0f,
            const bool& in_has_faces = false,
            const bool& in_has_edges = false,
            const bool& in_has_points = false):
        Shape(in_cx, in_cy, in_cz,
             in_has_faces,
             in_has_edges,
             in_has_points), height(in_height), base(in_base)
    {
        create_pyramid(&base_color);
        
        if (has_points)
            add_points(&base_color);
    }

private:
    float height, base;

    void create_pyramid(Color *in_color)
    {
        float h = height / 2.0f;
        float b = base  / 2.0f;

        // Apex
        vertices.push_back(c_x);       vertices.push_back(c_y + h);  vertices.push_back(c_z);

        // FL FR BR BL
        vertices.push_back(c_x - b);   vertices.push_back(c_y - h);  vertices.push_back(c_z + b);
        vertices.push_back(c_x + b);   vertices.push_back(c_y - h);  vertices.push_back(c_z + b);
        vertices.push_back(c_x + b);   vertices.push_back(c_y - h);  vertices.push_back(c_z - b);
        vertices.push_back(c_x - b);   vertices.push_back(c_y - h);  vertices.push_back(c_z - b);

        // Faces
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        info_faces.push_back(IndicesInfo(0, 3, GL_TRIANGLES, true, in_color));

        indices.push_back(0); indices.push_back(2); indices.push_back(3);
        info_faces.push_back(IndicesInfo(3, 3, GL_TRIANGLES, true, in_color));

        indices.push_back(0); indices.push_back(3); indices.push_back(4);
        info_faces.push_back(IndicesInfo(6, 3, GL_TRIANGLES, true, in_color));

        indices.push_back(0); indices.push_back(4); indices.push_back(1);
        info_faces.push_back(IndicesInfo(9, 3, GL_TRIANGLES, true, in_color));

        
        // Base 
        indices.push_back(1); indices.push_back(2); indices.push_back(3);
        indices.push_back(1); indices.push_back(3); indices.push_back(4);
        info_faces.push_back(IndicesInfo(12, 6, GL_TRIANGLES, true, in_color));
    }

    void add_points(Color* in_color) override
    {
        int v_count = vertices.size() / 3;


        for (int i = 0; i < v_count; i++)
            info_points.push_back(IndicesInfo(i, 1, GL_POINTS, false, in_color));
    }
};

class Cube : public Shape
{
public:
    Cube(const float& in_size,
         const float& in_cx = 0.0f,
         const float& in_cy = 0.0f,
         const float& in_cz = 0.0f,
         const bool& in_has_faces = false,
         const bool& in_has_edges = false,
         const bool& in_has_points = false):
        Shape(in_cx, in_cy, in_cz,
              in_has_faces,
              in_has_edges,
              in_has_points), size(in_size)
    {
        create_cube(&base_color);

        if(has_points)
            add_points(&base_color);
    }
private:
    float size;

    void create_cube(Color* in_color)
    {
        float s = size / 2.0f;

        //    7   6
        //  3   2
        //
        //    4   5
        //  0   1

        vertices.push_back(c_x - s); vertices.push_back(c_y - s); vertices.push_back(c_z + s); // 0 FL
        vertices.push_back(c_x + s); vertices.push_back(c_y - s); vertices.push_back(c_z + s); // 1 FR
        vertices.push_back(c_x + s); vertices.push_back(c_y + s); vertices.push_back(c_z + s); // 2 TR
        vertices.push_back(c_x - s); vertices.push_back(c_y + s); vertices.push_back(c_z + s); // 3 TL
        vertices.push_back(c_x - s); vertices.push_back(c_y - s); vertices.push_back(c_z - s); // 4 BL
        vertices.push_back(c_x + s); vertices.push_back(c_y - s); vertices.push_back(c_z - s); // 5 BR
        vertices.push_back(c_x + s); vertices.push_back(c_y + s); vertices.push_back(c_z - s); // 6 TR back
        vertices.push_back(c_x - s); vertices.push_back(c_y + s); vertices.push_back(c_z - s); // 7 TL back

        // Front
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
        info_faces.push_back(IndicesInfo(0, 6, GL_TRIANGLES, true, in_color));

        // Back
        indices.push_back(5); indices.push_back(4); indices.push_back(7);
		indices.push_back(5); indices.push_back(7); indices.push_back(6);
        info_faces.push_back(IndicesInfo(6, 6, GL_TRIANGLES, true, in_color));

        // Left
        indices.push_back(4); indices.push_back(0); indices.push_back(3);
		indices.push_back(4); indices.push_back(3); indices.push_back(7);
        info_faces.push_back(IndicesInfo(12, 6, GL_TRIANGLES, true, in_color));

        // Right
        indices.push_back(1); indices.push_back(5); indices.push_back(6);
		indices.push_back(1); indices.push_back(6); indices.push_back(2);
        info_faces.push_back(IndicesInfo(18, 6, GL_TRIANGLES, true, in_color));

        // Top
        indices.push_back(3); indices.push_back(2); indices.push_back(6);
		indices.push_back(3); indices.push_back(6); indices.push_back(7);
        info_faces.push_back(IndicesInfo(24, 6, GL_TRIANGLES, true, in_color));

        // Bottom
        indices.push_back(4); indices.push_back(5); indices.push_back(1);
		indices.push_back(4); indices.push_back(1); indices.push_back(0);
        info_faces.push_back(IndicesInfo(30, 6, GL_TRIANGLES, true, in_color));
    }

    void add_points(Color* in_color) override
    {
        int v_count = vertices.size() / 3;


        for (int i = 0; i < v_count; i++)
            info_points.push_back(IndicesInfo (i, 1, GL_POINTS, false, in_color));
    }
};

class Cone : public Shape
{
public:
    Cone(const unsigned int& in_points,
         const float& in_height,
         const float& in_radius = 1.0f,
         const float& in_cx = 0.0f,
         const float& in_cy = 0.0f,
         const float& in_cz = 0.0f,
         const bool& in_has_faces = false,
         const bool& in_has_edges = false,
         const bool& in_has_points = false):
        Shape(in_cx, in_cy, in_cz,
              in_has_faces,
              in_has_edges,
              in_has_points), height(in_height), radius(in_radius), points(in_points)
    {
        create_cone(&base_color);
    }
private:
    float height, radius;
    unsigned int points;

    void create_cone(Color* in_color)
    {
        float h = height / 2.0f;
        float step = 360.0f / float(points);

        // Apex
        vertices.push_back(c_x); vertices.push_back(c_y + h); vertices.push_back(c_z);

        // Base
        for (int i = 0; i <= points; i++)
        {
            float ang = utils::ang_to_rad(i * step);
            float x = c_x + radius * std::cos(ang);
            float z = c_z + radius * std::sin(ang);
            vertices.push_back(x); vertices.push_back(c_y - h); vertices.push_back(z);
        }

        // Base center
        vertices.push_back(c_x); vertices.push_back(c_y - h); vertices.push_back(c_z);
        unsigned int center = points + 2;

        
        // Lateral
        for (unsigned int i = 1; i <= points; i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        info_faces.push_back(IndicesInfo(0, points * 3, GL_TRIANGLES, true, in_color));

        // Base
        unsigned int base_start = points * 3;
        for (unsigned int i = 1; i <= points; i++)
        {
            indices.push_back(center);
            indices.push_back(i + 1);
            indices.push_back(i);
        }
        info_faces.push_back(IndicesInfo(base_start, points * 3, GL_TRIANGLES, true, in_color));
    }

};

class Sphere : public Shape
{
public:
    Sphere(const unsigned int& in_points,
           const float& in_radius = 1.0f,
           const float& in_cx = 0.0f,
           const float& in_cy = 0.0f,
           const float& in_cz = 0.0f,
           const bool& in_has_faces = false):
        Shape(in_cx, in_cy, in_cz,
              in_has_faces), points(in_points), radius(in_radius)
    {
        create_sphere(&base_color);
    }

private:
    unsigned int points;
    float radius;

    void create_sphere(Color* in_color)
    {
        float stack_step  = 180.0f / float(points);
        float sector_step = 360.0f / float(points);

        for (int i = 0; i <= points; i++)
        {
            float cur_stack_ang  = utils::ang_to_rad(-90.0f + (i * stack_step));
            float cos_stack = std::cos(cur_stack_ang);
            float sin_stack = std::sin(cur_stack_ang);

            for (int j = 0; j <= points; j++)
            {
                float cur_sector_ang = utils::ang_to_rad(j * sector_step);
                float cos_sector = std::cos(cur_sector_ang);
                float sin_sector = std::sin(cur_sector_ang);

                float x = (radius * cos_stack) * cos_sector;
                float y = (radius * cos_stack) * sin_sector;
                float z = radius * sin_stack;

                vertices.push_back(x + c_x);
                vertices.push_back(y + c_y);
                vertices.push_back(z + c_z);
            }
        }

        // Face info
        for (unsigned int i = 0; i < points; i++)
        {
            for (unsigned int j = 0; j < points; j++)
            {
                unsigned int tl = i * (points + 1) + j;
                unsigned int tr = i * (points + 1) + j + 1;
                unsigned int bl = (i + 1) * (points + 1) + j;
                unsigned int br = (i + 1) * (points + 1) + j + 1;

                indices.push_back(tl); indices.push_back(bl); indices.push_back(tr);
                indices.push_back(tr); indices.push_back(bl); indices.push_back(br);
            }
        }

        info_faces.push_back(IndicesInfo(0, points * points * 6, GL_TRIANGLES, true, in_color));
    }
};

#endif