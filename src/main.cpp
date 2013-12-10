//Standard C++/system headers
#include <cstdio>
#include <cstdlib>
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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp> //C++ Importer interface
#include <assimp/scene.h> //Output data structure
#include <assimp/postprocess.h> //Post processing fla

//#include "common/controls/controls.cpp"
#include "common/shaders/shaders.cpp"

using namespace glm;

GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,


        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

int main( int argc, char* argv[] )
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

    // Initialize OpenGL
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_STENCIL_TEST );
    //glEnable( GL_CULL_FACE );

    // Create Vertex Array Object
    GLuint VertexArrayID; 
    glGenVertexArrays( 1, &VertexArrayID );
    glBindVertexArray( VertexArrayID );

    //Load Shaders into a shader program
    GLuint shaderProgram = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );

    // Create Vertex Array Object and copy the vertex data to it
    GLuint VertexBufferID;
    glGenBuffers( 1, &VertexBufferID ); //Generate 1 buffer

    glBindBuffer( GL_ARRAY_BUFFER, VertexBufferID );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW ); 

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
    glEnableVertexAttribArray( posAttrib ); //Enable the vertex attrib
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0 ); 

    GLint colAttrib = glGetAttribLocation( shaderProgram, "color" );
    glEnableVertexAttribArray( colAttrib );
    glVertexAttribPointer( colAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(2*sizeof(float)));

    GLint texAttrib = glGetAttribLocation( shaderProgram, "texcoord" );
    glEnableVertexAttribArray( texAttrib );
    glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)));

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color );
    glGenerateMipmap( GL_TEXTURE_2D );

    // Load textures
    GLuint textures[2];
    glGenTextures( 2, textures );

    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
        image = SOIL_load_image("kitten.png", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
        image = SOIL_load_image("puppy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint uniColor = glGetUniformLocation( shaderProgram, "model" );    

    //Set up projection 
    mat4 viewMatrix = lookAt(
        vec3( 1.2f, 1.2f, 1.2f),
        vec3( 0.0f, 0.0f, 0.0f),
        vec3( 0.0f, 0.0f, 1.0f)
        );
    GLint uniViewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix" );
    glUniformMatrix4fv( uniViewMatrix, 1, GL_FALSE, value_ptr( viewMatrix ));

    mat4 projMatrix = perspective( 45.0f, 800.0f / 600.0f, 1.0f, 10.0f );
    GLint uniProjMatrix = glGetUniformLocation( shaderProgram, "projMatrix" );
    glUniformMatrix4fv( uniProjMatrix, 1, GL_FALSE, value_ptr( projMatrix ));

    while ( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(window) ) {              
        
        //computeMatricesFromInputs();

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Clear the screen to dark blue
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

        glUseProgram( shaderProgram );

        //Calculate transformation
        mat4 model;
        model = rotate( 
            model, 
            (float)clock() / (float)CLOCKS_PER_SEC * 180.0f, 
            vec3( 0.0f, 0.0f, 1.0f )
            );
        vec4 result = model * vec4( 1.0f, 0.0f, 0.0f, 1.0f );
        printf( "%f, %f, %f\n", result.x, result.y, result.z );

        GLint uniModel = glGetUniformLocation( shaderProgram, "model" );
        glUniformMatrix4fv( uniModel, 1, GL_FALSE, value_ptr( model ));

        //Draw floor
        glStencilFunc( GL_ALWAYS, 1, 0xFF ); //Set any stencil to 1
        glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
        //glStencilMask( 0xFF ); // Write to stencil buffer
        glDepthMask( GL_FALSE );
        //glClear( GL_STENCIL_BUFFER_BIT );

        glDrawArrays( GL_TRIANGLES, 36, 6 );

        //Draw cube reflection
        //glStencilFunc( GL_EQUAL, 1, 0xFF ); //Pass test if stencil value = 1
        //glStencilMask( 0x00 ); //Don't write anything to the stencil buffer
        glDepthMask( GL_TRUE ); // Write to depth buffer

        /*model = scale(
            translate( model, vec3( 0, 0, -1)),
            vec3( 1, 1, -1 )
            );
        glUniformMatrix4fv( uniModel, 1, GL_FALSE, value_ptr( model ));*/
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteTextures( 2, textures );

    glDeleteBuffers( 1, &VertexBufferID );

    glDeleteVertexArrays( 1, &VertexArrayID );

    glfwTerminate();
    return 0;
}