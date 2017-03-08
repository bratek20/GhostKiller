// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include <time.h>
#include "Shader.h"
#include "linmath.h"
#include "ObjectsManager.h"
#include "Assets.h"
#include "Scene.h"
#include "Player.h"
#include "TextManager.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void doAction();

GLfloat lastX = SCREEN_WIDTH / 2.0;
GLfloat lastY = SCREEN_HEIGHT / 2.0;
int keys[1024];
int mouse_pressed = 0;
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GhostKiller", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to 1 so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    loadAssets();
    initTextManager();
    initScene();

    // Game loop
    srand(time(NULL));
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        doAction();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (!isGameEndedInScene()) {
            updateScene(deltaTime);
            drawScene();
        }
        else {
            int status = getLastSceneStatus();
            
            if (status == NO_STATUS) {
                drawStartScreen();
            }
            if (status == LOST_STATUS) {
                drawLostScreen();
            }
            if (status == WON_STATUS) {
                drawWonScreen();
            }
        }
        
        glfwSwapBuffers(window);
        /*double yield = (1000.0 / 60 - (glfwGetTime() - lastFrame));
        printf("yield = %lf\n", yield);
        if (yield < 0) {
            yield = 0;
        }
        glfwWaitEventsTimeout(yield);*/
    }


    clearScene();
    clearTextManager();
    disposeAssets();

    glfwTerminate();
    
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = 1;
        else if (action == GLFW_RELEASE)
            keys[key] = 0;
    }
}

void doAction()
{
    if (isGameEndedInScene() && mouse_pressed) {
        startGameInScene();
        return;
    }

    if (mouse_pressed) {
        playerShoot(GLOBAL_PLAYER);
        mouse_pressed = 0;
    }
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        movePlayerUp(GLOBAL_PLAYER);
    }
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        movePlayerDown(GLOBAL_PLAYER);
    }
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        movePlayerLeft(GLOBAL_PLAYER);
    }    
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        movePlayerRight(GLOBAL_PLAYER);
    }
    if (keys[GLFW_KEY_SPACE]) {
        makePlayerJump(GLOBAL_PLAYER);
    }
}

int firstMouse = 1;
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (isGameBeforeEndInScene()) return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = MOUSE_SENSIVITY;	
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    rotatePlayerSight(GLOBAL_PLAYER, xoffset, yoffset);
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
        mouse_pressed = 1;
    }
}