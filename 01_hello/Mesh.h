#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include "common.h"
#include "shader_m.h"
#include "Primitive.h"
#include "Texture.h"

class Mesh
{
public:
	Mesh(Primitive* primitive, Texture* texture);
	~Mesh();

	void draw();

private:
	Primitive* primitive;
	Texture* texture;
};

#endif