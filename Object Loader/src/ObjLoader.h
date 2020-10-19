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
	std::vector<float> Interleaved;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	static const std::vector<BufferAttribute> bufAttributes;
};