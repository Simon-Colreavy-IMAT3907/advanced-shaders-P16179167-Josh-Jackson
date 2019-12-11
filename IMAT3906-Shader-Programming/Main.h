/*! \file Main.h
*	Contains the bulk of the application code. Functions from here are called via InitProgram.cpp
*	Main initialises and interfaces with OpenGL via Win32OpenGL.
*/

#pragma once

#define _USE_MATH_DEFINES //!< For M_PI Constant.
#include <cmath>

#include "Win32OpenGL.h" //!< Use OpenGL.
#include "resource.h"
#include "Log.h"
#include "Scene.h"
#include "Camera.h"
#include <chrono>

class Main
{
	Win32OpenGL m_win32OpenGL; //!< OpenGL helper class.

	/*! Camera positions (X, Y, Z)*/
	float m_cameraX{ 0.0f };
	float m_cameraY{ 0.0f };
	float m_cameraZ{ 5.0f };

	/*! Matrices*/
	/*glm::mat4 m_viewMatrix; //!< Contains data for the view.
	glm::mat4 m_projectionMatrix; //!< Contains data for the projection.*/

	float m_aspectRatio; //!< The aspect ratio of the view.

	/*! Lighting vectors, to be sent to the shader.*/
	glm::vec3 m_lightColourSpecular{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_lightColourDiffuse{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_lightColourAmbient{ 0.4f, 0.4f, 0.4f };
	glm::vec3 m_lightPosition{ 0.0f, 0.0f, 0.0f };

	float m_lightMovementSpeed = 20.0f;

	Scene scene; //!< The scene that controls all of the models.
public:
	Main();
	~Main();

	/*! Creates and sets up an OpenGL Context within the Window that was created within InitProgram.*/
	void CreateGLWindow(HWND hWnd, RECT rect);

	/*! Destroys the OpenGL Context.*/
	void DestroyGLWindow();

	/*! Initiates the computation of both the View and Projection matrices. Sends both the
	*	matrices as well as the lighting vectors to the shader and then setups the Scene object
	*	which controls the models within the scene.
	*/
	void PrepareToDraw();

	/*! Clears the OpenGL display and then gets the shader program. It then calls Scene::Render()
	*	which will call the draw functions of any models within the scene.
	*/
	void Draw();

	/*! Calls the Scene::Update() method which will carry out any model transformations and 
	*	other actions that need to occur every tick but aren't directly related to drawing.
	*/
	void Update();

	/*! Receives input from InitProgram and carries out actions related to that input.
	*	For now it handles moving the camera around the scene.
	*/
	void HandleInput(unsigned char virtualKeyCode);

	void HandleMouseInput(int xPosRelative, int yPosRelative);

	void MoveLightPosition(unsigned char key);

	/*! Handles the window being resized and updating OpenGL of the action so it can
	*	compute the correct projection matrix.
	*/
	void Resize(HDC hdc, RECT rect);
};