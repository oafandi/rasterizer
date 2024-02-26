#ifndef _SCENE_H_
#define _SCENE_H_
#include "Vec3.h"
#include "Vec4.h"
#include "Color.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Camera.h"
#include "Mesh.h"

class Scene
{
public:

	std::vector<Camera *> cameras;

	std::vector<Vec3 *> vertices;
	std::vector<Color *> colorsOfVertices;
	
	// Transformations
	std::vector<Scaling *> scalings;
	std::vector<Rotation *> rotations;
	std::vector<Translation *> translations;

	std::vector<Mesh *> meshes;
	std::vector<Vec4> homogeneousVertices;

	Color backgroundColor;
	bool cullingEnabled;

	Scene(const char *xmlPath);

	void setHomogeneousVertices();
	void forwardRenderingPipeline(Camera *camera);
	void render();



private:
	void applyModelingTransformations();
};

#endif
