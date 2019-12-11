#define GL_SILENCE_DEPRECATION
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "tk.h"
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <algorithm>
//other files
#include "Geometry.h"
#include "vector.h"
#include "matrix.h"
using namespace std;
char *inputFileName;
constexpr int PIXEL_SIZE = 1;
constexpr int GRID_WIDTH= 600;
constexpr int GRID_HEIGHT = 600;
constexpr int WINDOW_HEIGHT = GRID_HEIGHT*PIXEL_SIZE;
constexpr int WINDOW_WIDTH= GRID_WIDTH*PIXEL_SIZE;
//constexpr float SCALE= GRID_WIDTH*.5;
bool phongSwitch;
bool htSwitch;
void init();
void idle();
void display();
void draw_pix(int x, int y, RGB color);
void draw();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
//void readinput(char *filename, std::vector<Polygon> &polygons);
//void writeFile(char *filename, std::vector<Polygon> &polygons);
bool* loadBuffer;

void drawLineDDA(std::vector<float> start, std::vector<float> end);
RGB calculateRGB(float y1, float y2, float y3, RGB color1, RGB color2);
void drawMegaPixel(Point point);
void goShading(Point a, Point b, Point c, char mode, vec3f fn, float spe);
Point swapCoordsyz(Point &v1);
Point swapCoordsxz(Point &v1);
void goroShading(Point p1, Point p2, Point p3,vec3f fn, float spect, char mode);
int ddaIntercepts(int yvalue, Point start, Point end);
void setGlobalValues();
void initGlobalValues();
char fileG;
vec3f lightSource(0, -10, 0);
vec3f frontPoint(0, -10, 0);
//vec3f lightSource(0,100,100);
//vec3f frontPoint(-10,10,10);
RGB ambient(1,1,1);

struct Light {
    Light(const vec3f &p, const float i) : position(p), intensity(i) {}
    vec3f position;
    float intensity;
};

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //windown title is "glut demo"
    glutCreateWindow("Ray Tracing");
    /*defined glut callback functions*/
    glutDisplayFunc(display); //rendering calls here
    glutReshapeFunc(reshape); //update GL on window size change
    glutMouseFunc(mouse);     //mouse button events
    glutMotionFunc(motion);   //mouse movement events
    glutKeyboardFunc(key);    //Keyboard events
    glutIdleFunc(idle);       //Function called while program is sitting "idle"
    //initialize opengl variables
    init();
    //start glut event loop
    glutMainLoop();
    return 0;
}

void init()
{
    //set clear color (Default background to white)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glLineWidth(2.0f);
    //checks for OpenGL errors
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, GRID_WIDTH, 0, GRID_HEIGHT, -10, 10);
    check();
}

//called repeatedly when glut isn't doing anything else
void idle(){
    glutPostRedisplay();
    
}
void drawSplitLines(){
    glColor3f(0.41, 0.4, 0.4);
    glBegin(GL_LINES);
    glVertex2f(0.5f*GRID_WIDTH,0.0f);
    glVertex2f(0.5f*GRID_WIDTH, 1.0f*GRID_HEIGHT);
    glVertex2f(0.0f*GRID_WIDTH, 0.5f*GRID_HEIGHT);
    glVertex2f(1.0f*GRID_WIDTH, 0.5f*GRID_HEIGHT);
    glEnd();
}

void displayLight(){
    draw_pix(lightSource.x()*.5, lightSource.y()*.5, RGB(0,1,0));
    draw_pix((lightSource.x()+1)*.5, (lightSource.y()+1)*.5, RGB(0,1,0));
    draw_pix(lightSource.y()*.5, lightSource.z()*0.5+.5*WINDOW_HEIGHT, RGB(1,1,1));
    draw_pix((lightSource.x()+1)*.5, lightSource.z()*+.5*0.5*WINDOW_HEIGHT, RGB(1,1,1));
    draw_pix(lightSource.x()*.5+.5*WINDOW_WIDTH, lightSource.z()*0.5+.5*WINDOW_HEIGHT, RGB(1,1,1));
    draw_pix((lightSource.x()+1)*.5+.5*WINDOW_WIDTH, lightSource.z()*0.5+.5*WINDOW_HEIGHT, RGB(1,1,1));
    
}


void printit(vec3f lala){
    std::cout<<"X Y Z:  "<< lala.x() <<" " << lala.y()<< " " <<lala.z()<<std::endl;
}

