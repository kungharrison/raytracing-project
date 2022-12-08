#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Screenshot.h"
#include "RTScene.h"
#include "Image.h"
#include "Ray.h"


static const int width = 400;
static const int height = 300;
static const char* title = "Scene viewer";
static Image image(width, height);
static const glm::vec4 background(0.1f, 0.2f, 0.3f, 1.0f);
static RTScene scene;

#include "hw3AutoScreenshots.h"

void printHelp(){
    std::cout << R"(
    Available commands:
      press 'H' to print this message again.
      press Esc to quit.
      press 'O' to save a screenshot.
      press the arrow keys to rotate camera.
      press 'A'/'Z' to zoom.
      press 'R' to reset camera.
      press 'L' to turn on/off the lighting.
        
)";
}

void initialize(void){
    printHelp();
    glClearColor(background[0], background[1], background[2], background[3]); // background color
    glViewport(0,0,width,height);
    
    // Initialize scene
    scene.init();
    scene.buildTriangleSoup();

    image.initialize();

    // Testing stuff
    // Ray ray;
    // ray.p0 = glm::vec3(0.0f, 0.0f, 0.0f);
    // ray.dir = normalize(glm::vec3(0.0f, -0.1f, 1.0f));
    // Triangle triangle;
    // glm::vec3 p1 = glm::vec3(-1.0f, -1.0f, 1.0f);
    // glm::vec3 p2 = glm::vec3(-1.0f, 1.0f, 1.0f);
    // glm::vec3 p3 = glm::vec3(1.0f, -1.0f, 1.0f);
    // glm::vec3 n1 = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 n2 = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 n3 = glm::vec3(0.0f, 0.0f, 0.0f);
    // triangle.P = std::vector<glm::vec3>({p1, p2, p3});
    // triangle.N = std::vector<glm::vec3>({n1, n2, n3});
    // Camera c = Camera();
    // c.aspect = 135.0f / 90.0f;
    // c.fovy = 90.0f;
    // c.target = glm::vec3(0.0f, 1.0f, 0.0f);
    // c.eye = glm::vec3(-5.0f, 1.0f, 0.0f);
    // c.up = glm::vec3(0.0f, 1.0f, 0.0f);
    // int i = 82;
    // int j = 22;
    // Ray r = RayTracer::RayThruPixel(c, i, j, 135, 90);
    // std::cout << r.dir[0] << " " << r.dir[1] << " " << r.dir[2] << std::endl;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    RayTracer::Raytrace(*scene.camera, scene, image);
    image.draw();
    // std::cout << scene.camera->eye.x << " " << scene.camera->eye.y << " " << scene.camera->eye.z << " " << std::endl;
    // std::cout << scene.camera->up.x << " " << scene.camera->up.y << " " << scene.camera->up.z << " " << std::endl;
    
    glutSwapBuffers();
    glFlush();
    
}

void saveScreenShot(const char* filename = "test.png"){
    int currentwidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentheight = glutGet(GLUT_WINDOW_HEIGHT);
    Screenshot imag = Screenshot(currentwidth,currentheight);
    imag.save(filename);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27: // Escape to quit
            exit(0);
            break;
        case 'h': // print help
            printHelp();
            break;
        case 'o': // save screenshot
            saveScreenShot();
            break;
        case 'r':
            scene.camera -> aspect_default = float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT));
            scene.camera -> reset();
            glutPostRedisplay();
            break;
        case 'a':
            scene.camera -> zoom(0.9f);
            glutPostRedisplay();
            break;
        case 'z':
            scene.camera -> zoom(1.1f);
            glutPostRedisplay();
            break;
        case 'l':
            scene.shader -> enablelighting = !(scene.shader -> enablelighting);
            glutPostRedisplay();
            break;
        case ' ':
            hw3AutoScreenshots();
            glutPostRedisplay();
            break;
        default:
            glutPostRedisplay();
            break;
    }
}
void specialKey(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP: // up
            scene.camera -> rotateUp(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // down
            scene.camera -> rotateUp(10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // right
            scene.camera -> rotateRight(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // left
            scene.camera -> rotateRight(10.0f);
            glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv)
{
    // BEGIN CREATE WINDOW
    glutInit(&argc, argv);
    
    glutInitContextVersion(3,1);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit() ;
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    // END CREATE WINDOW
    
    initialize();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    
    glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}
