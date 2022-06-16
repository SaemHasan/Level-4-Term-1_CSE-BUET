#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#if __WIN32__
	#include <windows.h>
	#include<glut.h>
#elif __APPLE__
	#include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#define pi (2*acos(0.0))
// #define WHEEL_RADIUS 30
// #define WHEEL_HEIGHT 20
#define WHEEL_STEP 5

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};

struct point wheelCenter;
double rotationAngle;
double directionAngle;
double distance = 100;
int drawRect;

double WHEEL_RADIUS  =30;
double WHEEL_HEIGHT =20;
double antenaHeight = 20;

struct point initPoint(double x, double y, double z)
{
	struct point p;
	p.x=x;
	p.y=y;
	p.z=z;
	return p;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		//glColor3f(1.0, 0, 0);
		glBegin(GL_LINES);{

			glColor3f(1.0, 0, 0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glColor3f(0, 1.0, 0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawLine(double len){
    glBegin(GL_LINES);{

			glColor3f(1.0, 0, 0);
			glVertex3f( 0,0,0);
			glVertex3f(len,0,0);

            glVertex3f(0,0,0);
			glVertex3f(0,0,-antenaHeight);
		}glEnd();
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-16;i<=16;i++){

				// if(i==0)
				// 	continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -170, 0);
				glVertex3f(i*10,  170, 0);

				//lines parallel to X-axis
				glVertex3f(-170, i*10, 0);
				glVertex3f( 170, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawRectangle(double length, double width){
	glColor3f(0.7,0.7,0.7);
	glBegin(GL_QUADS);{
		glVertex3f(length, width,0);
		glVertex3f(length,-width,0);
		glVertex3f(-length,-width,0);
		glVertex3f(-length, width,0);
	}glEnd();
}

void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    struct point points2[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[i].z=height/2;

        points2[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points2[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points2[i].z=-height/2;
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);
        // glColor3f(0,1,0);
        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x,points[i].y,points[i].z);
            glVertex3f(points[i+1].x,points[i+1].y,points[i+1].z);
            glVertex3f(points2[i+1].x,points2[i+1].y,points2[i+1].z);
            glVertex3f(points2[i].x,points2[i].y,points2[i].z);
        }
        glEnd();
    }
}

void drawWheel(){
	
    glPushMatrix();
    {
    	glRotatef(90,1,0,0);
    	drawCylinder(WHEEL_RADIUS,WHEEL_HEIGHT,80);
    }
	glPopMatrix();

	glPushMatrix();
	{
		drawRectangle(WHEEL_RADIUS,WHEEL_HEIGHT/2.0);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotatef(90,0,1,0);
		drawRectangle(WHEEL_RADIUS,WHEEL_HEIGHT/2.0);
	}
	glPopMatrix();
}

void drawtask(){
    if(drawRect==1){
        glPushMatrix();
        {
        glTranslatef(distance/2,distance/2,WHEEL_RADIUS*2);
	    glTranslatef(wheelCenter.x,wheelCenter.y,wheelCenter.z);
	    //glRotatef(directionAngle,0,0,1);
        drawRectangle(distance/2,distance/2);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(distance/2,distance/2,WHEEL_RADIUS*2+antenaHeight);
	        glTranslatef(wheelCenter.x,wheelCenter.y,wheelCenter.z);
	        glRotatef(directionAngle,0,0,1);
            drawLine(60);
        }
        glPopMatrix();
    }
	glTranslatef(0,0,WHEEL_RADIUS);
	glTranslatef(wheelCenter.x,wheelCenter.y,wheelCenter.z);
	glRotatef(directionAngle,0,0,1);
	glRotatef(rotationAngle,0,1,0);
	drawWheel();
}


void drawRobot(){
    glPushMatrix();{
        drawRect=1;
        drawtask();
        drawRect=0;
    }
    glPopMatrix();

    glPushMatrix();{
        glTranslatef(distance, 0,0);
        drawtask();
    }
    glPopMatrix();

    glPushMatrix();{
        glTranslatef(0, distance,0);
        drawtask();
    }
    glPopMatrix();

    glPushMatrix();{
        glTranslatef(distance, distance,0);
        drawtask();
    }
    glPopMatrix();

    

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
		case 'a':
			directionAngle -= WHEEL_STEP;
			break;
		case 'd':
			directionAngle += WHEEL_STEP;
			break;
		case 'w':
			rotationAngle += WHEEL_STEP;
			wheelCenter.x += (WHEEL_RADIUS*WHEEL_STEP*pi/180.0)*cos(directionAngle*pi/180.0);
			wheelCenter.y += (WHEEL_RADIUS*WHEEL_STEP*pi/180.0)*sin(directionAngle*pi/180.0);
			wheelCenter.z = 0;
			break;
		case 's':
			rotationAngle -= WHEEL_STEP;
			wheelCenter.x -= (WHEEL_RADIUS*WHEEL_STEP*pi/180.0)*cos(directionAngle*pi/180.0);
			wheelCenter.y -= (WHEEL_RADIUS*WHEEL_STEP*pi/180.0)*sin(directionAngle*pi/180.0);
			wheelCenter.z = 0;
			break;
        
        case 'x':
            if(WHEEL_RADIUS<(distance/2 - 5)){
                WHEEL_RADIUS+=1;
                WHEEL_HEIGHT+=1;
            }
            break;
        case 'c':
            if(WHEEL_RADIUS>11){
                WHEEL_RADIUS-=1;
                WHEEL_HEIGHT-=1;
            }
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
    //drawtask();
    drawRobot();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	wheelCenter = initPoint(0,0,0);
	rotationAngle =0;
	directionAngle =0;
    drawRect=1;
    WHEEL_RADIUS  =30;
    WHEEL_HEIGHT =20;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
