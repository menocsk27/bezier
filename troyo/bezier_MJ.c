#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "malloc.h"
#include <time.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


typedef struct {
  double r;
  double g;
  double b;
} COLOR;

COLOR **buffer;

typedef struct {
  int x;
  int y;
}point;


static int resx = 800;
static int resy = 748;

static int currPoint[2] = {75,413};
static int nextPoint[2] = {0,0};

static int points[2][2] = {{0,1},{1,0}}; 

static point *pointsArray;
static int sizePointArray = 0;


void plot (int x, int y){
    COLOR color;

    glBegin (GL_POINTS);
    glVertex2i (x,y);
    glEnd();
    glFlush();
}

void addToPointsArray(point *array, int x, int y){

    array[sizePointArray].x= x;
    array[sizePointArray].y= y;
    
    sizePointArray++;
}


void deletePointsArray (point *array){

    for (int i = sizePointArray; i>0; i--){
        array[i-1].x=0;
        array[i-1].y=0;
    }
    sizePointArray=0;
}

//Trazo de la línea entre dos puntos.
void bresenham (int x0, int y0, int x1, int y1, void (*plot)(int,int)){
    int d2x,d2y,dx,dy,d,
        Delta_N,Delta_NE,Delta_E,Delta_SE,
        Delta_S,Delta_SW,Delta_W,Delta_NW,
        xp,yp;

    xp = x0; yp = y0;
    (*plot)(xp,yp);

    d2y = 2*(y1-y0);
    d2x = 2*(x1-x0);
    dy  = (y1-y0);
    dx  = (x1-x0);

    if (y1 > y0) { //1,2,3 o 4
        if (x1 > x0) { //1 o 2
            Delta_NE =  d2y-d2x;
            if ( dy <= dx ){ //Octante 1

                Delta_E  =  d2y;
                d = d2y-dx;

                while (xp < x1){ //Avanza en x
                    xp++;
                    if (d<0) {
                        d += Delta_E;
                    }
                    else {
                        yp++;
                        d += Delta_NE;
                    }
                    (*plot)(xp,yp);
                }
            }
            else{ //Octante 2

                Delta_N  = -d2x;
                d = dy -d2x;

                while (yp < y1){ //Avanza en y
                    yp++;
                    if (d<=0) {
                        xp++;
                        d += Delta_NE;
                    }
                    else {
                        d += Delta_N;
                    }
                    (*plot)(xp,yp);
                }
            }
        }
        else { //3 u 4
            Delta_NW = -d2y-d2x;
            if (dx >= -dy){ //Octante 3

                Delta_N  = -d2x;
                d = -dy -d2x;

                while (yp < y1){ //Avanza en y
                    yp++;
                    if (d<=0) {
                        d += Delta_N;
                    }
                    else {
                        xp--;
                        d += Delta_NW;
                    }
                    (*plot)(xp,yp);
                }
            }
            else{ //Octante 4
                Delta_W  = -d2y;
                d = -d2y-dx ;

                while (xp > x1){ //Retrocede en x
                    xp--;
                    if (d<=0) {
                        yp++;
                        d += Delta_NW;
                    }
                    else {
                        d += Delta_W;
                    }
                    (*plot)(xp,yp);
                }
            }
        }
    }
    else { //5,6,7 u 8
        if (x1 < x0) { //5 o 6
            Delta_SW = -d2y+d2x;
            if (dx <= dy){ //Octante 5

                Delta_W  = -d2y   ;
                d = -d2y+dx ;

                while (xp > x1){ //Retrocede en x
                    xp--;
                    if (d<=0) {
                        d += Delta_W;
                    }
                    else {
                        yp--;
                        d += Delta_SW;
                    }
                    (*plot)(xp,yp);

                }
            }
            else{ //Octante 6

                Delta_S  =  d2x;
                d = -dy +d2x;

                while (yp > y1){ //Retrocede en y
                    yp--;
                    if (d<0) {
                        xp--;
                        d += Delta_SW;
                    }
                    else {
                        d += Delta_S;
                    }
                    (*plot)(xp,yp);

                }
            }
        }
        else { //7 u 8
            Delta_SE =  d2y+d2x;
            if (dx <= -dy){ //Octante 7

                Delta_S  =  d2x   ;
                d =  dy +d2x;

                while (yp > y1){ //Retrocede en y
                    yp--;
                    if (d<0) {
                        d += Delta_S;
                    }
                    else {
                        xp++;
                        d += Delta_SE;
                    }
                    (*plot)(xp,yp);

                }
            }
            else{ //Octante 8

                Delta_E  =  d2y;
                d =  d2y+dx;

                while (xp < x1){ //Avanza en x
                    xp++;
                    if (d<0) {
                        yp--;
                        d += Delta_SE;
                    }
                    else {
                        d += Delta_E;
                    }
                    (*plot)(xp,yp);

                }
            }
        }
    }
}

