#include "MyIcosphere.h"

MyIcosphere::MyIcosphere(float radius, int subdivision, bool smooth)
{
	this->icosphere = new Icosphere(radius, subdivision, smooth);
	this->setupPrimitive();
}

MyIcosphere::~MyIcosphere()
{
	delete this->icosphere;
}

std::vector<Vertex> MyIcosphere::getVertices()
{
	const float* p = this->icosphere->getInterleavedVertices();
	unsigned int n = this->icosphere->getVertexCount();
	
	std::vector<Vertex> vertices;
	vertices.assign((Vertex*)p, (Vertex*)p + n);

	return vertices;
}

std::vector<unsigned int> MyIcosphere::getIndices()
{
	const unsigned int* p = this->icosphere->getIndices();
	const unsigned int n = this->icosphere->getIndexCount();

	std::vector<unsigned int> indices;
	indices.assign(p, p + n);

	return indices;
}
