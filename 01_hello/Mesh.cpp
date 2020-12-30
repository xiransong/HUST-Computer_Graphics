#include "Mesh.h"

Mesh::Mesh(Primitive* primitive, Texture* texture)
{
	this->primitive = primitive;
	this->texture = texture;
}

Mesh::~Mesh()
{
}

void Mesh::draw()
{
	this->texture->bindTexture();
	this->primitive->draw();
}
