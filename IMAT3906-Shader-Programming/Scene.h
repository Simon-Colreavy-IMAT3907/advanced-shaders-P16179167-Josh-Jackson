/*! \file Scene.h
*	Controls all of the Models in a scene including their drawing and transformations.
*/

#pragma once
#include "ModelLoader.h"
#include "Model.h"
#include "Camera.h"
#include <map>

class Scene
{
private:
	/*! Models should be loaded via m_modelLoader in here.*/
	void loadModels();

	/*! Model::setPosition() and Model::setRotation() to be called on models in here when
	only happening once. Constant postion/rotation changes should be placed in Update()*/
	void setModelTransformations();

	std::vector<Model> models;
	std::map <std::string, std::string> fileNames;

	void loadExternalScene(const char * filePath);
public:
	/*! Scene default constructor.*/
	Scene();

	/*!Scene deconstructor.*/
	~Scene();

	/*! The Start function gets called once when Main is preparing to be drawn. Anything
	*	that only needs to happen once, such as creating a Model should occur here.
	*/
	void Start();

	void keyIsPressed(unsigned char key);

	void mouseIsMoved(int xPos, int yPos);

	/*! The Update function runs every tick and is used to manage Model transformations 
	*	and other actions that need to occur every tick that aren't directly related
	*	to the drawing of a model.
	*/
	void Update();

	/*! The Render function calls the draw function of every Model being used
	*	in the scene.
	*/
	void Render(GLuint &program);

	ModelLoader m_modelLoader;

	Camera camera;

	float m_cameraMovementSpeed = 10.0f;
};