//Actualiza el arreglo global dinámico que almacenará las coordenadas universales actuales. AL leerlas del archivo establece todo con el mapa completo. 
void paramEc (point *array, double t) {
    
    int x0p, y0p, x1p, y1p, x2p, y2p, x3p, y3p;
    double exp3, exp2, exp1, exp0;

    exp0 = pow((1-t),3);
    exp1 = 3*pow((1-t),2)*t;
    exp2 = 3*(1-t)*pow(t,2);
    exp3 = pow(t,3);

    x0p = exp0 * array[0].x; y0p = exp0 * array[0].y;    
    x1p = exp1 * array[1].x; y1p = exp1 * array[1].y;
    x2p = exp2 * array[2].x; y2p = exp2 * array[2].y;
    x3p = exp3 * array[3].x; y3p = exp3 * array[3].y;

    nextPoint[0] = x0p + x1p + x2p + x3p;   
    nextPoint[1] = y0p + y1p + y2p + y3p;

    //printf("(%i,%i)\n", nextPoint[0], nextPoint[1]);
    bresenham(currPoint[0],currPoint[1],nextPoint[0],nextPoint[1],plot);
    
    currPoint[0] = nextPoint[0];
    currPoint[1] = nextPoint[1];
}

void bezier (point *array, int precision) {
    int i;
    double t;
    for (i=0;i<=precision;i++){
        t = ((double)i)/precision;
        paramEc(array,t);
    }
}

void drawStraightLines(point *a){

    for (int i=0; i<sizePointArray;i++){
        if (i!=sizePointArray-1){
            bresenham(a[i].x,a[i].y,a[i+1].x,a[i+1].y,plot);
        }
        
    }
}



