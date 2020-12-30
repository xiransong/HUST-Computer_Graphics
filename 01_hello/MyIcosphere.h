#ifndef MYICOSPHERE_H
#define MYICOSPHERE_H

#include "Primitive.h"
#include "Icosphere.h"

class MyIcosphere : public Primitive
{
public:
	MyIcosphere(float radius = 1.0f, int subdivision = 3, bool smooth = false);
	~MyIcosphere();

private:
	Icosphere* icosphere;

	std::vector<Vertex> getVertices();
	std::vector<unsigned int> getIndices();
};

#endif