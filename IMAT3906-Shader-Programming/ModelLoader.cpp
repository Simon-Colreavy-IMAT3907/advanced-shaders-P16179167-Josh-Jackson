#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
	textureLoader = TextureLoader();
}
ModelLoader::~ModelLoader()
{

}

bool ModelLoader::parseVertexData(const char * line, FILE * file)
{
	/*If the first word of the line is a "v" then the next 3 words on the line will be floats
	representing (x,y,z) of a vertex.*/
	if (strcmp(line, "v") == 0)
	{
		float vertex[3];

		/*Grab the next 3 words on the line and store them in a float array representing the
		x, y and z of a vertex.*/
		fscanf_s(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);

		for (int i = 0; i < 3; i++)
		{
			unindexedVertices.push_back(vertex[i]); //Push the 3 vertices to unindexedVertices.
		}

		return true; //We've found a line that can be parsed.
	}
	/*If the first word of the line is a "vt" then the next 2 words on the line will be floats
	representing (x,y) of a texture uv.*/
	else if (strcmp(line, "vt") == 0)
	{
		float textureUV[2];

		/*Grab the next 2 words on the line and store them in a float array representing
		the x and y of a texture UV.*/
		fscanf_s(file, "%f %f\n", &textureUV[0], &textureUV[1]);

		for (int i = 0; i < 2; i++)
		{
			unindexedUVs.push_back(textureUV[i]); //Push the texture UVs to unindexedUVs.
		}

		return true; //We've found a line that can be parsed.
	}
	/*If the first word of the line is a "vn" then the next 3 words on the line will be floats
	representing (x, y, z) of a vertex normal.*/
	else if (strcmp(line, "vn") == 0)
	{
		float vertexNormal[3];

		/*Grab the next 3 words on the line and store them in a float array representing
		the x, y and z of a vertex normal.*/
		fscanf_s(file, "%f %f %f\n", &vertexNormal[0], &vertexNormal[1], &vertexNormal[2]);

		for (int i = 0; i < 3; i++)
		{
			unindexedNormals.push_back(vertexNormal[i]); //Push the vertexNormals to unindexedNormals.
		}

		return true; //We've found a line that can be parsed.
	}

	return false; //Cannot parse this line as a v, vt or vn line.
}

void ModelLoader::parseFaceData(const char * line, FILE * file)
{
	if (strcmp(line, "f") == 0)
	{
		std::vector<unsigned int> vertexIndex; //Stores the index of each vertex.
		vertexIndex.resize(3);
		std::vector<unsigned int> textureUvIndex; //Stores the index of each texture uv.
		textureUvIndex.resize(3);
		std::vector<unsigned int> vertexNormalIndex; //Stores the index of each vertex normal.
		vertexNormalIndex.resize(3);

		/*Go through an face line ("f") and grab each number in the order of f v/vt/vn and
		store them as indices within the vertex, textureUV and vertexNormal index arrays.
		Also get the number of pieces of data on the line which should be equal to 9 if the
		reader can read this type of file as the layout should follow f 1/1/5 2/2/5 3/3/5*/

		int piecesOfData = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			&vertexIndex.at(0), &textureUvIndex.at(0), &vertexNormalIndex.at(0),
			&vertexIndex.at(1), &textureUvIndex.at(1), &vertexNormalIndex.at(1),
			&vertexIndex.at(2), &textureUvIndex.at(2), &vertexNormalIndex.at(2));


		if (piecesOfData == 9)
		{
			/*Go through each vertex (v) of each triangle.*/
			for (unsigned int i = 0; i < vertexIndex.size(); i++)
			{
				unsigned int index = vertexIndex.at(i) - 1; //The index to vertex position.

				index = 3 * index;

				/*The position of the current vertex in the unindexed vertices is
				the current index in vertexIndices -1 as OBJ indexing starts at 1.*/
				for (int i = 0; i < 3; i++)
				{
					finalVertices.push_back(unindexedVertices.at(index + i));
				}
			}

			/*Go through each vertex texure uv (vt) of each triangle.*/
			for (unsigned int i = 0; i < textureUvIndex.size(); i++)
			{
				unsigned int index = textureUvIndex.at(i) - 1; //The index to the position of the texture UV.

				index = 2 * index;

				/*The position of the current UV in the unindexed UVs is
				the current index in uvIndices -1 as OBJ indexing starts at 1.*/
				for (int i = 0; i < 2; i++)
				{
					finalTextureUVs.push_back(unindexedUVs.at(index + i));
				}
			}


			/*Go through each vertex normal (vn) of each triangle.*/
			for (unsigned int i = 0; i < vertexNormalIndex.size(); i++)
			{
				unsigned int index = vertexNormalIndex.at(i) - 1; //The index to the position of the vertex normal.

				index = 3 * index;

				/*The position of the current normal in the unindexed normals is
				the current index in normalIndices -1 as OBJ indexing starts at 1.*/
				for (int i = 0; i < 3; i++)
				{
					finalNormals.push_back(unindexedNormals.at(index + i));
				}
			}
		}
		else
		{
			//TODO: Split up the f line first, and then assign values. fscanf breaks things when line follows v//vn not v/vt/vn
		}
	}
}

/*! Loads a model from an OBJ file.
*\param filePath The filepath containing the OBJ file.
*/
Model ModelLoader::loadFromObj(const char * modelFilePath, string textureFilePath)
{
	/*Clear the previous model load if using the same instance of ModelLoader
	for multiple models.*/
	finalVertices.clear();
	finalTextureUVs.clear();
	finalNormals.clear();
	unindexedNormals.clear();
	unindexedUVs.clear();
	unindexedVertices.clear();

	FILE * file;
	fopen_s(&file, modelFilePath, "r"); //Open the file at filePath in read mode.

	if (file == NULL)
	{
		throw std::invalid_argument("OBJ file not found");
	}

	/*Read each line of the file until end of file is reached.*/
	while (1)
	{
		char line[128]; //Assumes first word of a line isn't longer than 128 characters. (TODO: CHANGE)

		int res = fscanf_s(file, "%s", line, 128); //Read the first word of the line.

		if (res == EOF) //If we've reached the end of the file, exit the loop.
		{
			break;
		}

		/*If the next line is v, vt or vn, parse it and add it to the relevant unindexed vector.
		If the next line isn't a v, vt or vn line, check if it is an f line, and if so parse it and
		store the result in the final vector[s].*/
		if (!parseVertexData(line, file))
		{
			parseFaceData(line, file);
		}
	}

	GLuint textureID;

	textureLoader.LoadBMP(textureFilePath, textureID, false);

	return Model(finalVertices, finalTextureUVs, finalNormals, textureID); //could pass tex data as pointer/ref?
}

