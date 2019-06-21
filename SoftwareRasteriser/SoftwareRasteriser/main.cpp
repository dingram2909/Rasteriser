#pragma once
#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

//#include "Texture.h"

int main() {
	SoftwareRasteriser r(800,600); //Make an 800 by 600 window to draw in

	srand(static_cast <unsigned> (time(0)));
	float rand1;
	float rand2;
	float rand3;
	const float LOW = -99.999f;
	const float HIGH = 100.0f;
	
	/*STAR MAP*/
	RenderObject*starmap = new RenderObject();
	vector<Vector3> stars;
	for (int i = 0; i < 10000; ++i)
	{
		rand1 = LOW + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (HIGH - LOW));
		rand2 = LOW + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (HIGH - LOW));
		rand3 = LOW + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (HIGH - LOW));
		stars.push_back(Vector3(rand1, rand2, rand3));
	}
	starmap->mesh = Mesh::GeneratePoints(stars);
	/*BIG DIPPER*/
	/*RenderObject*dipper = new RenderObject();
	vector<Vector3> dipPts;
	
	dipPts.push_back(Vector3(0.6f, -0.6f, 20.0f));
	dipPts.push_back(Vector3(0.7f, -0.7f, 20.0f));
	dipPts.push_back(Vector3(0.8f, -0.9f, 20.0f));
	dipPts.push_back(Vector3(0.9f, -0.9f, 20.0f));
	dipPts.push_back(Vector3(0.95f, -0.7f, 20.0f));

	dipper->mesh = Mesh::GenerateLinestrip(dipPts);*/

	/*PLANET*/
	RenderObject*planet = new RenderObject();
	vector<Vector3> planetPts;	
	float radius = 1.0f;
	int noPts = 20;
	Vector3 centre = Vector3(3.5f, 3.5f, 30.0f);
	for (int i = 0; i < noPts; ++i){
		float x = centre.x + radius * cos(2 * PI * i / noPts);
		float y = centre.y + radius * sin(2 * PI * i / noPts);

		planetPts.push_back(Vector3(x, y, centre.z));
	}

	//Colour pColour = Colour(255, 255, 255, 255);
	planet->mesh = Mesh::GenerateLineloop(planetPts);

	/*SUN*/
	RenderObject*sun = new RenderObject();
	vector<Vector3> sunPts;
	radius = 2.0f;
	noPts = 36;
	centre = Vector3(-2.7f, -2.7f, 20.0f);
	//sunPts.push_back(centre);
	for (int i = 0; i < noPts; ++i){
		float x = centre.x + radius * cos(2 * PI * i / noPts);
		float y = centre.y + radius * sin(2 * PI * i / noPts);

		sunPts.push_back(Vector3(x, y, centre.z));
	}

	//Colour sColour = Colour(250, 243, 29, 255);
	sun->mesh = Mesh::GenerateLineloop(sunPts);
	
	/*SHIP*/
	RenderObject*s1 = new RenderObject();

	//s1->mesh = Mesh::GenerateTriangle(Vector3(20.0f, 1.0f, 20.0f), Vector3(1.0f, 1.0f, 20.0f), Vector3(20.0f, 20.0f, 20.0f));
	//RenderObject*o1 = new RenderObject();
	s1->mesh = Mesh::LoadMeshFile("ship.mesh");
	//s1->modelMatrix = Matrix4::Translation(Vector3(0, 0, 0));

	/*NORTH STAR*/
	RenderObject*ns1 = new RenderObject();
	RenderObject*ns2 = new RenderObject();
	RenderObject*ns3 = new RenderObject();
	RenderObject*ns4 = new RenderObject();

	ns1->mesh = Mesh::GenerateLine(Vector3(1.0f, 0.0f, 100.0f), Vector3(-1.0f, 0.0f, 100.f));
	ns2->mesh = Mesh::GenerateLine(Vector3(0.0f, 2.0f, 100.0f), Vector3(0.0f, -1.0f, 100.f));
	ns3->mesh = Mesh::GenerateLine(Vector3(0.75f, 0.75f, 100.0f), Vector3(-0.75f, -0.75f, 100.f));
	ns4->mesh = Mesh::GenerateLine(Vector3(-0.75f, 0.75f, 100.0f), Vector3(0.75f, -0.75f, 100.f));

	/*RENDERING*/
	Matrix4 viewMatrix;
	float yaw = 0.0f;
	float aspect = 800.0f / 600.0f;
	
	/*3D Perspective*/
	r.SetProjectionMatrix(Matrix4::Perspective(1.0f, 100.0f, aspect, 45.0f));
	
	while(r.UpdateWindow()) {
		viewMatrix = viewMatrix * Matrix4::Rotation(yaw, Vector3(0, 1, 0));
		yaw = 0;

		if (Keyboard::KeyDown(KEY_A)){
			//viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.001f, 0, 0));
			yaw += 0.1f;
		}
		if (Keyboard::KeyDown(KEY_D)){
			//viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.001f, 0, 0));
			yaw -= 0.1f;
		}
		if (Keyboard::KeyDown(KEY_W)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0, 0, -0.01f));
		}
		if (Keyboard::KeyDown(KEY_S)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0, 0, 0.01f));
		}
		r.SetViewMatrix(viewMatrix);

			r.ClearBuffers();	// Start from 'black' every frame
			// Draw the objects
			r.DrawObject(starmap);
			r.DrawObject(planet);
			r.DrawObject(sun);
			r.DrawObject(s1);
			r.DrawObject(ns1);
			r.DrawObject(ns2);
			r.DrawObject(ns3);
			r.DrawObject(ns4);
			r.SwapBuffers();	// Swap the buffers, display on screen
		}

	return 0;
}