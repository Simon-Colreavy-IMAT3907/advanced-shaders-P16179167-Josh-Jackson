#pragma once
#include "Win32OpenGL.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

struct Texture {
	GLuint id;
	string type;
	string path;
};

class Mesh {
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);
	void draw(GLuint &program, std::string drawType);
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
	glm::vec3 m_pos;
	glm::vec3 m_scale;
	glm::vec3 m_rot;
	void setupMesh();
};