#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <thread>

#include "utils.h"
#include "shaderList.h"
#include "animation_list.h"
#include "shape.h"
#include "matrix.h"


float bgR = 0.0f, bgG = 0.0f, bgB = 0.0f;


AnimationList animations;
std::vector<Shape*> shapes;

float offset = 0.1f;
float angle = 10.0f;
bool is_moving = true;

int current_id = 0;
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 model;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = model * vec4(aPos, 1.0);\n"
                                 "}\0";

const char *fragmentShader = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "uniform vec3 color;\n"
                             "void main()\n"
                             "{\n"
                             "   FragColor = vec4(color, 1.0f);\n"
                             "}\0";
						   								   



void traslate(float x, float y, float z)
{
    for (auto &m_i : shapes)
    {
		if (m_i->id != current_id)
			continue;
		
        m_i->model.traslate(x, y, z);
        m_i->c_x += x;
        m_i->c_y += y;
        m_i->c_z += z;
    }
}

void scale(float factor)
{
    for (auto &m_i : shapes)
	{
		if (m_i->id != current_id)
			continue;

        m_i->model.traslate(-m_i->c_x, -m_i->c_y, -m_i->c_z);
		m_i->model.scale(factor, factor, factor);
        m_i->model.traslate(m_i->c_x, m_i->c_y, m_i->c_z);
	}
}

void rotate_c_x(float angle)
{
    for (auto &m_i : shapes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->model.traslate(-m_i->c_x, -m_i->c_y, -m_i->c_z);
        m_i->model.rotate_x(angle);
        m_i->model.traslate(m_i->c_x, m_i->c_y, m_i->c_z);
    }
}

void rotate_c_y(float angle)
{
    for (auto &m_i : shapes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->model.traslate(-m_i->c_x, -m_i->c_y, -m_i->c_z);
        m_i->model.rotate_y(angle);
        m_i->model.traslate(m_i->c_x, m_i->c_y, m_i->c_z);
    }
}

void rotate_c_z(float angle)
{
    for (auto &m_i : shapes)
    {
		if (m_i->id != current_id)
			continue;
        m_i->model.traslate(-m_i->c_x, -m_i->c_y, -m_i->c_z);
        m_i->model.rotate_z(angle);
        m_i->model.traslate(m_i->c_x, m_i->c_y, m_i->c_z);
    }
}

void frame_buffer_size_call_back(GLFWwindow* in_window, int in_w, int in_h)
{
    glViewport(0, 0, in_w, in_h);
}