void display (void){

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char *argv[]){
    buffer = (COLOR **)malloc(resx * sizeof(COLOR*));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(resx,resy);
    glutCreateWindow("Firma de Michael Jackson");
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(-0.5, resx +0.5, -0.5, resx + 0.5);


    point a[4];

    //Comienza primer arco de la m
    glColor3f(1,0,0);
    addToPointsArray(a, 75, 413);
    addToPointsArray(a, 169, 590);
    addToPointsArray(a, 183, 550);
    addToPointsArray(a, 132, 435);
    glColor3f(0,0,0);
    bezier(a,3000);
    deletePointsArray(a);

    //Comienza primera mitad del segundo arco de la m
    glColor3f(1,0,0);
    addToPointsArray(a, 132, 435);
    addToPointsArray(a, 240, 713);
    addToPointsArray(a, 317, 748);
    addToPointsArray(a, 235, 588);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Comienza segunda mitad del segundo arco de la m
    glColor3f(1,0,0);
    addToPointsArray(a, 235, 588);
    addToPointsArray(a, 205, 467);
    addToPointsArray(a, 146, 371);
    addToPointsArray(a, 145, 259);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //COmienza la i
    currPoint[0]=145;
    currPoint[1]=461;
    glColor3f(1,0,0);
    addToPointsArray(a, 145, 461);
    addToPointsArray(a, 153, 405);
    addToPointsArray(a, 219, 409);
    addToPointsArray(a, 247, 468);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la i y comienza la c
    glColor3f(1,0,0);
    addToPointsArray(a, 247, 468);
    addToPointsArray(a, 204, 406);
    addToPointsArray(a, 270, 426);
    addToPointsArray(a, 270, 449);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Sigue la c
    glColor3f(1,0,0);
    addToPointsArray(a, 270, 449);
    addToPointsArray(a, 294, 482);
    addToPointsArray(a, 348, 478);
    addToPointsArray(a, 315, 452);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Sigue la c
    glColor3f(1,0,0);
    addToPointsArray(a, 315, 452);
    addToPointsArray(a, 348, 478);
    addToPointsArray(a, 294, 482);
    addToPointsArray(a, 270, 449);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la c y comienza la h
    glColor3f(1,0,0);
    addToPointsArray(a, 270, 449);
    addToPointsArray(a, 239, 400);
    addToPointsArray(a, 323, 419);
    addToPointsArray(a, 332, 438);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Sigue la h
    glColor3f(1,0,0);
    addToPointsArray(a, 332, 438);
    addToPointsArray(a, 535, 642);
    addToPointsArray(a, 418, 611);
    addToPointsArray(a, 324, 372);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Sigue la H 
    glColor3f(1,0,0);
    addToPointsArray(a, 324, 372);
    addToPointsArray(a, 343, 461);
    addToPointsArray(a, 403, 473);
    addToPointsArray(a, 415, 471);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la h y comienza la a
    glColor3f(1,0,0);
    addToPointsArray(a, 415, 471);
    addToPointsArray(a, 329, 385);
    addToPointsArray(a, 443, 414);
    addToPointsArray(a, 466, 462);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la a y comienza la e
    glColor3f(1,0,0);
    addToPointsArray(a, 466, 462);
    addToPointsArray(a, 393, 397);
    addToPointsArray(a, 475, 402);
    addToPointsArray(a, 489, 434);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la e y comienza la l
    glColor3f(1,0,0);
    addToPointsArray(a, 489, 434);
    addToPointsArray(a, 486, 368);
    addToPointsArray(a, 570, 424);
    addToPointsArray(a, 602, 547);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la l
    glColor3f(1,0,0);
    addToPointsArray(a, 602, 547);
    addToPointsArray(a, 516, 363);
    addToPointsArray(a, 585, 405);
    addToPointsArray(a, 640, 431);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue la l alargada
    glColor3f(1,0,0);
    addToPointsArray(a, 640, 431);
    addToPointsArray(a, 800, 630);
    addToPointsArray(a, 596, 790);
    addToPointsArray(a, 350, 685);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue la l alargada
    glColor3f(1,0,0);
    addToPointsArray(a, 350, 685);
    addToPointsArray(a, 171, 647);
    addToPointsArray(a, 55, 335);
    addToPointsArray(a, 257, 304);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //FInaliza Michael
    //Comienza JAckson

    //Comienza la J
    currPoint[0]=330;
    currPoint[1]=292;
    glColor3f(1,0,0);
    addToPointsArray(a, 330, 292);
    addToPointsArray(a, 287, 337);
    addToPointsArray(a, 397, 526);
    addToPointsArray(a, 340, 242);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue la J
    glColor3f(1,0,0);
    currPoint[0]=340;
    currPoint[1]=243;
    addToPointsArray(a, 340, 243);
    addToPointsArray(a, 273, 0);
    addToPointsArray(a, 210, 3);
    addToPointsArray(a, 200, 38);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Gancho de la J terminada
    //Prosigue la J
    glColor3f(1,0,0);
    addToPointsArray(a, 200, 38);
    addToPointsArray(a, 162, 262);
    addToPointsArray(a, 131, 306);
    addToPointsArray(a, 247, 280);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //COmienza la a
    glColor3f(1,0,0);
    currPoint[0]= 416;
    currPoint[1]= 295;
    addToPointsArray(a, 416, 295);
    addToPointsArray(a, 381, 304);
    addToPointsArray(a, 364, 285);
    addToPointsArray(a, 366, 271);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la a
    glColor3f(1,0,0);
    addToPointsArray(a, 366, 271);
    addToPointsArray(a, 384, 266);
    addToPointsArray(a, 400, 275);
    addToPointsArray(a, 403, 284);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Comienza la c
    glColor3f(1,0,0);
    addToPointsArray(a, 403, 284);
    addToPointsArray(a, 408, 267);
    addToPointsArray(a, 438, 282);
    addToPointsArray(a, 446, 290);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosig la c
    glColor3f(1,0,0);
    addToPointsArray(a, 446, 290);
    addToPointsArray(a, 456, 285);
    addToPointsArray(a, 456, 286);
    addToPointsArray(a, 451, 290);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina la c y comienza la k
    glColor3f(1,0,0);
    addToPointsArray(a, 451, 290);
    addToPointsArray(a, 389, 248);
    addToPointsArray(a, 518, 275);
    addToPointsArray(a, 510, 320);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue k
    glColor3f(1,0,0);
    addToPointsArray(a, 510, 315);
    addToPointsArray(a, 497, 298);
    addToPointsArray(a, 498, 293);
    addToPointsArray(a, 485, 260);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue k
    glColor3f(1,0,0);
    addToPointsArray(a, 485, 260);
    addToPointsArray(a, 495, 282);
    addToPointsArray(a, 508, 312);
    addToPointsArray(a, 507, 335);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina k Y COMIENZA S
    glColor3f(1,0,0);
    addToPointsArray(a, 507, 335);
    addToPointsArray(a, 494, 255);
    addToPointsArray(a, 538, 273);
    addToPointsArray(a, 546, 297);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Termina k Y COMIENZA S
    glColor3f(1,0,0);
    addToPointsArray(a, 546, 297);
    addToPointsArray(a, 501, 225);
    addToPointsArray(a, 634, 296);
    addToPointsArray(a, 559, 289);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //cOMIENZA LA O
    glColor3f(1,0,0);
    addToPointsArray(a, 559, 289);
    addToPointsArray(a, 593, 289);
    addToPointsArray(a, 610, 289);
    addToPointsArray(a, 608, 287);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Comienza la n
    glColor3f(1,0,0);
    addToPointsArray(a, 608, 287);
    addToPointsArray(a, 611, 278);
    addToPointsArray(a, 611, 272);
    addToPointsArray(a, 602, 267);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue la n
    glColor3f(1,0,0);
    addToPointsArray(a, 602, 267);
    addToPointsArray(a, 660, 383);
    addToPointsArray(a, 711, 383);
    addToPointsArray(a, 682, 354);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    //Prosigue la n
    glColor3f(1,0,0);
    addToPointsArray(a, 682, 354);
    addToPointsArray(a, 582, 185);
    addToPointsArray(a, 498, 130);
    addToPointsArray(a, 715, 128);
    glColor3f(0,0,0);
    bezier(a,4000);
    deletePointsArray(a);

    glFlush();

    glutMainLoop();
}
