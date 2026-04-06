#ifndef PIZZA_H
#define PIZZA_H

#include <vector>
#include <string>
#include <random>

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
    float start_angle, end_angle, radius;
    CircularSector inner_slice, outward_slice;
    std::vector<std::pair<Shape*, std::string>> toppings;

    PizzaSlice(const float& in_start_angle, const float& in_end_angle, const float& radius_inner, const float& radius_outward):
        start_angle(in_start_angle), end_angle(in_end_angle),
        inner_slice(30, start_angle + 0.5f, end_angle - 0.5f, radius_inner),
        outward_slice(30, start_angle, end_angle, radius_outward),
        toppings(), radius(radius_inner)
    {
        int pepperoni_count = 3;
        int pineapple_count = 2;
        int oregano_count = 30;
        int oregano_red_count = 20;
        int olive_count = 2;

        add_toppings(pepperoni_count,
                     pineapple_count,
                     oregano_count,
                     oregano_red_count,
                     olive_count);
    }

    void add_toppings(int pepperoni_count,
                      int pineapple_count,
                      int oregano_count,
                      int oregano_red_count,
                      int olive_count)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        shape_generate_valid_figures(pepperoni_count, "PEPPERONI",
                                    [&]() {return create_circle(0.05f, gen); });
            
            
        shape_generate_valid_figures(pineapple_count, "PINEAPPLE", 
                                    [&]() {return create_rectangle(0.05f, 0.08f, gen); });
        
        
        shape_generate_valid_figures(oregano_count, "OREGANO",
                                    [&]() {return create_rectangle(0.012f, 0.020f, gen); });
                        
        shape_generate_valid_figures(oregano_red_count, "OREGANO RED",
                                    [&]() {return create_rectangle(0.012f, 0.020f, gen); });
        
        
        shape_generate_valid_figures(olive_count, "OLIVE",
                                    [&]() {return create_elipse(0.05f, 0.03f, gen); });    
    }

private:
    bool is_in_range(const float& in_angle, const float &lower, const float &upper)
    {
        if (in_angle <= upper && in_angle >= lower)
            return true;

        return false;
    }

    bool validate(Shape *shape)
    {
        auto v_temp = shape->get_vertices();
        int size = v_temp.size() / 3;
        for (int i = 0; i < size; i++)
        {
            auto &x = v_temp[3 * i];
            auto &y = v_temp[3 * i + 1];

            float d = std::sqrt(x*x + y*y);
            float ang = utils::rad_to_ang(std::atan2(y, x));
            
            if (d > radius || !is_in_range(ang, start_angle, end_angle))
                return false;
        }

        return true;
    }

    template <typename CREATOR_FUNCTION>
    void shape_generate_valid_figures(int limit, std::string name, CREATOR_FUNCTION function)
    {
        for (int i = 0; i < limit; i++)
        {
            int attempts = 0;
            Shape* shape = nullptr;
    
            do
            {
                if (shape)
                    delete shape;
    
                shape = function();
                attempts++;
            } while (!validate(shape) && attempts < 100);
    
            toppings.push_back({shape, name});
        }
    }

    Circle* create_circle(float in_radius, std::mt19937& gen)
    {
        float x = 0.0f , y = 0.0f;
           
        std::uniform_real_distribution<> RAD(0.1f, radius - in_radius); 
        std::uniform_real_distribution<> ANG_R(start_angle + 1.0f, end_angle - 1.0f);
        
        float ang_to_convert = ANG_R(gen);
        float ang = utils::ang_to_rad(ang_to_convert);
        float new_radius = RAD(gen);

        x = new_radius * std::cos(ang);
        y = new_radius * std::sin(ang);
        
        return new Circle(40, in_radius, x, y);
    }

    Rectangle* create_rectangle(float in_height, float in_width, std::mt19937& gen)
    {
        float x = 0.0f, y = 0.0f;

        std::uniform_real_distribution<> RAD(0.1f, radius); 
        std::uniform_real_distribution<> ANG_R(start_angle + 1.0f, end_angle - 1.0f); 

        float ang_to_convert = ANG_R(gen);
        float ang = utils::ang_to_rad(ang_to_convert);
        float ang_to_rotate = 0.0f;
        float new_radius = RAD(gen);

        x = new_radius * std::cos(ang);
        y = new_radius * std::sin(ang);

        std::uniform_real_distribution<> ANG_MOV(0.0f, 360.0f); 
        ang_to_rotate = ANG_MOV(gen);

        return new Rectangle(in_height, in_width, ang_to_rotate, x, y);
    }

    Elipse* create_elipse(float in_height, float in_width, std::mt19937& gen)
    {
        float x = 0.0f, y = 0.0f;

        std::uniform_real_distribution<> RAD(0.1f, radius); 
        std::uniform_real_distribution<> ANG_R(start_angle + 1, end_angle - 1);

        float ang_to_convert = ANG_R(gen);
        float ang = utils::ang_to_rad(ang_to_convert);
        float ang_to_rotate = 0.0f;    
        float new_radius = RAD(gen);

        x = new_radius * std::cos(ang);
        y = new_radius * std::sin(ang);

        std::uniform_real_distribution<> ANG_MOV(0.0f, 360.0f); 
        ang_to_rotate = ANG_MOV(gen);

        return new Elipse(40, in_height, in_width, ang_to_rotate, x, y);
    }

    /*
    ~PizzaSlice()
    {
        for (auto &s: toppings)
        delete s.first;
        
        toppings.clear();
    }
    */
    
};

