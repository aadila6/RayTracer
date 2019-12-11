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
constexpr int GRID_WIDTH= 800;
constexpr int GRID_HEIGHT = 800;
constexpr int WINDOW_HEIGHT = GRID_HEIGHT*PIXEL_SIZE;
constexpr int WINDOW_WIDTH= GRID_WIDTH*PIXEL_SIZE;
constexpr float SCALE= GRID_WIDTH*.5;
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
vec3f lightSource(10,10,10);
vec3f frontPoint(10,10,10);
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
    glutCreateWindow("Three Dimentional Drawing");
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
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glLineWidth(2.0f);
    //checks for OpenGL errors
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, GRID_WIDTH, 0, GRID_HEIGHT, -1, 1);
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
    Sphere dila(vec3f(0.9*GRID_WIDTH,0.9*GRID_WIDTH,0.9*GRID_WIDTH), 100);
    Sphere dile(vec3f(1*GRID_WIDTH,2*GRID_WIDTH,3*GRID_WIDTH), 80);
    Sphere dilaa(vec3f(3*GRID_WIDTH,4*GRID_WIDTH,5*GRID_WIDTH), 200);
//    drawSplitLines();
    for(int i = 0; i<GRID_HEIGHT*2;i++){
        for(int j = 0; j < GRID_WIDTH*2; j++){
            float dist_i = 0;
            vec3f orig(i,j,10);
            vec3f dir = vec3f(i,j,GRID_WIDTH).normalized();
            vec3f N = (orig - dila.center).normalized();
            vec3f nP(1,0,0);
            if (dila.ray_intersect(orig, dir, dist_i)) {
                Point r1 = Point(vec3f(i,j,1),RGB(1.0,1.0,1.0));
                vec3f N = (orig - dila.center).normalized();
                PhongModel one(r1,N,frontPoint, lightSource,1,ambient);
                draw_pix(i,j,one.phongrgb);
            }
            
            if (dile.ray_intersect(orig, dir, dist_i)) {
                    Point r2 = Point(vec3f(i,j,1),RGB(1.0,1.0,1.0));
                    PhongModel two(r2,vec3f(0,1,0),frontPoint,lightSource,.5,RGB(0.6,.8,.9));
                    draw_pix(i,j,two.phongrgb);
            }
            
            if (dilaa.ray_intersect(orig, dir, dist_i)) {
                    Point r3 = Point(vec3f(i,j,1),RGB(1.0,1.0,1.0));
                    PhongModel three(r3,vec3f(0,1,0),frontPoint,lightSource,.5,RGB(0.9,.6,.3));
                    draw_pix(i,j,three.phongrgb);
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





//void goroShading(Point p1, Point p2, Point p3,vec3f fn, float spect, char mode){
//    if(mode == 'y'){
//        p1 = swapCoordsyz(p1);
//        p2 = swapCoordsyz(p2);
//        p3 = swapCoordsyz(p3);
//    }else if(mode == 'z'){
//        p1 = swapCoordsxz(p1);
//        p2 = swapCoordsxz(p2);
//        p3 = swapCoordsxz(p3);
//    }
//    int count = 0;
//    int ymax, ymin;
//    std::vector<int> records;
//    std::vector<RGB> rgbs;
//    ymax = rdf(max(max(p1.point.y(),p2.point.y()),p3.point.y()));
//    ymin = rdf(min(min(p1.point.y(),p2.point.y()),p3.point.y()));
//    Point r1 = p1;
//    Point r2 = p2;
//    Point r3 = p3;
//    r1.point = r1.point / GRID_WIDTH;
//    r2.point = r2.point / GRID_WIDTH;
//    r3.point = r3.point / GRID_WIDTH;
//
//    PhongModel one(r1,fn,frontPoint, lightSource,spect,ambient);
//    PhongModel two(r2,fn,frontPoint, lightSource,spect,ambient);
//    PhongModel three(r3,fn,frontPoint, lightSource,spect,ambient);
//    int lol = 0;
//    for(int i=ymin; i<ymax; i++){
//        int x = 0;
//        x = ddaIntercepts(i, p1, p2);
//        RGB temp = calculateRGB(p1.point.y(),p2.point.y(),i,p1.intensity,p2.intensity);
//        if(phongSwitch){
//            temp = calculateRGB(p1.point.y(),p2.point.y(),i,one.phongrgb,two.phongrgb);
//        }
//        if(x!= -1){
//            bool found = false;
//            for(int k = 0; k<records.size();k++){
//                if(records[k] == x){
//                    found = true;
//                    break;
//                }
//            }
//            if(!found){
//                int h = 0;
//                for(int h = 0; h<records.size();h++){
//                    if(x<records[h]){
//                        break;
//                    }
//                }
//                records.insert(records.begin()+h,x);
//                rgbs.insert(rgbs.begin() + h, temp);
//            }
//        }
//
//        x = ddaIntercepts(i, p1, p3);
//        temp = calculateRGB(p1.point.y(),p3.point.y(),i,p1.intensity,p3.intensity);
//        if(phongSwitch){
//            temp = calculateRGB(p1.point.y(),p3.point.y(),i,one.phongrgb,three.phongrgb);
//        }
//        if(x!= -1){
//            bool found = false;
//            for(int k = 0; k<records.size();k++){
//                if(records[k] == x){
//                    found = true;
//                    break;
//                }
//            }
//            if(!found){
//                int h = 0;
//                for(int h = 0; h<records.size();h++){
//                    if(x<records[h]){
//                        break;
//                    }
//                }
//                records.insert(records.begin()+h,x);
//                rgbs.insert(rgbs.begin() + h, temp);
//            }
//        }
//
//
//        x = ddaIntercepts(i, p2, p3);
//        temp = calculateRGB(p2.point.y(),p3.point.y(),i,p2.intensity,p3.intensity);
//        if(phongSwitch){
//            temp = calculateRGB(p2.point.y(),p3.point.y(),i,two.phongrgb,three.phongrgb);
//        }
//        if(x!= -1){
//            bool found = false;
//            for(int k = 0; k<records.size();k++){
//                if(records[k] == x){
//                    found = true;
//                    break;
//                }
//            }
//            if(!found){
//                int h = 0;
//                for(int h = 0; h<records.size();h++){
//                    if(x<records[h]){
//                        break;
//                    }
//                }
//                records.insert(records.begin()+h,x);
//                rgbs.insert(rgbs.begin() + h, temp);
//            }
//        }
//        //开始画
//        if (records.size() == 1){
//            lol = 1;
//            if(mode == 'y'){
//                if(htSwitch){
//                    if(lol==0){
//                        Point la(vec3f(records[0], i,0)/3,rgbs[0]);
//                        drawMegaPixel(la);
//                    }else if(lol == 1){
//                        lol++;
//                    }else if(lol == 2){
//                        lol = 0;
//                    }
//                }else{
//                    draw_pix(records[0]*.5, i*.5+0.5*WINDOW_HEIGHT, rgbs[0]);
//                }
//            }else if(mode == 'z'){
//                if(htSwitch){
//                    Point la(vec3f(records[0]*.5+.5*WINDOW_WIDTH, i*.5+0.5*WINDOW_HEIGHT,0)/3,rgbs[0]);
//                    drawMegaPixel(la);
//                }else{
//                    draw_pix(records[0]*.5+.5*WINDOW_WIDTH, i*.5+0.5*WINDOW_HEIGHT, rgbs[0]);
//                }
//            }
//            else{
//                if(htSwitch){
//                    if(lol==0){
//                        Point la(vec3f(records[0]*.5, i*.5,0),rgbs[0]);
//                        drawMegaPixel(la);
//                    }else if(lol == 1){
//                        lol++;
//                    }else if(lol == 2){
//                        lol = 0;
//                    }
//                }else{
//                    draw_pix(records[0]*.5, i*.5, rgbs[0]);
//                }
//            }
//        }
//
//        else if (records.size() == 2)
//        {
//            //            glm::vec3 colorbtw = glm::vec3(0);
//            int dx;
//            int start,end;
//            dx = records[1] - records[0];
//            lol = 1;
//            if(dx<0){
//                start = records[1];
//                end = records[0];
//                count++;
////                std::cout<<"dx<0!!!!"<<std::endl;
//            }else{
//                start = records[0];
//                end = records[1];
//            }
//
//            for (int k = start; k <= end; k++)
//            {
//                //linear interpolation
//                RGB colorbtw;
//                colorbtw.r= (records[1] - k) * 1.0f / (dx * 1.0f) * rgbs[0].r + (k - records[0]) * 1.0f / (dx * 1.0f) * rgbs[1].r;
//                colorbtw.g= (records[1] - k) * 1.0f / (dx * 1.0f) * rgbs[0].g + (k - records[0]) * 1.0f / (dx * 1.0f) * rgbs[1].g;
//                colorbtw.b= (records[1] - k) * 1.0f / (dx * 1.0f) * rgbs[0].b + (k - records[0]) * 1.0f / (dx * 1.0f) * rgbs[1].b;
//                if(mode == 'y'){
//                    if(htSwitch){
//                        if(lol==0){
//                            Point la(vec3f(k*.5,i*.5+0.5*WINDOW_HEIGHT,0)/3,colorbtw);
//                            drawMegaPixel(la);
//                        }else if(lol == 1){
//                            lol++;
//                        }else if(lol == 2){
//                            lol = 0;
//                        }
//                    }else{
//                        draw_pix(k*.5, i*.5+0.5*WINDOW_HEIGHT, colorbtw);
//                    }
//
//                }else if(mode == 'z'){
//                    if(htSwitch){
//                        if(lol==0){
//                            Point la(vec3f(k*.5+.5*WINDOW_WIDTH,i*.5+0.5*WINDOW_HEIGHT,0)/3,colorbtw);
//                            drawMegaPixel(la);
//                        }else if(lol == 1){
//                            lol++;
//                        }else if(lol == 2){
//                            lol = 0;
//                        }
//                    }else{
//                        draw_pix(k*.5+.5*WINDOW_WIDTH, i*.5+0.5*WINDOW_HEIGHT, colorbtw);
//                    }
//                }else{
//                    if(htSwitch){
//                        if(lol==0){
//                            Point la(vec3f(k*.5,i*.5,0)/3,colorbtw);
//                            drawMegaPixel(la);
//                        }else if(lol == 1){
//                            lol++;
//                        }else if(lol == 2){
//                            lol = 0;
//                        }
//                    }else{
//                        draw_pix(k*.5, i*.5, colorbtw);
//                    }
//                }
//            }
//        }else{
//            int dx;
//            int start,end;
//            dx = records[2] - records[0];
//            lol = 1;
//            if(dx<0){
//                start = records[2];
//                end = records[0];
//                count++;
////                std::cout<<"dx<0!!!!"<<std::endl;
//            }else{
//                start = records[0];
//                end = records[2];
//            }
//            for (int k = start; k <= end; k++)
//            {
//                //linear interpolation
//                RGB colorbtw;
//                colorbtw.r = (records[2] - k) * 1.0f / (dx * 1.0f) * rgbs[0].r + (k - records[0]) * 1.0f / (dx * 1.0f) * rgbs[2].r;
//                colorbtw.g = (records[2] - k) * 1.0f / (dx * 1.0f) * rgbs[0].g + (k - records[0]) * 1.0f / (dx * 1.0f) * rgbs[2].g;
//                colorbtw.b = (records[2] - k) * 1.0f / (dx * 1.0f) * rgbs[0].b + (k - records[0]) * 1.0f / (dx * 1.0f) * rgbs[2].b;
////                int lol = 0;
//                if(mode == 'y'){
//                    if(htSwitch){
//                        if(lol==0){
//                            Point la(vec3f(k*.5,i*.5+0.5*WINDOW_HEIGHT,0)/3,colorbtw);
//                            drawMegaPixel(la);
//                        }else if(lol == 1){
//                            lol++;
//                        }else if(lol == 2){
//                            lol = 0;
//                        }
//                    }else{
//                        draw_pix(k*.5, i*.5+0.5*WINDOW_HEIGHT, colorbtw);
//                    }
//
//                }else if(mode == 'z'){
//                    if(htSwitch){
//                        if(lol==0){
//                            Point la(vec3f(k*.5+.5*WINDOW_WIDTH,i*.5+0.5*WINDOW_HEIGHT,0)/3,colorbtw);
//                            drawMegaPixel(la);
//                        }else if(lol == 1){
//                            lol++;
//                        }else if(lol == 2){
//                            lol = 0;
//                        }
//                    }else{
//                        draw_pix(k*.5+.5*WINDOW_WIDTH, i*.5+0.5*WINDOW_HEIGHT, colorbtw);
//                    }
//                }else{
//                    if(htSwitch){
//                        if(lol==0){
//                            Point la(vec3f(k*.5,i*.5,0)/3,colorbtw);
//                            drawMegaPixel(la);
//                        }else if(lol == 1){
//                            lol++;
//                        }else if(lol == 2){
//                            lol = 0;
//                        }
//                    }else{
//                        draw_pix(k*.5, i*.5, colorbtw);
//                    }
//                }
//            }
//        }
//        records.clear();
//        rgbs.clear();
//        lol = 1;
//    }
////    std::cout<<count<<std::endl;
//}







