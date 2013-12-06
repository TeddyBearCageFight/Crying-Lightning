//Standard C++/system headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>

//OpenGL and GL related headers
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

using namespace glm;

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "in vec3 color;"
    "in vec2 texcoord;"
    "out vec3 Color;"
    "out vec2 Texcoord;"
    "void main() {"
    "   Color = color;"
    "   Texcoord = texcoord;"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec3 Color;"
    "in vec2 Texcoord;"
    "out vec4 outColor;"
    "uniform sampler2D texKitten;"
    "uniform sampler2D texPuppy;"
    "void main() {"
    "   vec4 colKitten = texture( texKitten, Texcoord );"
    "   vec4 colPuppy = texture( texPuppy, Texcoord );"
    "   outColor = mix( colKitten, colPuppy, 0.5 );"
    "}";

float vertices[] = {
//  Position      Color             Texcoords
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
};

float pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
};

int main(int argc, char* argv[])
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 800, 600, "Crying Lightning", nullptr, nullptr);   
    glfwMakeContextCurrent(window);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    GLuint vao; //Vertex Array Object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    GLuint vbo; //Vertex Buffer Object
    glGenBuffers( 1, &vbo ); //Generate 1 buffer
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW ); 
    
    //Create an array element
    GLuint ebo;
    glGenBuffers( 1, &ebo );

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW );

    // Create and compile the Vertex Shader object
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexSource, NULL );
    glCompileShader( vertexShader );

    // Create and compile the Fragment Shader object
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentSource, NULL );
    glCompileShader( fragmentShader );
    
    // Combine the shader into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glBindFragDataLocation( shaderProgram, 0, "outColor" );
    glLinkProgram( shaderProgram );
    glUseProgram( shaderProgram );

    // Retrieve a reference for the input in the vertex shader - first location will always be 0
    GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
    glEnableVertexAttribArray( posAttrib ); //Enable the vertex attrib
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0 ); 

    GLint colAttrib = glGetAttribLocation( shaderProgram, "color" );
    glEnableVertexAttribArray( colAttrib );
    glVertexAttribPointer( colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float)));

    GLint texAttrib = glGetAttribLocation( shaderProgram, "texcoord" );
    glEnableVertexAttribArray( texAttrib );
    glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color );
    glGenerateMipmap( GL_TEXTURE_2D );

    GLuint textures[2];
    glGenTextures( 2, textures );

    int width, height;
    unsigned char* image;

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    image = SOIL_load_image("kitten.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    SOIL_free_image_data( image );
    glUniform1i( glGetUniformLocation( shaderProgram, "texKitten"), 0 );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint uniColor = glGetUniformLocation( shaderProgram, "triangleColor" );

    
    while ( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(window) ) {              
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
    
        //glDrawArrays( GL_TRIANGLES, 0, 3 );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
    // Loop until the user closes the window w/ ESC

    glDeleteProgram( shaderProgram );
    glDeleteShader( fragmentShader );
    glDeleteShader( vertexShader );


    glDeleteBuffers( 1, &vbo );

    glDeleteVertexArrays( 1, &vao);

    glfwTerminate();
    return 0;
}