void display()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    std::vector<Sphere> spheres;
    Sphere dila(vec3f(300.0f, 300.0f, -100.0f), 100.0f); //vec3f(0.9*GRID_WIDTH,0.9*GRID_WIDTH,0.9*GRID_WIDTH), 100);
    Sphere dile(vec3f(500.0f,500.0f,-100.0f), 50.f);
    Sphere dilaa(vec3f(500.0f,200.0f,-100.0f), 120.f);
    
    //    drawSplitLines();
    for(int i = 0; i < GRID_HEIGHT; i++){
        for(int j = 0; j < GRID_WIDTH; j++){
            float dist_i = 0;
            vec3f orig(j, i, 10);
            //vec3f dir = vec3f(i,j,GRID_WIDTH).normalized();
            vec3f dir(0.0f, 0.0f, -1.0f);
            vec3f N = (orig - dila.center).normalized();
            if (dila.ray_intersect(orig, dir, dist_i)) {
                vec3f hit = orig + dist_i*dir;
                Point r1 = Point(hit, RGB(1.0,1.0,1.0));
                vec3f N = (2.0f*(hit - dila.center)).normalized();
                float d = N.dot(-1.0f*(lightSource - hit).normalized());
                RGB color(d, d, d);
                draw_pix(i, j, color);
            }
            
            dist_i = 0;
            orig = vec3f(j, i, 10);
            dir = vec3f(0.0f, 0.0f, -1.0f);
            if (dile.ray_intersect(orig, dir, dist_i)) {
                vec3f hit = orig + dist_i*dir;
                Point r1 = Point(hit, RGB(1.0,1.0,1.0));
                vec3f N = (2.0f*(hit - dile.center)).normalized();
                float d = N.dot(-1.0f*(lightSource - hit).normalized());
                RGB color(d*1.0, d*0.5, d*0.5);
                draw_pix(i, j, color);
            }
            
            dist_i = 0;
            orig = vec3f(j, i, 10);
            dir = vec3f(0.0f, 0.0f, -1.0f);
            if (dilaa.ray_intersect(orig, dir, dist_i)) {
                vec3f hit = orig + dist_i*dir;
                Point r1 = Point(hit, RGB(1.0,1.0,1.0));
                vec3f N = (2.0f*(hit - dilaa.center)).normalized();
                float d = N.dot(-1.0f*(lightSource - hit).normalized());
                RGB color(d*0.3, d*0.4, d*0.9);
                draw_pix(i, j, color);
            }
        }
    }
    glutSwapBuffers();
    check();
}

void draw_pix(int x, int y, RGB color)
{
    glBegin(GL_POINTS);
    glColor3f(color.r, color.g, color.b);
    glVertex3f(x + .5, y + .5, 0);
    glEnd();
}

//will sort all vec2fs and return in order of v1,v2,v3
void sortYlocation(Point &a, Point &b,Point &c){
    if(a.point.y()<b.point.y()){
        Point tmp(a.point,a.intensity);
        a = b;
        b = tmp;
    }
    if(b.point.y()<c.point.y()){
        Point tmp(b.point,b.intensity);
        b=c;
        c = tmp;
    }
    if(a.point.y()<c.point.y()){
        Point tmp(a.point,a.intensity);
        a=c;
        c=tmp;
    }
    if(a.point.y()<b.point.y()){
        Point tmp(a.point,a.intensity);
        a = b;
        b = tmp;
    }
    if(b.point.y()<c.point.y()){
        Point tmp(b.point,b.intensity);
        b=c;
        c = tmp;
    }
    if(a.point.y()<c.point.y()){
        Point tmp(a.point,a.intensity);
        a=c;
        c=tmp;
    }
}
Point swapCoordsyz(Point &v1){
    return(Point(vec3f(v1.point.y(),v1.point.z(),v1.point.x()), v1.intensity));
}
Point swapCoordsxz(Point &v1){
    return(Point(vec3f(v1.point.x(),v1.point.z(),v1.point.y()), v1.intensity));
}

int rdf(float x){
    return (int)(x+0.5);
}

/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
    /*set up projection matrix to define the view port*/
    //update the ne window width and height
    
    //creates a rendering area across the window
    glViewport(0, 0, width, height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, GRID_WIDTH, 0, GRID_HEIGHT, -10, 10);
    
    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    //PIXEL_SIZE = width / (float)GRID_WIDTH;
    
    //set pixel size relative to the grid cell size
    glPointSize(PIXEL_SIZE);
    //check for opengl errors
    check();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
    switch (ch)
    {
        case 'p':
            phongSwitch = !phongSwitch;
            
        default:
            
            printf("User hit the \"%c\" key\n", ch);
            break;
    }
    
    glutPostRedisplay();
}

//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf("MOUSE AT PIXEL: %d %d, GRID: %d %d\n", x, y, (int)(x / PIXEL_SIZE), (int)((WINDOW_HEIGHT - y) / PIXEL_SIZE));
    switch (button)
    {
        case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
        case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
        default:
            printf("UNKNOWN "); //any other mouse button
            break;
    }
    if (state != GLUT_DOWN) //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n"); //button clicked
    
    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)

{
    //redraw the scene after mouse movement
    glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("GLERROR: There was an error %s\n", "error");
        exit(1);
    }
}

RGB calculateRGB(float y1, float y2, float y3, RGB color1, RGB color2)
{
    float a = (y3 - y2)/(y1 - y2);
    float b = (y1 - y3)/(y1 - y2);
    float c0,c1,c2;
    c0 = a*color1.r + b*color2.r;
    c1 = a*color1.g + b*color2.g;
    c2 = a*color1.b + b*color2.b;
    RGB temp(c0,c1,c2);
    return temp;
}