void key_call_back(GLFWwindow* in_window, int key, int scan_code, int action, int mods)
{
    if (key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(in_window, true);
    else if ( key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        traslate(-offset, 0.0f, 0.0f);
    }
    else if ( key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        traslate(offset, 0.0f, 0.0f);
    }
    else if ( key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        traslate(0.0f, offset, 0.0f);
    }
    else if ( key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        traslate(0.0f, -offset, 0.0f);
    }
    else if ( key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT) ) // X
		rotate_c_x(angle);
    else if ( key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT) )
		rotate_c_x(-angle);
    else if ( key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT) ) // Y
		rotate_c_y(angle);
    else if ( key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT) )
		rotate_c_y(-angle);
    else if ( key == GLFW_KEY_N && (action == GLFW_PRESS || action == GLFW_REPEAT) ) // Z
		rotate_c_z(angle);
    else if ( key == GLFW_KEY_M && (action == GLFW_PRESS || action == GLFW_REPEAT) )
		rotate_c_z(-angle);
    else if ( key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        scale(1.0f + offset);
    else if ( key == GLFW_KEY_C && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        scale(1.0f + -offset);
    else if ( key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        is_moving = !is_moving;
    else if ( key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        animations.add_animation(1, {AnimationInfo(1, -0.5, "MOVE_Y"),
                                     AnimationInfo(1, 0.3, "SCALE_Y")},
                                     60);
        animations.add_animation(1, {AnimationInfo(1, -0.3, "SCALE_Y"),
                                     AnimationInfo(1, -0.3, "MOVE_Y") },
                                     40);
        animations.add_animation(1, {AnimationInfo(1, -0.2, "SCALE_Y"),
                                     AnimationInfo(1, 0.5, "SCALE_X")},
                                     45);
        animations.add_animation(1, {AnimationInfo(1, -0.5, "SCALE_X"),
                                     AnimationInfo(1, 0.2, "SCALE_Y")},
                                     45);

        // Trompo
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y")}, 60);
        animations.add_animation(4, {AnimationInfo(2, 90, "ROTATE_C_Y"),
                                     AnimationInfo(2, -50, "ROTATE_C_Z"),
                                     AnimationInfo(2, -0.1, "MOVE_Y")}, 60);
        
    }
	
	else if ( key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        current_id --;
		if (current_id < 0)
			current_id = shapes.size() -1;
		
		std::cout << "S_size: " << shapes.size() << " << current_id: " << current_id << "\n";
    }
	
	else if ( key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        current_id ++;
		if (current_id >= shapes.size())
			current_id = 0;
		
		std::cout << "S_size: " << shapes.size() << " << current_id: " << current_id << "\n";
    }
	
}

void add_figure(Shape* in_shape, std::vector<float>& vertices, std::vector<unsigned int>& indices, int &v_count, int &i_count, const std::string& label)
{
    auto vF = in_shape->vertices;
    auto iF = in_shape->indices;
    
    int size = vF.size() / 3;
	
    
	// 

    // Faces
	for (auto &i_Info: in_shape->info_faces)
	{
		if (i_Info.uses_EBO)
			i_Info.start_indice += i_count;  // EBO: offset en el array de índices
		else
			i_Info.start_indice += v_count;
		
	}
	
	// Edges
	for (auto &i_Info: in_shape->info_edges)
	{
		if (i_Info.uses_EBO)
			i_Info.start_indice += i_count;
		else
			i_Info.start_indice += v_count;
	}
	
	// Points
	for (auto &i_Info: in_shape->info_points)
	{
		if (i_Info.uses_EBO)
			i_Info.start_indice += i_count;
		else
			i_Info.start_indice += v_count;
	}

    
    vertices.insert(vertices.end(), vF.begin(), vF.end());
    
    if (!iF.empty())
	{
		for (auto &i : iF)
			i += v_count;
		indices.insert(indices.end(), iF.begin(), iF.end());
	}
        

    v_count += size;
    i_count += iF.size();
	
	shapes.push_back(in_shape);
	
    /*
	std::cout << "COORDS OF " << label << "\n";
	
	auto s = shapes.back();
	for (int i = 0; i < s->vertices.size()/3; i++)
    std::cout << i+1 <<"\t" << s->vertices[i] << " " << s->vertices[i+1] << " " << s->vertices[i+2] << "\n";
	std::cout << "START " << s->info_faces.front().start_indice << " COUNT " << s->info_faces.front().count << "\n"; 
	std::cout << "\n\n\n\n";
	
	std::cout << "v count: " << v_count << "\n";
    */
	
}

int main()
{
    const int width = 900;
    const int height = 900;


    // Initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Animation", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_call_back);
    glfwSetKeyCallback(window, key_call_back);

    // Init glad
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // ***********************
    // SHADERSS
    // ***********************
    
    ShaderList shaders;
    shaders.create_vertex_shader(vertexShaderSource);
    shaders.add_fragment_shader("UNIQUE", fragmentShader);
    shaders.delete_shaders();


    // *************
    // Triangle
    // *************

    unsigned int VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    
    // Colors
    Color pink(255.0f, 0.0f, 255.0f, true);
    Color blue(10.0f, 15.0f, 40.0f, true);
    Color mint(10.0f, 15.0f, 40.0f, true);
    Color lava(226.0f, 45.0f, 0.0f, true);
    Color radioactive(199.0f, 255.0f, 0.0f, true);
    Color turquesa(0.0f, 128.0f, 128.0f, true);
    Color purple(157.0f, 0.0f, 255.0f, true);
    Color golden(225.0f, 190.0f, 150.0f, true);
    Color le_lime(133.0f, 235.0f, 52.0f, true);
    Color red(255.0f, 0.0f, 0.0f, true);
    Color white(255.0f, 255.0f, 255.0f, true);

    // Figuras

    std::vector<float> vertices;
    std::vector<unsigned int> indices;


    Cube floor(0.5f, 0.0f, -0.5f, -0.0f, true, true);
	floor.id = 0;
    floor.set_face_color(0, &golden);
    floor.set_face_color(1, &golden);
    floor.set_face_color(2, &golden);
    floor.set_face_color(3, &golden);
    floor.set_face_color(4, &golden);
    floor.set_face_color(5, &golden);
	
    Cube cubeA(0.2, -0.5, 0.0f, 0.0f,
                                    true, true, true);
	cubeA.id = 1;
	

	cubeA.set_face_color(2, &pink);      // Front
	
	cubeA.set_face_color(1, &lava);      // Back
	cubeA.set_face_color(2, &radioactive); // Left
	cubeA.set_face_color(3, &turquesa);  // Right
	cubeA.set_face_color(4, &purple);    // Top
	cubeA.set_face_color(5, &golden);    // Bottom
	
	cubeA.set_edge_color(0, &white);
    cubeA.set_edge_color(2, &white);
    cubeA.set_edge_color(4, &white);
    cubeA.set_edge_color(6, &white);
    cubeA.set_edge_color(8, &white);
    cubeA.set_edge_color(10, &white);
	
	Pyramid pyrA(0.2, 0.1, 0.5f, 0.5f, 0.0f, true, true, true);
	pyrA.id = 2;
	pyrA.set_face_color(0, &pink);
	pyrA.set_face_color(1, &lava);
	pyrA.set_face_color(2, &radioactive);
    pyrA.set_face_color(3, &le_lime);
    pyrA.set_face_color(4, &red);
	
	
	Sphere sphA(40, 0.1, 0.0, 0.0, 0.0, true);
	sphA.id = 3;
    sphA.set_face_color(0, &pink);
	sphA.set_face_color(1, &lava);
	sphA.set_face_color(2, &radioactive);
    sphA.set_face_color(3, &le_lime);
    sphA.set_face_color(4, &red);
    sphA.set_face_color(5, &white);
    sphA.set_face_color(6, &le_lime);
    sphA.set_face_color(7, &red);


    Cone conA(40, 0.2f, 0.1f,
              0.3f, 0.3f, 0.0f,
              true, true, true);
    conA.id = 4;
    conA.set_face_color(0, &le_lime);
	conA.set_face_color(1, &turquesa);
	
	
    
    int v_count = 0;
    int i_count = 0;
	
	
	add_figure(&floor, vertices, indices, v_count, i_count, "RECT");
	add_figure(&pyrA, vertices, indices, v_count, i_count, "PYRA");
    add_figure(&cubeA, vertices, indices, v_count, i_count, "CUBE");
	add_figure(&sphA, vertices, indices, v_count, i_count, "SPHERE");
    add_figure(&conA, vertices, indices, v_count, i_count, "CONE");


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Specify how to read
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    // Unbind - optional for security
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    


    // Bucle
	glPointSize(10.0f);
	glLineWidth(5.0f);
	
	glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window))
    {
        if (is_moving)
            animations.process_animations(shapes);

        glClearColor(bgR, bgG, bgB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);

        
        shaders.use_shader("UNIQUE");
        glBindVertexArray(VAO);


        for (auto &s : shapes)
			s->draw(shaders);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    /*
    Work flow:
    input
    rendering
    check and call
    */

    // Delete - optional
    shaders.delete_programs();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}