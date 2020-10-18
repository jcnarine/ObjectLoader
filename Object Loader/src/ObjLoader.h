#pragma once

#include "VertexArrayObject.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class ObjLoader
{
public:

	VertexArrayObject::sptr loadObj();
	VertexArrayObject::sptr makeVAO();
	ObjLoader(const std::string f);

private:
	std::string filename;
	std::vector < glm::vec3 > out_vertices;
	std::vector < glm::vec2 > out_uvs;
	std::vector < glm::vec3 > out_normal;
	std::vector < glm::vec3 > out_colour;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	static const std::vector<BufferAttribute> bufAttributes;
};