class Pizza
{
public:
    std::vector<float> vertices;
    std::vector<DrawInfo> info;

    Pizza(unsigned int in_slices):
        n_slices(in_slices), slices(), vertices(), info(),
        outward_color(218, 159, 110, true),
        inner_color(227, 206, 165, true),
        pepperoni_color(108, 1, 1, true),
        pineapple_color(238, 192, 1, true),
        oregano_color(136, 134, 62, true),
        olive_color(11, 9, 4, true),
        oregano_red_color(77, 22, 14, true)
    {
        slices.reserve(n_slices);
        float step = 360.0 / float(n_slices);
        for (int c = 0; c < n_slices; c++)
            slices.push_back(PizzaSlice(c * step, (c + 1) * step, 0.65f, 0.70f));
    }

    void create_vertices()
    {
        vertices.clear();
        info.clear();
        unsigned int count = 0;
        for (int i = 0; i < n_slices; i++)
        {
            std::vector<float> i_vertices = slices[i].inner_slice.get_vertices();
            std::vector<float> o_vertices = slices[i].outward_slice.get_vertices();

            int o_size = o_vertices.size() / 3;
            vertices.insert(vertices.end(), o_vertices.begin(), o_vertices.end());
            info.push_back(DrawInfo(count, o_size, "OUTWARD SLICE", false, i, &outward_color));
            count += o_size;


            int i_size = i_vertices.size() / 3;
            vertices.insert(vertices.end(), i_vertices.begin(), i_vertices.end());
            info.push_back(DrawInfo(count, i_size, "INWARD SLICE", false, i, &inner_color));
            count += i_size;


            for (auto &t: slices[i].toppings)
            {
                auto topping_vertices = t.first->get_vertices();
                int topping_size = topping_vertices.size() / 3;
                
                vertices.insert(vertices.end(), topping_vertices.begin(), topping_vertices.end());
                if (t.second == "PEPPERONI")
                    info.push_back(DrawInfo(count, topping_size, "PEPPERONI", false, i, &pepperoni_color));
                else if (t.second == "PINEAPPLE")
                    info.push_back(DrawInfo(count, topping_size, "PINEAPPLE", false, i, &pineapple_color));
                else if (t.second == "OREGANO")
                    info.push_back(DrawInfo(count, topping_size, "OREGANO", false, i, &oregano_color));
                else if (t.second == "OREGANO RED")
                    info.push_back(DrawInfo(count, topping_size, "OREGANO RED", false, i, &oregano_red_color));
                else if (t.second == "OLIVE")
                    info.push_back(DrawInfo(count, topping_size, "OLIVE", false, i, &olive_color));
                count += topping_size;
            }
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
    Color outward_color,
          inner_color,
          pepperoni_color,
          pineapple_color,
          oregano_color,
          olive_color,
          oregano_red_color;
};

#endif