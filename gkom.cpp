/*

* GKOM Lab. 3: OpenGL Vertex Arrays

*

* Program stanowi zaadaptowana wersje przykladu accnot.c.

//20101116 Michal Kurowski - Modified the project to demonstrate the use of Vertex Arrays
//20110330 Michal Kurowski - Modified the project to demonstrate procedural satellite dish, fixed lighting bug

*/


#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <fstream>
#include <string>

#include "glut.h"
#include <math.h>

using namespace std;

void AnalyzeFile(char* name, int* vertices_count, int* indices_count, int* normals_count)
{
	ifstream f(name);
	string line;
	int& v = *vertices_count;
	int& i = *indices_count;
	int& n = *normals_count;
	v = 0;
	i = 0;
	n = 0;
	while (f.good())
	{
		getline(f, line);
		if (line.length() > 0)
		{
			if (line.find("vn ") == 0)
			{
				//found normal
				n++;
				continue;
			}
			if (line.find("vt ") == 0)
			{
				//found texture coord
				//nop
				continue;
			}
			if (line.find("v ") == 0)
			{
				//found vertex definition
				v++;
				continue;
			}
			if (line.find("f ") == 0)
			{
				//found face definition
				i += 3; //we assume triangles
				continue;
			}

		}
	}
	f.close();

}

void LoadFile(char* name, int *vertices_count, int* indices_count, GLfloat** vertices, GLfloat** normals, GLushort ** indices)
{
	cout << "Reading model " << name << "..." << endl;
	//init
	GLfloat*& v = *vertices;
	GLfloat*& n = *normals;
	GLfloat* tn = NULL;
	GLushort*& i = *indices;
	GLint* tni = NULL;
	int normals_count;
	cout << "Analyzing file..." << endl;
	AnalyzeFile(name, vertices_count, indices_count, &normals_count);
	cout << "Found " << *vertices_count << " vertices, " << normals_count << " normals and " << *indices_count << " indices;" << endl;
	cout << "Allocating memory..." << endl;
	v = new GLfloat[*vertices_count * 3];
	n = new GLfloat[*vertices_count * 3];
	tn = new GLfloat[normals_count * 3];
	i = new GLushort[*indices_count];
	tni = new GLint[*indices_count];

	//read
	cout << "Reading file..." << endl;
	ifstream f(name);
	string line;

	int vi = 0;
	int ii = 0;
	int ni = 0;
	while (f.good())
	{
		getline(f, line);
		if (line.length() > 0)
		{
			if (line.find("vn ") == 0)
			{
				//found normal
				sscanf(line.c_str(), "vn %f %f %f", &tn[ni + 0], &tn[ni + 1], &tn[ni + 2]);
				ni += 3;
				continue;
			}
			if (line.find("vt ") == 0)
			{
				//found texture coord
				//nop
				continue;
			}
			if (line.find("v ") == 0)
			{
				//found vertex definition
				sscanf(line.c_str(), "v %f %f %f", &v[vi + 0], &v[vi + 1], &v[vi + 2]);
				vi += 3;
				continue;
			}
			if (line.find("f ") == 0)
			{
				//found face definition
				int a, b, c;
				sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &a, &tni[ii + 0], &b, &tni[ii + 1], &c, &tni[ii + 2]);
				i[ii + 0] = a; i[ii + 1] = b; i[ii + 2] = c;
				ii += 3;
				continue;
			}

		}
	}

	//convert
	cout << "Performing conversions..." << endl;
	for (int j = 0; j < *indices_count; j++)
	{
		i[j] = i[j] - 1;
		tni[j] = tni[j] - 1;
	}
	for (int j = 0; j < *indices_count; j++)
	{
		int vertex_i = i[j];
		int normal_i = tni[j];
		n[vertex_i * 3 + 0] = tn[normal_i * 3 + 0];
		n[vertex_i * 3 + 1] = tn[normal_i * 3 + 1];
		n[vertex_i * 3 + 2] = tn[normal_i * 3 + 2];
	}

	//cleanup
	cout << "Cleaning up..." << endl;
	delete tni;
	delete tn;
	f.close();
	cout << "Done (^_^)" << endl;

}

static GLfloat* s_ModelVertices = NULL;
static GLfloat* s_ModelNormals = NULL;
static GLushort* s_ModelIndices = NULL;

static int s_ModelVerticesCount = 0;
static int s_ModelIndicesCount = 0;

static GLfloat* s_ModelVertices2 = NULL;
static GLfloat* s_ModelNormals2 = NULL;
static GLushort* s_ModelIndices2 = NULL;

