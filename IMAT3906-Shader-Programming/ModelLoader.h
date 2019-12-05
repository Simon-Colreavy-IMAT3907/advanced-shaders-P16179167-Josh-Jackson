/*! \file ModelLoader.h
*	Loads a model from an OBJ file and returns it as a Model object that can be drawn and transformed.
*/

#pragma once
#include "Model.h"
#include "TextureLoader.h"

class ModelLoader
{
private:
	/*! Given an OBJ file and next line as per the loadfromObj function, parseVertexData checks
	to see if that line within the file is a vertex (v), texture UV (vt) or vertex normal (vn) line.
	If it is, it will grab the 3 floats associated with that v/vn or the 2 floats associated with the vt
	and store them in a vector of unindexed floats.*/
	bool parseVertexData(const char * line, FILE * file);

	/*! Is called when parseVertexData returns false as the next line doesn't match v, vt or vn.
	parseFaceData carries out the same process as parseVertexData but checks to see if the next line
	is an f line. If it is, it will grab the relevant floats from their unindexed vector via the index
	on the f line and then store them as indexed floats inside the final vector.*/
	void parseFaceData(const char * line, FILE * file);

	//The data directly read from the file.
	std::vector<float> unindexedVertices;
	std::vector<float> unindexedUVs;
	std::vector<float> unindexedNormals;

	//The previous data, albeit, indexed for use with OpenGL.
	std::vector<float> finalVertices;
	std::vector<float> finalTextureUVs;
	std::vector<float> finalNormals;

	TextureLoader textureLoader;

public:
	/*! ModelLoader default constructor.*/
	ModelLoader();

	/*! ModelLoader deconstructor.*/
	~ModelLoader();

	/*! Loads a model from an OBJ file by looping through the file and storing all of the
	*	vertices, vertex texture uvs and vertex normals and then looping through the face lines
	*	and indexing the aforementioned values.
	*\param filePath The filepath containing the OBJ file.
	*/
	Model loadFromObj(const char * modelFilePath, string diffuseTextureFilePath, string specularTextureFilePath = "");
};