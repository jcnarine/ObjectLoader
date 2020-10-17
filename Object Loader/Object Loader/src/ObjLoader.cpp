#include "ObjLoader.h"

#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

const char* ignoreCharacters = " \t\n\r\f\v";

struct FaceConstructor
{
	list<glm::vec3> Points;
	list<glm::vec3> Normal;
	list<glm::vec3> Color;
	list<glm::vec2> Uv;

}LoadedObject;

static inline string& rtrim(std::string& s, const char* t = ignoreCharacters)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

static inline string& ltrim(std::string& s, const char* t = ignoreCharacters)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

static inline string& trim(std::string& s, const char* t = ignoreCharacters)
{
	return ltrim(rtrim(s, t), t);
}

#pragma endregion 

 void loadObj()
{
	// Open our file in binary mode
	ifstream file;
	file.open("Text.txt");

	// If our file fails to open, we will throw an error
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	std::string line;
	
	// Iterate as long as there is content to read
	while (std::getline(file, line)) {
		trim(line);
		if (line.substr(0, 1) == "#")
		{
			// Comment, no-op
		}
		else if (line.substr(0, 2) == "v ") 
		{
			istringstream ss = istringstream(line.substr(2));
			glm::vec3 pos;
			LoadedObject.Color.push_back(glm::vec3(1.0, 1.0, 1.0));
			ss >> pos.x >> pos.y >> pos.z;
			LoadedObject.Points.push_back(pos);
			cout<<pos.x;
		}
		else if (line.substr(0, 3) == "vt ") 
		{
			istringstream ss = istringstream(line.substr(3));
			glm::vec3 pos;
			LoadedObject.Color.push_back(glm::vec3(1.0, 1.0, 1.0));
			ss >> pos.x >> pos.y >> pos.z;
			LoadedObject.Points.push_back(pos);
		}
		else if (line.substr(0, 3) == "vn ")
		{
		}
		else if (line.substr(0, 2) == "f ")
		{
		}
		

		}
	}

 ObjLoader::ObjLoader(const std::string f)
 {
 filename = f;
 }
