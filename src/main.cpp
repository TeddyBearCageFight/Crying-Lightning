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
#include <assimp/postprocess.h> //Post processing fla

//#include "common/controls/controls.cpp"
#include "common/shaders/shaders.cpp"

using namespace glm;

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
    glEnable( GL_STENCIL_TEST );
    //glEnable( GL_CULL_FACE );

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -1.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 
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
         15.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    // Create Vertex Array Object
    GLuint VertexArrayID; 
    glGenVertexArrays( 1, &VertexArrayID );
    glBindVertexArray( VertexArrayID );

    //Load Shaders into a shader program
    GLuint shaderProgram = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );
    glUseProgram(shaderProgram);
 
    /*----------------------------------
     ___ _   _ ___ ___ ___ ___  ___ 
    | _ ) | | | __| __| __| _ \/ __|
    | _ \ |_| | _|| _|| _||   /\__ \
    |___/\___/|_| |_| |___|_|_\|___/
                                 
    ------------------------------------*/

    // Create Vertex Array Object and copy the vertex data to it
    GLuint VertexBufferID;
    glGenBuffers( 1, &VertexBufferID ); //Generate 1 buffer

    glBindBuffer( GL_ARRAY_BUFFER, VertexBufferID );
    glBufferData( GL_ARRAY_BUFFER,	 sizeof(vertices), vertices, GL_STATIC_DRAW ); 

    GLuint frameBuffer;
    glGenFramebuffers( 1, &frameBuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );


    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
    glEnableVertexAttribArray( posAttrib ); //Enable the vertex attrib
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0 ); 

    GLint colAttrib = glGetAttribLocation( shaderProgram, "color" );
    glEnableVertexAttribArray( colAttrib );
    glVertexAttribPointer( colAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));

    GLint texAttrib = glGetAttribLocation( shaderProgram, "texcoord" );
    glEnableVertexAttribArray( texAttrib );
    glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
        image = SOIL_load_image("puppy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLuint texColorBuffer;
    glGenTextures( 1, &texColorBuffer );
    glBindTexture( GL_TEXTURE_2D, texColorBuffer );

    //Attach image to the framebuffer
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
        );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
        );

    //Renderbuffer object image
    GLuint rboDepthStencil;
    glGenRenderbuffers( 1, &rboDepthStencil );
    glBindRenderbuffer( GL_RENDERBUFFER, rboDepthStencil );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600 );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
        );

    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    
    GLuint uniModel = glGetUniformLocation( shaderProgram, "model" );    

    //Set up projection 
    mat4 view = lookAt(
        vec3( 2.5f, 2.5f, 2.5f),
        vec3( 0.0f, 0.0f, 0.0f),
        vec3( 0.0f, 0.0f, 1.0f)
        );
    GLint uniView = glGetUniformLocation( shaderProgram, "view" );
    glUniformMatrix4fv( uniView, 1, GL_FALSE, value_ptr( view ));

    mat4 proj = perspective( 45.0f, 800.0f / 600.0f, 1.0f, 10.0f );
    GLint uniProj = glGetUniformLocation( shaderProgram, "proj" );
    glUniformMatrix4fv( uniProj, 1, GL_FALSE, value_ptr( proj ));

    GLint uniColor = glGetUniformLocation( shaderProgram, "overrideColor" );

    /*------------------------
     ___  ___    ___      __
    |   \| _ \  /_\ \    / /
    | |) |   / / _ \ \/\/ / 
    |___/|_|_\/_/ \_\_/\_/  

    ---------------------------*/
                         
    while ( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(window) ) {              
        
        //computeMatricesFromInputs();

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

        //Calculate transformation
        mat4 model;
        model = rotate( 
            model, 
            (float)clock() / (float)CLOCKS_PER_SEC * 180.0f, 
            vec3( 0.0f, 0.0f, 1.0f )
            );
        /*vec4 result = model * vec4( 1.0f, 0.0f, 0.0f, 1.0f );
        printf( "%f, %f, %f\n", result.x, result.y, result.z );*/

        glUniformMatrix4fv( uniModel, 1, GL_FALSE, value_ptr( model ));

        glDrawArrays(GL_TRIANGLES, 0, 36 );

        glEnable( GL_STENCIL_TEST );

        // Draw floor
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);
        glDepthMask(GL_FALSE);
        glClear(GL_STENCIL_BUFFER_BIT);
                
        glDrawArrays(GL_TRIANGLES, 36, 6);

        // Draw cube reflection
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDepthMask(GL_TRUE);

        model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

        glDisable( GL_STENCIL_TEST );

        // Swap front and back buffers */
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteTextures( 2, textures );

    glDeleteBuffers( 1, &VertexBufferID );
    glDeleteFramebuffers( 1, &frameBuffer );
    glDeleteRenderbuffers( 1, &rboDepthStencil );
    glDeleteVertexArrays( 1, &VertexArrayID );

    glfwTerminate();
    return 0;
}
