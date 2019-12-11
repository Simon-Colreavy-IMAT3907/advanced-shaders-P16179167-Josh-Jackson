#include "Scene.h"
#include "cmath"

Scene::Scene()
{
	camera = Camera(glm::vec3(0, 60, 280), glm::vec3(0, 0, 0));
}
Scene::~Scene()
{

}

/*! Loads all of the models for the scene. Either via the loadExternalScene()
method which reads an external scene file or manually via ModelLoader::loadFromObj()*/
void Scene::loadModels()
{
	loadExternalScene("./testScene.txt");
}

/*! Called when a key is pressed via Main, receives the code of the
key that was pressed.*/
void Scene::keyIsPressed(unsigned char key)
{
	if (key == 0x57)
	{
		camera.move(Direction::forwards, m_cameraMovementSpeed);
	}
	else if (key == 0x53)
	{
		camera.move(Direction::backwards, m_cameraMovementSpeed);
	}

	if (key == 0x41)
	{
		camera.move(Direction::left, m_cameraMovementSpeed);
	}
	else if (key == 0x44)
	{
		camera.move(Direction::right, m_cameraMovementSpeed);
	}
}

/*! Called via Main when the mouse is moved. Receives the change
in x and y on the screen.*/
void Scene::mouseIsMoved(int xPos, int yPos)
{
	camera.rotate((float)xPos, -((float)yPos));
}

/*! Used to set the position, scale and rotation of models that are
created within the code here and not from an external scene file.*/
void Scene::setModelTransformations()
{
}

/*! The Start function gets called once when Main is preparing to be drawn. Anything
*	that only needs to happen once, such as creating a Model should occur here.
*/
void Scene::Start()
{
	loadModels();
	setModelTransformations();
}

/*! The Update function runs every tick and is used to manage Model transformations
*	and other actions that need to occur every tick that aren't directly related
*	to the drawing of a model.
*/
void Scene::Update()
{

}

/*! The Render function calls the draw function of every Model being used
*	in the scene.*/
void Scene::Render(GLuint &program)
{
	for (int i = 0; i < models.size(); i++)
	{
		models.at(i).draw(program);
	}
	
	//Send the camera's view matrix to the shader.
	Win32OpenGL::SendUniformMatrixToShader(program, camera.viewMatrix, "view_matrix");
}


void Scene::loadExternalScene(const char * filePath)
{
	models = std::vector<Model>();
	fileNames = std::map<std::string, std::string>();

	FILE * file;
	fopen_s(&file, filePath, "r"); //Open the file at filePath in read mode.

	if (file == NULL)
	{
		throw std::invalid_argument("Scene file doesn't exist.");
	}

	/*Read each line of the file until end of file is reached.*/
	while (1)
	{
		char line[128]; //Assumes first word of a line isn't longer than 512 characters.

		int res = fscanf_s(file, "%s", line, 128); //Read the first word of the line.

		if (res == EOF) //If we've reached the end of the file, exit the loop.
		{
			break;
		}

		if (strcmp(line, "f") == 0) //A file reference has been found.
		{
			char name[128];
			char path[128];

			fscanf_s(file, "%s %s\n", &name, 128, &path, 128);

			fileNames.insert(std::pair<string, string>(std::string(name), std::string(path)));
		}
		else if (strcmp(line, "m") == 0)
		{
			glm::vec3 pos;
			glm::vec3 scale;
			glm::vec3 rot;

			char modelName[128];
			char drawType[128];

			int n = fscanf_s(file, "%s %f/%f/%f %f/%f/%f %f/%f/%f %s\n", &modelName, 128,
				&pos.x, &pos.y, &pos.z, &scale.x, &scale.y, &scale.z,
				&rot.x, &rot.y, &rot.z, &drawType, 128);

			Model model = Model(fileNames.at(modelName).c_str(), pos, scale, rot, std::string(drawType));

			/*Model model = m_modelLoader.loadFromObj(fileNames.at(std::string(modelName)).c_str(), fileNames.at(std::string(diffuseTextureName)), fileNames.at(std::string(specularTextureName)), fileNames.at(std::string(normalTextureName)));
			model.setPosition(pos.x, pos.y, pos.z);
			model.setScaleFactor(scale.x, scale.y, scale.z);
			model.setRotation(rot.x, rot.y, rot.z);*/

			models.push_back(model);

		}
	}
}