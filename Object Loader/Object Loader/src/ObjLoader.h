#pragma once

#include "VertexArrayObject.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>



class ObjLoader
{
public:
	//static VertexArrayObject loadObj(const std::string& filename);
    static void loadObj();
	ObjLoader(const std::string f);

protected:
	std::string filename;
	VertexArrayObject vao;

};