static int s_ModelVerticesCount2 = 0;
static int s_ModelIndicesCount2 = 0;

static GLfloat* s_DishVertices = NULL;
static GLfloat* s_DishNormals = NULL;
static GLfloat* s_DishInvNormals = NULL;
static GLushort* s_DishIndices = NULL;
static GLushort* s_DishInvIndices = NULL;
static int s_DishVerticesCount = 0;
static int s_DishIndicesCount = 0;





void init()

{

	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	GLfloat mat_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };



	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);



	glShadeModel(GL_SMOOTH);



	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);



	glDepthFunc(GL_LESS);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

}

void BruteForceNormalCalculation(GLfloat *v, int vcount, GLushort *ix, int count, GLfloat *n)
{
	//zero
	for (int i = 0; i < vcount * 3; i++)
	{
		n[i] = 0.0f;
	}
	//for each quad
	for (int i = 0; i < count; i += 4)
	{
		float nx, ny, nz;
		//calculate normal
		//we cheat! let's close our eyes and imagine, that our quad is really a triangle

		//variable names compatible with Wikipedia entry on cross product:)

		float a1 = v[ix[i + 1] * 3 + 0] - v[ix[i] * 3 + 0];
		float a2 = v[ix[i + 1] * 3 + 1] - v[ix[i] * 3 + 1];
		float a3 = v[ix[i + 1] * 3 + 2] - v[ix[i] * 3 + 2];

		float b1 = v[ix[i + 2] * 3 + 0] - v[ix[i] * 3 + 0];
		float b2 = v[ix[i + 2] * 3 + 1] - v[ix[i] * 3 + 1];
		float b3 = v[ix[i + 2] * 3 + 2] - v[ix[i] * 3 + 2];

		//cross product

		nx = a2 * b3 - a3 * b2;
		ny = a3 * b1 - a1 * b3;
		nz = a1 * b2 - a2 * b1;


		//normalize it, so that each quad contributes equally...
		float il = 1.0f / sqrt(nx * nx + ny * ny + nz * nz);
		nx *= il;
		ny *= il;
		nz *= il;

		//accumulate
		n[ix[i] * 3 + 0] += nx;
		n[ix[i] * 3 + 1] += ny;
		n[ix[i] * 3 + 2] += nz;

		n[ix[i + 1] * 3 + 0] += nx;
		n[ix[i + 1] * 3 + 1] += ny;
		n[ix[i + 1] * 3 + 2] += nz;

		n[ix[i + 2] * 3 + 0] += nx;
		n[ix[i + 2] * 3 + 1] += ny;
		n[ix[i + 2] * 3 + 2] += nz;

		n[ix[i + 3] * 3 + 0] += nx;
		n[ix[i + 3] * 3 + 1] += ny;
		n[ix[i + 3] * 3 + 2] += nz;
	}
}

void InitializeDishData()
{
	//let's pump lots of polys!
	int sections = 1024;
	int splices = 32;

	//initialize data	
	s_DishVerticesCount = (sections + 2) * splices;

	s_DishVertices = new GLfloat[s_DishVerticesCount * 3];
	s_DishNormals = new GLfloat[s_DishVerticesCount * 3];
	s_DishInvNormals = new GLfloat[s_DishVerticesCount * 3];

	s_DishIndicesCount = (sections + 1) * (splices - 1) * 4;
	s_DishIndices = new GLushort[s_DishIndicesCount];
	s_DishInvIndices = new GLushort[s_DishIndicesCount];

	cout << "Creating dish vertices...." << endl;
	//let's calculate the vertices positions
	//this method is not optimal, but it doesn't have any special cases
	//BTW >> please not the loops' bounds
	int offset = 0;
	for (int section = 0; section < sections + 2; section++)
	{
		for (int splice = 0; splice < splices; splice++)
		{
			float angle = section / (float)sections * 2.0f * 3.14f;
			float radius = splice / (float)(splices - 1);
			s_DishVertices[offset + 0] = sin(angle) * radius;
			s_DishVertices[offset + 1] = cos(angle) * radius;
			s_DishVertices[offset + 2] = radius * radius;
			offset += 3;
		}
	}

	cout << "Creating dish indices...." << endl;
	//let's calculate indices for quads
	offset = 0;
	for (int section = 0; section < sections + 1; section++)
	{
		for (int splice = 0; splice < splices - 1; splice++)
		{
			s_DishIndices[offset + 0] = section * splices + splice;
			s_DishIndices[offset + 1] = section * splices + splice + 1;
			s_DishIndices[offset + 2] = (section + 1) * splices + splice + 1;
			s_DishIndices[offset + 3] = (section + 1) * splices + splice;

			s_DishInvIndices[offset + 0] = (section + 1) * splices + splice;
			s_DishInvIndices[offset + 1] = (section + 1) * splices + splice + 1;
			s_DishInvIndices[offset + 2] = section * splices + splice + 1;
			s_DishInvIndices[offset + 3] = section * splices + splice;

			offset += 4;
		}
	}

	cout << "Brute-forcing dish normals...." << endl;
	//let's calculate the normals in a brute-force, yet generic way
	BruteForceNormalCalculation(s_DishVertices, s_DishVerticesCount, s_DishIndices, s_DishIndicesCount, s_DishNormals);

	//let's invert the normals
	for (int i = 0; i < s_DishVerticesCount * 3; i++)
	{
		s_DishInvNormals[i] = -s_DishNormals[i];
	}

}
void DeinitializeDishData()
{
	delete s_DishVertices;
	delete s_DishNormals;
	delete s_DishIndices;
	delete s_DishInvIndices;
	delete s_DishInvNormals;
}



