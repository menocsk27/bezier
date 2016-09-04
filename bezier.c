#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "malloc.h"
#include <time.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static int res = 700;
static int currPoint[2];
static int nextPoint[2];
static int (*points)[8]; //La cantidad de líneas la defino cargándola del archivo
static int lines = 0;
static int pixel = 1;

typedef struct {
  double r;
  double g;
  double b;
} COLOR;

COLOR **buffer;

double min(double a, double b){
    if(a < b) { return a; }
    else { return b; }
}

double max(double a, double b){
    if(a < b) { return b; }
    else { return a; }
}

void plot (int x, int y){
    COLOR color;

    glBegin (GL_POINTS);
    glVertex2i (x,y);
    glEnd();
    if (pixel > 0){
        glFlush();    
    }   
}

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

void paramEc (int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3,double t) {
    
    int x0p, y0p, x1p, y1p, x2p, y2p, x3p, y3p;
    double exp3, exp2, exp1, exp0;

    exp0 = pow((1-t),3);
    exp1 = 3*pow((1-t),2)*t;
    exp2 = 3*(1-t)*pow(t,2);
    exp3 = pow(t,3);

    x0p = exp0 * x0; y0p = exp0 * y0;    
    x1p = exp1 * x1; y1p = exp1 * y1;
    x2p = exp2 * x2; y2p = exp2 * y2;
    x3p = exp3 * x3; y3p = exp3 * y3;

    //Obtengo el siguiente punto que quiero pintar
    nextPoint[0] = x0p + x1p + x2p + x3p;   
    nextPoint[1] = y0p + y1p + y2p + y3p;

    //printf("(%i,%i)\n", nextPoint[0], nextPoint[1]);
    //Dibujo la línea entre el punto actual y el nuevo punto calculado
    bresenham(currPoint[0],currPoint[1],nextPoint[0],nextPoint[1],plot);
    
    //Dejo el último punto como punto actual para la siguiente iteración. 
    currPoint[0] = nextPoint[0];
    currPoint[1] = nextPoint[1];
}

int readFile(){

    char *filename = "points.txt";    
    char comma;
    int i, j, k, c;

    int value;

    FILE* file;
    if (file = fopen(filename, "r"))
    {
        while ((c = getc(file)) != EOF){
            if(c == '\n'){
                lines++;
            }
        }
        lines++;

        fclose(file);
        
        points = malloc(sizeof(int[8])*lines);

        file = fopen(filename, "r");
        for(i = 0; i < lines; i++){
            for(j = 0; j < 8; j++){
                if(j < 7){
                    fscanf(file,"%i",&value);
                    fscanf(file,"%c",&comma);
                }else{
                    fscanf(file,"%i",&value);
                }
                points[i][j] = value;
            }
        }

        fclose(file);
    
        return 1;
    }
    else {
        return 0;    
    }
    
}

void bezier (int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3,int ptAmount) {
    int i;
    double t;
    currPoint[0] = x0; currPoint[1] = y0;

    for (i=0;i<=ptAmount;i++){
        t = ((double)i)/ptAmount;
        paramEc(x0,y0,x1,y1,x2,y2,x3,y3,t);
    }
}

int main(int argc, char *argv[]){
    int i,x0,y0,x1,y1,x2,y2,x3,y3; //Los puntos gravitacionales
    int mode = 1;

    if( argc > 2 ) {
        printf("Se espera 1 o ningún parámetro.\n");
        return 0;
    }
    else if (argc == 2){
        if (sscanf (argv[1], "%i", &mode)!=1) {
            printf("Modo no reconocido. Puede que el parámetro no fuera un entero.\n");
            return 0;
        }
        else {
            pixel = mode;
        }    
    }
    
    buffer = (COLOR **)malloc(res * sizeof(COLOR*));
        
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(res,res);
    glutCreateWindow("Firma 2014121700");
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(-0.5, res +0.5, -0.5, res + 0.5);

    if (readFile()==0){
        printf("El archivo con los puntos no pudo encontrarse.\n");
        return 0;
    }  
    else {
        for (i = 0; i < lines; i++ ) {
            x0 = points[i][0]/1.5; y0 = points[i][1]/1.5+180;
            x1 = points[i][2]/1.5; y1 = points[i][3]/1.5+180;
            x2 = points[i][4]/1.5; y2 = points[i][5]/1.5+180;
            x3 = points[i][6]/1.5; y3 = points[i][7]/1.5+180;
            
            if (mode == 2 ) {
                glColor3f(1,0,0);
                bresenham(x0,y0,x1,y1,plot);
                bresenham(x1,y1,x2,y2,plot);
                bresenham(x2,y2,x3,y3,plot);
            }

            glColor3f(0,1,0);
            bezier(x0,y0,x1,y1,x2,y2,x3,y3,200);
        }
            
        glFlush();
        glutMainLoop();
    }
}
