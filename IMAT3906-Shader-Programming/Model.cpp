#include "stdafx.h"
#include "Model.h"

Model::Model()
{
	m_rotation = glm::vec3(0, 0, 0);
	m_postion = glm::vec3(0, 0, 0);
}

Model::Model(std::vector<float> vertices, std::vector<float> textureUVs, std::vector<float> normals, GLuint textureID)
{
	m_fVertices = vertices;
	m_fTextureUVs = textureUVs;
	m_fNormals = normals;

	m_textureDataID = textureID;

	loadModel();
}

void Model::createVBO()
{
	//*Create the Vertex Buffer Object (VBO)*/
	glGenBuffers(1, &m_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glBufferData(GL_ARRAY_BUFFER, m_fVertices.size() * sizeof(float), &m_fVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_vboTextureUVs);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboTextureUVs);
	glBufferData(GL_ARRAY_BUFFER, m_fTextureUVs.size() * sizeof(float), &m_fTextureUVs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glBufferData(GL_ARRAY_BUFFER, m_fNormals.size() * sizeof(float), &m_fNormals[0], GL_STATIC_DRAW);
}

void Model::createVAO()
{
	/*Create the Vertex Array Object (VAO)*/
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboTextureUVs);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0); //Vertices are element 0 in the VAO.
	glEnableVertexAttribArray(1); //Textures are element 1 in the VAO.
	glEnableVertexAttribArray(2); //Normals are element 2 in the VAO.
}

/**Creates a VBO using the member vector m_vertices and then creates a VAO using that previously created VBO.*/
void Model::loadModel()
{
	createVBO();
	createVAO();
}	 

void Model::setColour(float r, float g, float b)
{
	m_colour = { r, g, b };
}

void Model::setPosition(float x, float y, float z)
{
	m_postion = glm::vec3(x, y, z);
}

/**Change the angle by which the model will be rotated every Model::draw() call.
* The default sits at 0. Optional parameter degrees will take the angle[s] in
* degrees instead of radians if true. False default.
*/
void Model::setRotation(float x, float y, float z)
{
	m_rotation = glm::vec3(x, y, z);
}


void Model::setScaleFactor(float x, float y, float z)
{
	m_scaleFactor = glm::vec3(x, y, z);
}

/*! Draws the model and performs any transformations as per the member variable values.
* Takes a program parameter, taken as a reference to allow multiple models to send
their data to the shader using the same program.
*/
void Model::draw(GLuint &program)
{
	Win32OpenGL::SendUniformVector3ToShader(program, m_colour, "surface_colour"); //Let the shaders alter this colour.
	
	glBindTexture(GL_TEXTURE_2D, m_textureDataID);

	m_modelMatrix = glm::mat4(1.0f); //Initialise model matrix as a 4x4 identity matrix.
	

	m_modelMatrix = glm::translate(m_modelMatrix, m_postion);
	

	/*Rotate the model matrix by the x, y and z Rot variables.*/
	if (m_rotation.x > 0)m_modelMatrix = glm::rotate(m_modelMatrix, (float)glm::radians((float)m_rotation.x), glm::vec3(1, 0, 0));
	
	if (m_rotation.y > 0)m_modelMatrix = glm::rotate(m_modelMatrix, (float)glm::radians((float)m_rotation.y), glm::vec3(0, 1, 0));
	
	if (m_rotation.z > 0)m_modelMatrix = glm::rotate(m_modelMatrix, (float)glm::radians((float)m_rotation.z), glm::vec3(0, 0, 1));

	m_modelMatrix = glm::scale(m_modelMatrix, m_scaleFactor);
	
	Win32OpenGL::SendUniformMatrixToShader(program, m_modelMatrix, "model_matrix"); //Send the model matrix to the shaders.
	
	glBindVertexArray(m_vao); //Bind VAO
	GLuint numberOfElements = m_fVertices.size() / 3;
	glDrawArrays(GL_TRIANGLES, 0, numberOfElements); //Draw VAO
	glBindVertexArray(NULL); //Unbind VAO
	glBindTexture(GL_TEXTURE_2D, NULL);
}