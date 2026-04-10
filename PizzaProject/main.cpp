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

#include "shaderList.h"
#include "pizza.h"
#include "animation_list.h"

#define PI 3.14159265359

float bgR = 0.0f, bgG = 0.0f, bgB = 0.0f;
bool is_moving = true;
int slice_id = 0;
unsigned int n_slices = 0;
AnimationList animations;


float offset = 0.1f;

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
						   								   



void frame_buffer_size_call_back(GLFWwindow* in_window, int in_w, int in_h)
{
    glViewport(0, 0, in_w, in_h);
}

void key_call_back(GLFWwindow* in_window, int key, int scan_code, int action, int mods)
{
    Pizza* pizzaPtr = static_cast<Pizza*>(glfwGetWindowUserPointer(in_window));

    if (key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "CHAU WEIS\n";
        glfwSetWindowShouldClose(in_window, true);
    }
    else if ( key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        bgR = 0.2f; bgG = 0.3f; bgB = 0.3f;
    }
    else if ( key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        bgR = 1.0f; bgG = 0.0f; bgB = 0.0f;
    }
    else if ( key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        bgR = 0.0f; bgG = 1.0f; bgB = 0.0f;
    }
    else if ( key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        bgR = 0.0f; bgG = 0.0f; bgB = 1.0f;
    }
    else if ( key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        slice_id  = (slice_id + 1) % n_slices;
        std::cout << "Moving slice " << slice_id << "\n";
    }
    else if ( key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        slice_id --;
        if (slice_id < 0)
            slice_id = n_slices - 1;
        std::cout << "Moving slice " << slice_id << "\n";
    }
    else if ( key == GLFW_KEY_P && action == GLFW_PRESS)
        is_moving = !is_moving;
    else if ( key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->move_slice(slice_id, -offset, 0.0f);
    else if ( key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->move_slice(slice_id, +offset, 0.0f);
    else if ( key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->move_slice(slice_id, 0.0f, +offset);
    else if ( key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->move_slice(slice_id, 0.0f, -offset);
    else if ( key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->rotate_slice_x(slice_id, 10.0f);
    else if ( key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->rotate_slice_x(slice_id, -10.0f);
    else if ( key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->rotate_slice_x(slice_id, 10.0f);
    else if ( key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->rotate_slice_x(slice_id, -10.0f);
    else if ( key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->rotate_slice_y(slice_id, 10.0f);
    else if ( key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT) )
        pizzaPtr->rotate_slice_y(slice_id, -10.0f);
    else if ( key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT) )
    {
        animations.add_animation(slice_id, {AnimationInfo(slice_id, 0.5, "MOVE_X"),
                                            AnimationInfo(slice_id, 60, "ROTATE_Z"),
                                            AnimationInfo(slice_id, 30, "ROTATE_X"),
                                            AnimationInfo(slice_id, 30, "ROTATE_Y")},
                                            120);
    }
    
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

    GLFWwindow* window = glfwCreateWindow(width, height, "Pizza", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create a windows\n";
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window); // 2
    

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

    unsigned int VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    std::cout << "ENTER NUMBER OF SLICES: ";
    std::cin >> n_slices;

    Pizza my_pizza(n_slices, VBO);
    my_pizza.create_vertices();

    std::cout << "Moving slice " << slice_id << "\n";
    glfwSetWindowUserPointer(window, &my_pizza);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * my_pizza.vertices.size(), my_pizza.vertices.data(), GL_DYNAMIC_DRAW);


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


    while(!glfwWindowShouldClose(window))
    {
        if (is_moving)
            animations.process_animations(my_pizza.info);

        glClearColor(bgR, bgG, bgB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shaders.use_shader("UNIQUE");
        shaders.set_vec2("UNIQUE", "offset", 0.0f, 0.0f);
        shaders.set_float("UNIQUE", "scale", 1.0f);

        glBindVertexArray(VAO);
        my_pizza.render(shaders);
        
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