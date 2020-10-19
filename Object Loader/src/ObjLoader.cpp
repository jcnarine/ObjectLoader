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
	 // If our file fails to open, we will throw an error
	 if (!file) {
		 throw std::runtime_error("Failed to open file");
	 }

	 std::vector< glm::fvec3 > temp_vertices;
	 std::vector< glm::fvec2 > temp_uvs;
	 std::vector< glm::fvec3 > temp_normals;

	 std::string line, id;

	 // Iterate as long as there is content to read
	 while (std::getline(file, line)) {
		 trim(line);
		 if (line.substr(0, 2) == "# ")
		 {
			 // Comment, no-op
		 }
		 else if (line.substr(0, 2) == "v ")
		 {
			 std::istringstream ss = std::istringstream(line.substr(2));
			 glm::vec3 pos;
			 ss >> pos.x >> pos.y >> pos.z;
			 temp_vertices.push_back(pos);
		 }
		 else if (line.substr(0, 3) == "vt ")
		 {
			 std::istringstream ss = std::istringstream(line.substr(3));
			 glm::vec2 uv;
			 ss >> uv.x >> uv.y;
			 temp_uvs.push_back(uv);
		 }
		 else if (line.substr(0, 3) == "vn ")
		 {
			 std::istringstream ss = std::istringstream(line.substr(3));
			 glm::vec3 normal;
			 ss >> normal.x >> normal.y >> normal.z;
			 temp_normals.push_back(normal);
		 }
		 else if (line.substr(0, 2) == "f ")
		 {
			 std::istringstream ss = std::istringstream(line.substr(2));

			 std::string v1, v2, v3;
			 char tempIgnoreChar;

			 unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			 ss >> vertexIndex[0] >> tempIgnoreChar >> uvIndex[0] >> tempIgnoreChar >> normalIndex[0] >>  vertexIndex[1] >> tempIgnoreChar >> uvIndex[1] >> tempIgnoreChar >> normalIndex[1] >> vertexIndex[2] >> tempIgnoreChar >> uvIndex[2] >> tempIgnoreChar >> normalIndex[2];

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

	 for (unsigned int i = 0; i < vertexIndices.size(); i++)
	 {
		 unsigned int vertexIndex = vertexIndices[i];
		 unsigned int uvIndex = uvIndices[i];
		 unsigned int normIndex = normalIndices[i];

		 Interleaved.push_back(temp_vertices[vertexIndex - 1].x);
		 Interleaved.push_back(temp_vertices[vertexIndex - 1].y);
		 Interleaved.push_back(temp_vertices[vertexIndex - 1].z);
		 Interleaved.push_back(1.0f);
		 Interleaved.push_back(1.0f);
		 Interleaved.push_back(1.0f);
		 Interleaved.push_back(temp_uvs[uvIndex - 1].x);
		 Interleaved.push_back(temp_uvs[uvIndex - 1].y);
		 Interleaved.push_back(temp_normals[normIndex - 1].x);
		 Interleaved.push_back(temp_normals[normIndex - 1].y);
		 Interleaved.push_back(temp_normals[normIndex - 1].z);
	 }
	 
	 std::cout<<"Loading Object was a success!";
	 return makeVAO();
 }

VertexArrayObject::sptr ObjLoader::makeVAO() {

	VertexBuffer::sptr vbo = VertexBuffer::Create();
	vbo->LoadData(Interleaved.data(), Interleaved.size());

	VertexArrayObject::sptr result = VertexArrayObject::Create();

	size_t stride = sizeof(float) * 11;

	result->AddVertexBuffer(vbo, {
	BufferAttribute(0, 3, GL_FLOAT, false, stride, NULL),
	BufferAttribute(1, 3, GL_FLOAT, false, stride, sizeof(float) * 3),
	BufferAttribute(2, 2, GL_FLOAT, false, stride, sizeof(float) * 6),
	BufferAttribute(3, 3, GL_FLOAT, false, stride, sizeof(float) * 8)
		});

	return result;
}

 ObjLoader::ObjLoader(const std::string f)
 {
 filename = f;
 }
