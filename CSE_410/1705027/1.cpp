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

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};

struct point pos;
struct point l,r,u;
double RADIUS, radius, height;
double d = RADIUS - radius;

struct point initPoint(double x, double y, double z)
{
	struct point p;
	p.x=x;
	p.y=y;
	p.z=z;
	return p;
}

struct point NegPoint(struct point p){
	struct point n;
	n.x=-p.x;
	n.y=-p.y;
	n.z=-p.z;
	return n;
}

struct point SumPoints(struct point p, struct point q){
	struct point s;
	s.x=p.x+q.x;
	s.y=p.y+q.y;
	s.z=p.z+q.z;
	return s;
}

struct point multiplyPointByScalar(struct point p, double s){
	struct point m;
	m.x=p.x*s;
	m.y=p.y*s;
	m.z=p.z*s;
	return m;
}

void printPoint(struct point p){
	printf("(%lf, %lf, %lf)\n",p.x,p.y,p.z);
}

void printDetails(){
	printf("pos : \n");
	printPoint(pos);
	printf("l : \n");
	printPoint(l);
	printf("r : \n");
	printPoint(r);
	printf("u : \n");
	printPoint(u);
}

struct point crossProduct(struct point p, struct point q){
	struct point c;
	c.x=p.y*q.z-p.z*q.y;
	c.y=p.z*q.x-p.x*q.z;
	c.z=p.x*q.y-p.y*q.x;
	return c;
}

double getAngle(double s){
	return s * (pi/180);
}

struct point rotateVector(struct point p, struct point ax, double angle){
	struct point r;
	struct point c=crossProduct(ax,p);
	r=SumPoints(multiplyPointByScalar(p,cos(angle)),multiplyPointByScalar(c,sin(angle)));
	return r;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
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


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}



void drawOneEighthSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                
			}glEnd();
		}
	}
}

void drawoneforthCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    struct point points2[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*0.5*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*0.5*pi);
        points[i].z=height/2;

        points2[i].x=radius*cos(((double)i/(double)segments)*0.5*pi);
        points2[i].y=radius*sin(((double)i/(double)segments)*0.5*pi);
        points2[i].z=-height/2;
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        //if(i<segments/2)shade=2*(double)i/(double)segments;
        //else shade=2*(1.0-(double)i/(double)segments);
        //glColor3f(shade,shade,shade);
        glColor3f(0,1,0);
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


void drawSphereUpperPart(){
    //1
    glPushMatrix();
    {
    glTranslatef(radius,radius,radius);
    drawOneEighthSphere(d, 30,20);
    }
    glPopMatrix();


    glPushMatrix();
    {
	glRotatef(90,0,1,0);
    glTranslatef(radius,radius,radius);
    drawOneEighthSphere(d, 30,20);
    }
    glPopMatrix();


    glPushMatrix();
    {
	glRotatef(180,0,1,0);
    glTranslatef(radius,radius,radius);
    drawOneEighthSphere(d, 30,20);
    }
    glPopMatrix();


    glPushMatrix();
    {
	glRotatef(270,0,1,0);
    glTranslatef(radius,radius,radius);
    drawOneEighthSphere(d, 30,20);
    }
    glPopMatrix();
}


void drawSpherePart(){
    glColor3f(1,0,0);

    //upper
    glPushMatrix();
    {
    drawSphereUpperPart();
    }
    glPopMatrix();

    //lower
    glPushMatrix();
    {
    glRotatef(180,1,0,0);
    drawSphereUpperPart();
    }
    glPopMatrix();

}

void drawCyliderPart(){
    
    glPushMatrix();
    {
    glTranslatef(radius,radius,0);
    drawoneforthCylinder(d,radius*2,50);
    }
    glPopMatrix();


	glPushMatrix();
    {
	glRotatef(90,0,1,0);
    glTranslatef(radius,radius,0);
    drawoneforthCylinder(d,radius*2,50);
    }
    glPopMatrix();

	glPushMatrix();
    {
	glRotatef(180,0,1,0);
    glTranslatef(radius,radius,0);
    drawoneforthCylinder(d,radius*2,50);
    }
    glPopMatrix();


	glPushMatrix();
    {
	glRotatef(270,0,1,0);
    glTranslatef(radius,radius,0);
    drawoneforthCylinder(d,radius*2,20);
    }
    glPopMatrix();
}

