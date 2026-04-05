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
        add_toppings(3, 20, 0.05f);
    }

    void add_toppings(int n_toppings, int n_condiments, float pepperoni_radius)
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        
        
        int pepperoni_count = n_toppings * (2.0f/ 3.0f);
        int pineapple_count = n_toppings * (1.0f/ 3.0f);
        
        
        
        int counter = 0;
        while(counter < pepperoni_count)
        {
            bool valid_circle = false;
            float x = 0.0f , y = 0.0f;
            int attempts = 0;
            do
            {
                valid_circle = true;
                
                std::uniform_real_distribution<> RAD(0.1f, radius - pepperoni_radius); 
                //std::uniform_real_distribution<> ANG_R(start_angle + 1, end_angle - 1); 
                std::uniform_real_distribution<> ANG_R(start_angle + 1, end_angle - 1); 
                float ang_to_convert = ANG_R(gen);
                float ang = utils::ang_to_rad(ang_to_convert);
                float new_radius = RAD(gen);
                x = new_radius * std::cos(ang);
                y = new_radius * std::sin(ang);

                auto temp_circle = Circle(40, pepperoni_radius, x, y);

                auto v_temp = temp_circle.get_vertices();
                int size = v_temp.size() / 3;

                for (int i = 0; i < size; i++)
                {
                    auto &x = v_temp[3 * i];
                    auto &y = v_temp[3 * i + 1];

                    
                    float ang = utils::rad_to_ang(std::atan2(y, x));

                    if (!is_in_range(ang, start_angle, end_angle))
                    {
                        valid_circle = false;
                        break;
                    }
                }

                attempts++;
            } while (!valid_circle && attempts < 100);

            toppings.push_back({new Circle(40, pepperoni_radius, x, y), "PEPPERONI"});
            counter++;
        }
        

        counter = 0;
        while(counter < pineapple_count)
        {
            std::uniform_real_distribution<> RAD(0.1f, radius - pepperoni_radius); 
            std::uniform_real_distribution<> ANG_R(start_angle + 1.0f, end_angle - 1.0f); 
            float ang_to_convert = ANG_R(gen);
            float ang = utils::ang_to_rad(ang_to_convert);
            

            bool valid_rectangle = false;
            float x = 0.0f;
            float y = 0.0f;
            float ang_to_rotate = 0.0f;
            int attempts = 0;
            do
            {
                
                float new_radius = RAD(gen);
                x = new_radius * std::cos(ang);
                y = new_radius * std::sin(ang);

                std::uniform_real_distribution<> ANG_MOV(0.0f, 360.0f); 
                ang_to_rotate = ANG_MOV(gen);

                Rectangle temp(0.05f, 0.08f, ang_to_rotate, x, y);
                
                valid_rectangle = true;
                auto v_temp = temp.get_vertices();
                for (int i = 0; i < 4; i++)
                {
                    auto &x = v_temp[3 * i];
                    auto &y = v_temp[3 * i + 1];

                    float d = std::sqrt(x*x + y*y);
                    float ang = utils::rad_to_ang(std::atan2(y, x));
                    
                    if (d > radius || !is_in_range(ang, start_angle, end_angle))
                    {
                        valid_rectangle = false;
                        break;
                    }
                }
                attempts++;
            } while (!valid_rectangle && attempts < 100);

            toppings.push_back({new Rectangle(0.05f, 0.08f, ang_to_rotate, x, y), "PINEAPPLE"});
            counter++;
        }

        counter = 0;
        while(counter < n_condiments)
        {
            std::uniform_real_distribution<> RAD(0.1f, radius - pepperoni_radius); 
            std::uniform_real_distribution<> ANG_R(start_angle + 1, end_angle - 1); 
            float ang_to_convert = ANG_R(gen);
            float ang = utils::ang_to_rad(ang_to_convert);
            

            bool valid_rectangle = false;
            float x = 0.0f;
            float y = 0.0f;
            int attempts = 0;
            
            float ang_to_rotate = 0.0f;
            do
            {
                
                float new_radius = RAD(gen);
                x = new_radius * std::cos(ang);
                y = new_radius * std::sin(ang);

                std::uniform_real_distribution<> ANG_MOV(0.0f, 360.0f); 
                ang_to_rotate = ANG_MOV(gen);

                Rectangle temp(0.012f, 0.020f, ang_to_rotate, x, y);
                
                valid_rectangle = true;
                auto v_temp = temp.get_vertices();
                for (int i = 0; i < 4; i++)
                {
                    auto &x = v_temp[3 * i];
                    auto &y = v_temp[3 * i + 1];

                    float d = std::sqrt(x*x + y*y);
                    float ang = utils::rad_to_ang(std::atan2(y, x));
                    
                    if (d > radius || !is_in_range(ang, start_angle, end_angle))
                    {
                        valid_rectangle = false;
                        break;
                    }
                }
                attempts++;
            } while (!valid_rectangle && attempts < 100);

            toppings.push_back({new Rectangle(0.012f, 0.020f, ang_to_rotate, x, y), "OREGANO"});
            counter++;
        }
        

    }

private:
    bool is_in_range(const float& in_angle, const float &lower, const float &upper)
    {
        if (in_angle <= upper && in_angle >= lower)
            return true;

        return false;
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
        oregano_color(136, 134, 62, true)
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


        /*
        */
        for (int i = 0; i < n_slices; i++)
        {
            for (auto &t: slices[i].toppings)
            {
                auto topping_vertices = t.first->get_vertices();
                int topping_size = topping_vertices.size() / 3;
                
                if (t.second == "PEPPERONI")
                {
                    vertices.insert(vertices.end(), topping_vertices.begin(), topping_vertices.end());
                    info.push_back(DrawInfo(count, topping_size, "PEPPERONI", false, i, &pepperoni_color));
                    count += topping_size;
                }
                else if (t.second == "PINEAPPLE")
                {
                    vertices.insert(vertices.end(), topping_vertices.begin(), topping_vertices.end());
                    info.push_back(DrawInfo(count, topping_size, "PINEAPPLE", false, i, &pineapple_color));
                    count += topping_size;
                }
                else if (t.second == "OREGANO")
                {
                    vertices.insert(vertices.end(), topping_vertices.begin(), topping_vertices.end());
                    info.push_back(DrawInfo(count, topping_size, "PINEAPPLE", false, i, &oregano_color));
                    count += topping_size;
                }
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
          oregano_color;
};

#endif