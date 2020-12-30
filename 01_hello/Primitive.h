#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glad/glad.h>

#include "common.h"

#include <vector>

class Primitive
{
public:
	Primitive();
	~Primitive();

	void draw();

protected:
	void setupPrimitive();

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int indicesSize;

	virtual std::vector<Vertex> getVertices() = 0;
	virtual std::vector<unsigned int> getIndices() = 0;
};

#endif