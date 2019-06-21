#include "Mesh.h"


Mesh::Mesh(void)	{
	type			= PRIMITIVE_POINTS;

	numVertices		= 0;

	vertices		= NULL;
	colours			= NULL;
	textureCoords	= NULL;
}

Mesh::~Mesh(void)	{
	delete[] vertices;
	delete[] colours;
	delete[] textureCoords;
}

Mesh* Mesh::GenerateLinestrip(const vector<Vector3> &points)
{
	Mesh* m = new Mesh();

	m->numVertices = points.size();

	m->vertices = new Vector4[m->numVertices];

	for (uint i = 0; i < m->numVertices; ++i){
		m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
	}

	m->type = PRIMITIVE_LINESTRIP;

	return m;
}

Mesh* Mesh::GenerateLineloop(const vector<Vector3> &points)
{
	Mesh* m = new Mesh();

	m->numVertices = points.size();

	m->vertices = new Vector4[m->numVertices];

	for (uint i = 0; i < m->numVertices; ++i){
		m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
	}

	m->type = PRIMITIVE_LINELOOP;

	return m;
}

Mesh* Mesh::GenerateLine(const Vector3 &from, const Vector3 &to)
{
	Mesh* m = new Mesh();

	m->numVertices = 2;

	m->vertices = new Vector4[m->numVertices];
	m->colours = new Colour[m->numVertices];
	
	m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);
	m->vertices[1] = Vector4(to.x, to.y, to.z, 1.0f);

	m->colours[0] = Colour(250, 243, 29, 255);
	m->colours[1] = Colour(250, 243, 29, 255);

	m->type = PRIMITIVE_LINES;

	return m;
}

Mesh* Mesh::GeneratePoints(const vector<Vector3> &points) {
	Mesh* m = new Mesh();

	m->numVertices = points.size();

	m->vertices = new Vector4[m->numVertices];

	for (uint i = 0; i < m->numVertices; ++i){
		m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
	}
	
	m->type = PRIMITIVE_POINTS;

	return m;

}

Mesh* Mesh::GenerateTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3){
	Mesh* m = new Mesh();
	m->type = PRIMITIVE_TRIANGLES;
	m->numVertices = 3;

	m->vertices = new Vector4[m->numVertices];
	m->colours = new Colour[m->numVertices];
	
	m->vertices[0] = Vector4(p1.x, p1.y, p1.z, 1.0f);
	m->vertices[1] = Vector4(p2.x, p2.y, p2.z, 1.0f);
	m->vertices[2] = Vector4(p3.x, p3.y, p3.z, 1.0f);

	m->colours[0] = Colour(255, 0, 0, 255); // Red
	m->colours[1] = Colour(0, 255, 0, 255); // Green
	m->colours[2] = Colour(0, 0, 255, 255); // Blue

	return m;
}

Mesh* Mesh::GenerateTriFan(const vector<Vector3> &points){
	Mesh* m = new Mesh();
	
	m->numVertices = points.size();

	m->vertices = new Vector4[m->numVertices];
	m->colours = new Colour[m->numVertices];

	for (uint i = 0; i < m->numVertices; ++i){
		m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
		m->colours[i] = Colour(250, 243, 29, 255); //YELLOW
	}
	
	m->type = PRIMITIVE_TRIFAN;
	
	return m;
}

Mesh* Mesh::LoadMeshFile(const string &filename){
	ifstream f(filename);

	if (!f){
		return NULL;
	}

	Mesh*m = new Mesh();
	m->type = PRIMITIVE_TRIANGLES;
	f >> m->numVertices;

	int hasTex = 0;
	int hasColour = 0;

	f >> hasTex;
	f >> hasColour;

	m->vertices = new Vector4[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Colour[m->numVertices];

	for (uint i = 0; i < m->numVertices; ++i){
		f >> m->vertices[i].x;
		f >> m->vertices[i].y;
		f >> m->vertices[i].z;
	}

	if (hasColour){
		for (uint i = 0; i < m->numVertices; ++i){
			f >> m->colours[i].r; 
			f >> m->colours[i].g;
			f >> m->colours[i].b;
			f >> m->colours[i].a;
		}
	}
	return m;
}