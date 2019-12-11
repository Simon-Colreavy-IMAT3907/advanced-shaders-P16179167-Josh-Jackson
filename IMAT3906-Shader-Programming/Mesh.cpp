#include "Mesh.h";

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot) {
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	m_pos = pos;
	m_scale = scale;
	m_rot = rot;
	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		&m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	glBindVertexArray(0);
}

void Mesh::draw(GLuint &program, std::string drawType) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = m_textures[i].type;

		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	glActiveTexture(GL_TEXTURE0);

	glm::mat4 modelMatrix = glm::mat4(1.0f); //Initialise model matrix as a 4x4 identity matrix.


	modelMatrix = glm::translate(modelMatrix, m_pos);

	modelMatrix = glm::scale(modelMatrix, m_scale);

	Win32OpenGL::SendUniformMatrixToShader(program, modelMatrix, "model_matrix"); //Send the model matrix to the shaders.

	glBindVertexArray(m_vao);

	if (drawType == "wireframe") {
		glDrawElements(GL_LINE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, NULL);
}