void drawCylinderSides(){
	glPushMatrix();
    {
	glRotatef(90,0,1,0);
    glTranslatef(radius,radius,0);
    drawoneforthCylinder(d,radius*2,50);
    }
    glPopMatrix();


	glPushMatrix();
    {
	glRotatef(270,0,1,0);
    glTranslatef(radius,radius,0);
    drawoneforthCylinder(d,radius*2,50);
    }
    glPopMatrix();
}

void drawCylinders(){
	glColor3f(0,1,0);
	glPushMatrix();
	{
	drawCyliderPart();
	}
	glPopMatrix();

	glPushMatrix();
	{
	glRotatef(180,1,0,0);
	drawCyliderPart();
	}
	glPopMatrix();

	glPushMatrix();
	{
	glRotatef(90,0,0,1);
	drawCylinderSides();
	}
	glPopMatrix();


	glPushMatrix();
	{
	glRotatef(-90,0,0,1);
	drawCylinderSides();
	}
	glPopMatrix();
}

void drawSides(){
    glColor3f(1,1,1);

    glPushMatrix();
    {
        glTranslatef(RADIUS,0,0);
        glRotatef(90,0,1,0);
        drawSquare(radius);
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(-RADIUS,0,0);
        glRotatef(90,0,1,0);
        drawSquare(radius);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0,RADIUS,0);
        glRotatef(90,1,0,0);
        drawSquare(radius);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0,-RADIUS,0);
        glRotatef(90,1,0,0);
        drawSquare(radius);
    }
    glPopMatrix();

    glPushMatrix();
	{
    glTranslatef(0,0, RADIUS);
    drawSquare(radius);
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(0,0,-RADIUS);
        drawSquare(radius);
    }
    glPopMatrix();
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			r = rotateVector(r, u, getAngle(3.0));
			l = rotateVector(l, u, getAngle(3.0));
			printDetails();
			break;
		case '2':
			r = rotateVector(r, u, getAngle(-3.0));
			l = rotateVector(l, u, getAngle(-3.0));
			printDetails();
			break;
		case '3':
			l = rotateVector(l, r, getAngle(-3.0));
			u = rotateVector(u, r, getAngle(-3.0));
			printDetails(); 
			break;
		case '4':
			l = rotateVector(l, r, getAngle(3.0));
			u = rotateVector(u, r, getAngle(3.0));
			printDetails(); 
			break;
		case '5':
			r = rotateVector(r, l, getAngle(3.0));
			u = rotateVector(u, l, getAngle(3.0));
			printDetails();
			break;
		case '6':
			r = rotateVector(r, l, getAngle(-3.0));
			u = rotateVector(u, l, getAngle(-3.0));
			printDetails();
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos = SumPoints(pos, NegPoint(multiplyPointByScalar(l,3.0)));
			printDetails();
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos = SumPoints(pos, (multiplyPointByScalar(l,3.0)));
			printDetails();
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos = SumPoints(pos, NegPoint(multiplyPointByScalar(r,3.0)));
			printDetails();
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			pos = SumPoints(pos, (multiplyPointByScalar(r,3.0)));
			printDetails();
			break;

		case GLUT_KEY_PAGE_UP:
			pos = SumPoints(pos, (multiplyPointByScalar(u,3.0)));
			printDetails();
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = SumPoints(pos, NegPoint(multiplyPointByScalar(u,3.0)));
			printDetails();
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			if(radius>0){
				radius-=1;
				d+=1;
			}
			break;
		case GLUT_KEY_END:
			if(radius<RADIUS){
				radius+=1;
				d-=1;
			}
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
	gluLookAt(pos.x,pos.y,pos.z, pos.x+l.x,pos.y+l.y,pos.z+l.z, u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //offline
    drawSides();
    drawSpherePart();
    drawCylinders();
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
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

    RADIUS = 30;
    radius = 15;
	d = RADIUS - radius;

	pos = initPoint(0,0,-190);
	u = initPoint(0,1,0);
	r = initPoint(1,0,0);
	l = initPoint(0,0,1);

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