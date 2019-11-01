#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "Win32OpenGL.h"
#include <iostream>
#include <vector>

class Model
{
  private:
	std::vector<float> m_fVertices; //!< The vertices that make up this model.
	std::vector<float> m_fTextureUVs; //!< The texture UVs that make up this model.
	std::vector<float> m_fNormals; //!M The normals that make up this model.

	GLuint m_vao; //!< VAO Identifier
	GLuint m_vboVertices; //!< Vertices Identifier
	GLuint m_vboTextureUVs; //!< TextureUVs Identifier
	GLuint m_vboNormals; //!< Normals Identifier

	glm::vec3 m_colour; //!< RGB Colour of this model. (if not textured).

	glm::vec3 m_rotation; //!< The x,y,z rotation of this model in degrees.
	glm::vec3 m_postion; //!< The x,y,z position of this model.
	glm::vec3 m_scaleFactor = glm::vec3(1.0f, 1.0f, 1.0f); //!< The scale factor of this model. 1 is default.

	GLuint m_textureDataID; //!< Texture Identifier.

	void loadModel();
	void createVBO();
	void createVAO();

  public:
	Model();

	/*! The constructor for this Model. Stores the passed in data.
	*	\param vertices The vertices this model should use.
	*	\param textureUVs The texture coordinates this model should use.
	*	\param normals The vertex normals this model should use.
	*	\param textureID The texture data identifier this model should reference.
	*/
	Model(std::vector<float> vertices, std::vector<float> textureUVs, std::vector<float> normals, GLuint textureID);

	/*! Sets the colour of this model when it is not textured.
	*	\param r The amount of red.
	*	\param g The amount of green.
	*	\param b The amount of blue.
	*/
	void setColour(float r, float g, float b);

	/*! Sets the position of this model.
	*	\param x The position on the X.
	*	\param y The position on the Y.
	*	\param z The position on the Z.
	*/
	void setPosition(float x, float y, float z);

	/*! Sets the rotation of this model.
	*	\param x The rotation on the X in degrees.
	*	\param y The rotation on the Y in degrees.
	*	\param z The rotation on the Z in degrees.
	*/
	void setRotation(float x, float y, float z);

	/*! Sets the scale factor of this model.
	*	\param x The scale factor on the X.
	*	\param y The scale factor on the Y.
	*	\param z The scale factor on the Z.
	*/
	void setScaleFactor(float x, float y, float z);

	/*! Draws the model and performs any transformations as per the member variable values.
	* Takes a program parameter, taken as a reference to allow multiple models to send
	* their data to the shader using the same program.
	* \param program The shader program identifier.
	*/
	void draw(GLuint &program);

	glm::mat4 m_modelMatrix;
};