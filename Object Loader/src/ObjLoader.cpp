#include "ObjLoader.h"

#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

const char* ignoreCharacters = " \t\n\r\f\v";

//MeshBuilder<VertexPosNormTexCol> mesh;

static inline std::string& rtrim(std::string& s, const char* t = ignoreCharacters)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

static inline std::string& ltrim(std::string& s, const char* t = ignoreCharacters)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

static inline std::string& trim(std::string& s, const char* t = ignoreCharacters)
{
	return ltrim(rtrim(s, t), t);
}

#pragma endregion 

VertexArrayObject::sptr ObjLoader::loadObj()
 {
	 std::ifstream file;
	 file.open(filename, std::ios::binary);
	 std::istringstream ss;
	 // If our file fails to open, we will throw an error
	 if (!file) {
		 throw std::runtime_error("Failed to open file");
	 }

	 std::vector< glm::fvec3 > temp_vertices;
	 std::vector< glm::fvec2 > temp_uvs;
	 std::vector< glm::fvec3 > temp_normals;

	 std::string line, id;

	 ss.clear();
	 ss.str(line);
	 ss >> id;

	 // Iterate as long as there is content to read
	 while (std::getline(file, line)) {
		 trim(line);
		 if (id == "#")
		 {
			 // Comment, no-op
		 }
		 else if (id == "v")
		 {
			 glm::vec3 pos;
			 ss >> pos.x >> pos.y >> pos.z;
			 temp_vertices.push_back(pos);
		 }
		 else if (id == "vt")
		 {
			 glm::vec3 uv;
			 ss >> uv.x >> uv.y >> uv.z;
			 temp_uvs.push_back(uv);
		 }
		 else if (id == "vn")
		 {
			 glm::vec3 normal;
			 ss >> normal.x >> normal.y >> normal.z;
			 temp_normals.push_back(normal);
		 }
		 else if (id == "f")
		 {
			 std::string v1, v2, v3;

			 unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			 ss >> vertexIndex[0] >> vertexIndex[1] >> vertexIndex[2] >> uvIndex[0] >> uvIndex[1] >> uvIndex[2] >> normalIndex[0] >> normalIndex[1] >> normalIndex[2];

			 vertexIndices.push_back(vertexIndex[0]);
			 vertexIndices.push_back(vertexIndex[1]);
			 vertexIndices.push_back(vertexIndex[2]);
			 uvIndices.push_back(uvIndex[0]);
			 uvIndices.push_back(uvIndex[1]);
			 uvIndices.push_back(uvIndex[2]);
			 normalIndices.push_back(normalIndex[0]);
			 normalIndices.push_back(normalIndex[1]);
			 normalIndices.push_back(normalIndex[2]);

		 }else{}
	 }

	 for (unsigned int i = 0; i < vertexIndices.size(); ++i)
	 {
		 out_vertices.push_back(temp_vertices[vertexIndices[i] - 1]);
		 out_uvs.push_back(temp_uvs[uvIndices[i] - 1]);
		 out_colour.push_back(glm::vec3(1.f, 1.f, 1.f));
		 if (i<normalIndices.size()){
			 out_normal.push_back(temp_normals[normalIndices[i] - 1]);
		 }
	 }

	 std::cout<<"Loading Object was a success!";
	 return makeVAO();
 }

VertexArrayObject::sptr ObjLoader::makeVAO() {

	VertexBuffer::sptr pos_vbo = VertexBuffer::Create();
	pos_vbo->LoadData(out_vertices.data(), out_vertices.size());

	VertexBuffer::sptr col_vbo = VertexBuffer::Create();
	col_vbo->LoadData(out_colour.data(), out_colour.size());

	IndexBuffer::sptr ebo = IndexBuffer::Create();
	ebo->LoadData(vertexIndices.data(), vertexIndices.size());

	VertexArrayObject::sptr result = VertexArrayObject::Create();

	result->AddVertexBuffer(pos_vbo, {
		BufferAttribute(0, 3, GL_FLOAT, false, 0, NULL)
		});

	result->AddVertexBuffer(col_vbo, {
	BufferAttribute(1, 3, GL_FLOAT, false, 0, NULL)
		});

	result->SetIndexBuffer(ebo);

	return result;
}

 ObjLoader::ObjLoader(const std::string f)
 {
 filename = f;
 }

 std::vector<glm::vec3> ObjLoader::returnVertices()
 {
	 return out_vertices;
 }

 std::vector<glm::vec2> ObjLoader::returnUvs()
 {
	 return out_uvs;
 }

 std::vector<glm::vec3> ObjLoader::returnNormals()
 {
	 return out_normal;
 }

 std::vector<glm::vec3> ObjLoader::returnColour()
 {
	 return out_colour;
 }
