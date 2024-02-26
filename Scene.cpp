#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "tinyxml2.h"
#include "Triangle.h"
#include "Helpers.h"
#include "Scene.h"

#include <set>
#include <unordered_set>

using namespace tinyxml2;
using namespace std;

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *xmlElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *rootNode = xmlDoc.FirstChild();

	// read background color
	xmlElement = rootNode->FirstChildElement("BackgroundColor");
	str = xmlElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	xmlElement = rootNode->FirstChildElement("Culling");
	if (xmlElement != NULL)
	{
		str = xmlElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			this->cullingEnabled = true; 
			// TODO Pay attention manage
		}
		else
		{
			this->cullingEnabled = false;
			// TODO Pay attention manage
		}
	}

	// read cameras
	xmlElement = rootNode->FirstChildElement("Cameras");
	XMLElement *camElement = xmlElement->FirstChildElement("Camera");
	XMLElement *camFieldElement;
	while (camElement != NULL)
	{
		Camera *camera = new Camera();

		camElement->QueryIntAttribute("id", &camera->cameraId);

		// read projection type
		str = camElement->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			camera->projectionType = ORTOGRAPHIC_PROJECTION;
		}
		else
		{
			camera->projectionType = PERSPECTIVE_PROJECTION;
		}

		camFieldElement = camElement->FirstChildElement("Position");
		str = camFieldElement->GetText();
		sscanf(str, "%f %f %f", &camera->position.x, &camera->position.y, &camera->position.z);

		camFieldElement = camElement->FirstChildElement("Gaze");
		str = camFieldElement->GetText();
		sscanf(str, "%f %f %f", &camera->gaze.x, &camera->gaze.y, &camera->gaze.z);

		camFieldElement = camElement->FirstChildElement("Up");
		str = camFieldElement->GetText();
		sscanf(str, "%f %f %f", &camera->v.x, &camera->v.y, &camera->v.z);

		camera->gaze = normalizeVec3(camera->gaze);
		camera->u = crossProductVec3(camera->gaze, camera->v);
		camera->u = normalizeVec3(camera->u);

		camera->w = inverseVec3(camera->gaze);
		camera->v = crossProductVec3(camera->u, camera->gaze);
		camera->v = normalizeVec3(camera->v);

		camFieldElement = camElement->FirstChildElement("ImagePlane");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &camera->left, &camera->right, &camera->bottom, &camera->top,
			   &camera->near, &camera->far, &camera->horRes, &camera->verRes);

		camFieldElement = camElement->FirstChildElement("OutputName");
		str = camFieldElement->GetText();
		camera->outputFilename = string(str);

		camera->backgroundColor = backgroundColor;
		camera->cullingEnabled = cullingEnabled;


		// cout << "Camera output debug:\n";
		// cout << camera->projectionType << endl;
		// cout << camera->position << endl;
		// cout << camera->gaze << endl;
		// cout << camera->v << endl;
		// cout << camera->w << endl;
		// cout << camera->u << endl;
		// cout << camera->left << endl;
		// cout << camera->right << endl;
		// cout << camera->near << endl;
		// cout << camera->far << endl;
		// cout << camera->outputFilename << endl;
		// cout << camera->backgroundColor << endl;
		// cout << camera->cullingEnabled << endl;

		this->cameras.push_back(camera);

		camElement = camElement->NextSiblingElement("Camera");
	}

	// read vertices
	xmlElement = rootNode->FirstChildElement("Vertices");
	XMLElement *vertexElement = xmlElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (vertexElement != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = vertexElement->Attribute("position");
		sscanf(str, "%f %f %f", &vertex->x, &vertex->y, &vertex->z);

		str = vertexElement->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		this->vertices.push_back(vertex);
		this->colorsOfVertices.push_back(color);

		vertexElement = vertexElement->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	xmlElement = rootNode->FirstChildElement("Translations");
	XMLElement *translationElement = xmlElement->FirstChildElement("Translation");
	while (translationElement != NULL)
	{
		int id;
		translationElement->QueryIntAttribute("id", &id);

		str = translationElement->Attribute("value");
		double tx, ty, tz;
		sscanf(str, "%lf %lf %lf", &tx, &ty, &tz);
		Translation *translation = new Translation(id, tx, ty, tz);

		this->translations.push_back(translation);

		translationElement = translationElement->NextSiblingElement("Translation");
	}

	// read scalings
	xmlElement = rootNode->FirstChildElement("Scalings");
	XMLElement *scalingElement = xmlElement->FirstChildElement("Scaling");
	while (scalingElement != NULL)
	{
		int id;
		scalingElement->QueryIntAttribute("id", &id);
		str = scalingElement->Attribute("value");
		double sx, sy, sz;
		sscanf(str, "%lf %lf %lf", &sx, &sy, &sz);

		Scaling *scaling = new Scaling(id, sx, sy, sz);
		
		this->scalings.push_back(scaling);

		scalingElement = scalingElement->NextSiblingElement("Scaling");
	}

	// read rotations
	xmlElement = rootNode->FirstChildElement("Rotations");
	XMLElement *rotationElement = xmlElement->FirstChildElement("Rotation");
	while (rotationElement != NULL)
	{
		int id;
		rotationElement->QueryIntAttribute("id", &id);
		str = rotationElement->Attribute("value");
		double angle, ux, uy, uz;
		sscanf(str, "%lf %lf %lf %lf", &angle, &ux, &uy, &uz);
		Rotation *rotation = new Rotation(id, angle, ux, uy, uz);
		this->rotations.push_back(rotation);

		rotationElement = rotationElement->NextSiblingElement("Rotation");
	}

	// read meshes
	xmlElement = rootNode->FirstChildElement("Meshes");

	XMLElement *meshElement = xmlElement->FirstChildElement("Mesh");
	while (meshElement != NULL)
	{
		Mesh *mesh = new Mesh();

		meshElement->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = meshElement->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			mesh->type = WIREFRAME_MESH;
		}
		else
		{
			mesh->type = SOLID_MESH;
		}

		// read mesh transformations
		XMLElement *meshTransformationsElement = meshElement->FirstChildElement("Transformations");
		XMLElement *meshTransformationElement = meshTransformationsElement->FirstChildElement("Transformation");

		while (meshTransformationElement != NULL)
		{
			char transformationType;
			int transformationId;

			str = meshTransformationElement->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			meshTransformationElement = meshTransformationElement->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *cloneStr;
		int v1, v2, v3;
		XMLElement *meshFacesElement = meshElement->FirstChildElement("Faces");
		str = meshFacesElement->GetText();
		cloneStr = strdup(str);

		row = strtok(cloneStr, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

			if (result != EOF)
			{
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		this->meshes.push_back(mesh);

		meshElement = meshElement->NextSiblingElement("Mesh");
	}
}


void Scene::setHomogeneousVertices() {
	// std::cout << "Homogeneous Vertices Before Modeling Transformations: " << std::endl;
	for (int i = 0; i < this->vertices.size(); i ++) {
		this->homogeneousVertices.push_back(Vec4(this->vertices[i]->x, this->vertices[i]->y, this->vertices[i]->z, 1));
	}
}

void Scene::applyModelingTransformations() {
	for (int i = 0; i < meshes.size(); i ++) {
		float identity[4][4] = {
			{1, 0, 0, 0},
			{0, 1, 0, 0}, 
			{0, 0, 1, 0}, 
			{0, 0, 0, 1}
		};
		Matrix4 identityMatrix(identity);
		meshes[i]->modelingTransformationMatrix = identityMatrix;
		meshes[i]->setFaces(homogeneousVertices, colorsOfVertices);
		for (int j = 0; j < meshes[i]->numberOfTransformations; j ++) {
			if (meshes[i]->transformationTypes[j] == 't') {
				if (!j) meshes[i]->modelingTransformationMatrix = this->translations[meshes[i]->transformationIds[j]-1]->matrix;
				else meshes[i]->modelingTransformationMatrix = this->translations[meshes[i]->transformationIds[j]-1]->matrix * meshes[i]->modelingTransformationMatrix;
			} 
			else if (meshes[i]->transformationTypes[j] == 's') {
			 	if (!j) meshes[i]->modelingTransformationMatrix = this->scalings[meshes[i]->transformationIds[j]-1]->matrix;
			 	else meshes[i]->modelingTransformationMatrix = this->scalings[meshes[i]->transformationIds[j]-1]->matrix * meshes[i]->modelingTransformationMatrix;
			} 
			else
			if (meshes[i]->transformationTypes[j] == 'r') {
				if (!j) meshes[i]->modelingTransformationMatrix = this->rotations[meshes[i]->transformationIds[j]-1]->matrix;
				else meshes[i]->modelingTransformationMatrix = this->rotations[meshes[i]->transformationIds[j]-1]->matrix * meshes[i]->modelingTransformationMatrix;
			} 
		}
		// TODO: Check edge case when vertices can be shared across different meshes.
		for (int j = 0; j < meshes[i]->numberOfTriangles; j ++) {
			/*if (i == 2) {
				std::cout << "BEFORE Vertex 1: " << meshes[i]->faces[j].vertices[0] << std::endl;
				std::cout << "BEFORE Vertex 2: " << meshes[i]->faces[j].vertices[1] << std::endl;
				std::cout << "BEFORE Vertex 3: " << meshes[i]->faces[j].vertices[2] << std::endl << std::endl;
			}*/
			meshes[i]->faces[j].vertices[0] = meshes[i]->modelingTransformationMatrix * meshes[i]->faces[j].vertices[0];
			meshes[i]->faces[j].vertices[1] = meshes[i]->modelingTransformationMatrix * meshes[i]->faces[j].vertices[1];
			meshes[i]->faces[j].vertices[2] = meshes[i]->modelingTransformationMatrix * meshes[i]->faces[j].vertices[2];
			/*if (i == 2) {
				std::cout << "AFTER Vertex 1: " << meshes[i]->faces[j].vertices[0] << std::endl;
				std::cout << "AFTER Vertex 2: " << meshes[i]->faces[j].vertices[1] << std::endl;
				std::cout << "AFTER Vertex 3: " << meshes[i]->faces[j].vertices[2] << std::endl << std::endl;
			}*/
		}
	}
}
/*
A=(-2.121320,2.200000,-13.717871)
C=(-9.475230,2.200000,-6.363961)
E=(-2.121320,2.200000,0.989949)
F=(5.232590,2.200000,-6.363961)
G=(-2.121320,-8.200000,-13.717871)
H=(-9.475230,-8.200000,-6.363961)
X=(-2.121320,-8.200000,0.989949)
Y=(5.232590,-8.200000,-6.363961)
Gaze1=(0,5,0)
Gaze2=(0.3,4.1,3.5)
*/

/*
	Transformations, clipping, culling, rasterization are done here.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	camera->initializeImage();
	camera->render();
}

void Scene::render() {
	setHomogeneousVertices();
	applyModelingTransformations();
	for (int i = 0; i < this->cameras.size(); i ++) {
		this->cameras[i]->setMeshes(meshes);
		this->cameras[i]->applyCameraTransformation();
		this->cameras[i]->applyProjectionTransformation();
		this->cameras[i]->backfaceCulling();
		this->cameras[i]->applyViewportTransformation();
		this->cameras[i]->primitiveAssembly(cullingEnabled);
		forwardRenderingPipeline(this->cameras[i]);
		this->cameras[i]->writeImageToPPMFile();
		// convertPPMToPNG(this->cameras[i]->outputFilename);
	}
}