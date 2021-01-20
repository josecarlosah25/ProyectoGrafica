// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void animacionAccidente();

// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Variables para animaciones
float movCajon = 0; bool cajonActive = false;
float movRetrato = 0; bool retratoActive = false;
float movPuerta = 0; bool puertaActive = false;

float rotAccidenteB, rotAccidenteJ, movAccidente = 0; bool accidenteP1 = false; bool accidenteP2 = false; bool accidenteP3 = false;

int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 9", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    
    // Load models
    Model casa( (char *)"Models/casa/casa.obj");
    Model cajon((char*)"Models/buro/cajon.obj"); 
    Model telefono((char*)"Models/telefono/telefono.obj");
    Model retrato((char*)"Models/retrato/retrato.obj");
    Model silla((char*)"Models/silla/silla.obj");
    Model puertaPrin((char*)"Models/door/puerta.obj");
    Model banquito((char*)"Models/banco/banco.obj");
    Model jarron((char*)"Models/jarron/jarron.obj");
   

    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        animacionAccidente();
        
        // Clear the colorbuffer
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );

        float tras = -1.75f;
        
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");

        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::translate( model, glm::vec3( 0.0f, tras, 0.0f ) ); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
	
		casa.Draw( shader );

        

        
        // Draw the loaded model
        model= glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, tras, 0.0f-movCajon)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        cajon.Draw(shader);

        // Draw the loaded model
        model = glm::mat4(1);
        
        model = glm::translate(model, glm::vec3(0.75945f, 4.5712f+tras, -1.4055f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        telefono.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.417576f, 4.52347f + tras, -1.28148f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(movRetrato), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        retrato.Draw(shader);


        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.35797f, 4.31681f + tras, -1.82913f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        silla.Draw(shader);


        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.67457f, 1.05998f + tras, -0.914868f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(movPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        puertaPrin.Draw(shader);


        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.319778f-movAccidente, 3.90331f + tras, -4.38809f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(rotAccidenteJ), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        jarron.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.319778f, 3.90331f + tras, -4.38809f)); // Translate it down a bit so it's at the center of the scene
        //model = glm::scale( model, glm::vec3( 0.02f, 0.02f, 0.02f ) );	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(rotAccidenteB), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        banquito.Draw(shader);


        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

void reset() {
    cajonActive = false;
    movCajon = 0;
    retratoActive = false;
    movRetrato = 0;
    puertaActive = false;
    movPuerta = 0;

}

void animacionAccidente() {
    float vel=3;
    if (accidenteP1) {
        rotAccidenteB += vel;
        rotAccidenteJ += vel;
        if (rotAccidenteB >= 45) {
            accidenteP1 = false;
            accidenteP2 = true;
        }
    }
    if (accidenteP2) {
        rotAccidenteB += vel;
        rotAccidenteJ += vel;
        movAccidente += vel / 100;
        if (rotAccidenteB >= 90) {
            accidenteP2 = false;
            accidenteP3 = true;
        }
    }
    if (accidenteP3) {
        rotAccidenteJ += vel;
        if (rotAccidenteJ >= 95.5) {
            accidenteP3 = false;
        }
    }
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

    if (keys[GLFW_KEY_R]) {
        reset();
    }

    if (keys[GLFW_KEY_C]) {
        cajonActive = true;
        if (movCajon >= 0.3) {
            movCajon = 0;
        }
    }

    if (cajonActive) {
        if (movCajon < 0.3) {
            movCajon += 0.005;
        }
        else {
            cajonActive = false;
        }
    }

    if (keys[GLFW_KEY_G]) {
        retratoActive = true;
        if (movRetrato <= -110) {
            movRetrato = 0;
        }
    }

    if (retratoActive) {
        if (movRetrato > -110) {
            movRetrato -= 5;
        }
        else {
            retratoActive = false;
        }
    }

    if (keys[GLFW_KEY_V]) {
        puertaActive = true;
        if (movPuerta <= -90) {
            movPuerta = 0;
        }
    }

    if (puertaActive) {
        if (movPuerta > -90) {
            movPuerta -= 5;
        }
        else {
            puertaActive = false;
        }
    }

    if (keys[GLFW_KEY_F]) {
        rotAccidenteB = 0;
        rotAccidenteJ = 0;
        movAccidente = 0;
        accidenteP1 = true;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