void displayObjects(int frame_no)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
	glVertexPointer(3, GL_FLOAT, 0, s_ModelVertices);
	glNormalPointer(GL_FLOAT, 0, s_ModelNormals);
	glTranslatef(-2.0f, 0.0f, 0.0f);
	glRotatef(frame_no, 0.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, s_ModelIndicesCount, GL_UNSIGNED_SHORT, s_ModelIndices);
	glPopMatrix();

	glPushMatrix();
	glVertexPointer(3, GL_FLOAT, 0, s_ModelVertices2);
	glNormalPointer(GL_FLOAT, 0, s_ModelNormals2);
	glTranslatef(2.0f, 0.0f, 0.0f);
	glScalef(3.0f, 3.0f, 3.0f);
	glRotatef(-frame_no, 0.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, s_ModelIndicesCount2, GL_UNSIGNED_SHORT, s_ModelIndices2);
	glPopMatrix();

	glPushMatrix();
	glVertexPointer(3, GL_FLOAT, 0, s_DishVertices);
	glNormalPointer(GL_FLOAT, 0, s_DishNormals);

	glTranslatef(0.0f, 4.0f, 0.0f);
	glRotatef(-frame_no, 0.0f, 1.0f, 0.0f);
	glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
	glScalef(1.0f, 1.0f, 0.4f);
	//GOTCHA! Our dish is really two sided... so we draw it twice - please research GL_CULL_FACE
	glDrawElements(GL_QUADS, s_DishIndicesCount, GL_UNSIGNED_SHORT, s_DishIndices);

	glNormalPointer(GL_FLOAT, 0, s_DishInvNormals);
	glDrawElements(GL_QUADS, s_DishIndicesCount, GL_UNSIGNED_SHORT, s_DishInvIndices);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}



void display()

{
	GLfloat light_position[] = { 10, 8, 8, 1 };

	static int frame_no = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (frame_no<360) frame_no++; else frame_no = 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 4.0, 8.0, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	displayObjects(frame_no);


	glFlush();

	glutSwapBuffers();

}



void reshape(GLsizei w, GLsizei h)

{

	if (h > 0 && w > 0) {

		glViewport(0, 0, w, h);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();


		gluPerspective(50.0f, ((float)w) / (float)h, 1.0, 100.0);


		glMatrixMode(GL_MODELVIEW);

	}




}



int main(int argc, char** argv)

{
	LoadFile("girl.obj", &s_ModelVerticesCount, &s_ModelIndicesCount, &s_ModelVertices, &s_ModelNormals, &s_ModelIndices);
	LoadFile("viking.obj", &s_ModelVerticesCount2, &s_ModelIndicesCount2, &s_ModelVertices2, &s_ModelNormals2, &s_ModelIndices2);
	InitializeDishData();

	cout << "Initializing GLUT..." << endl;
	glutInit(&argc, argv);

	cout << "Creating window with context..." << endl;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);



	glutInitWindowPosition(0, 0);

	glutInitWindowSize(800, 600);



	glutCreateWindow("GKOM lab. 3: OpenGL Vertex Arrays");


	cout << "Registering handlers...." << endl;

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutIdleFunc(display);


	cout << "Initializing rendering pipeline..." << endl;
	init();


	cout << "Running..." << endl;
	glutMainLoop();
	cout << "Cleaning up..." << endl;

	DeinitializeDishData();

	delete s_ModelVertices;
	delete s_ModelIndices;
	delete s_ModelNormals;


	delete s_ModelVertices2;
	delete s_ModelIndices2;
	delete s_ModelNormals2;

	cout << "Done:) CU!" << endl;



	return 0;

}

