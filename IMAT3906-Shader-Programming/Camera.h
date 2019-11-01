#pragma once

#define _USE_MATH_DEFINES //!< For M_PI Constant.
#include "Win32OpenGL.h"

enum class Direction {forwards, left, right, backwards};

class Camera
{
	private:

		 glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 5.0f); //!< The position of the Camera in world space.
		 glm::vec3 m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //!< What the Camera should be looking at.
		 glm::vec3 m_cameraDir; //!< The inverse direction the camera is facing from its target.

		 glm::vec3 m_cameraUp; //!< The up vector of this camera.
		 //glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		 glm::vec3 m_cameraRight; //!< The right vector of this camera.

		 void calculateDirections(); //!< Calculates the inverse dir, up and right vectors of this camera.

		 float pitch = 0; //!< The angle of rotation, in degrees, on the Y axis.
		 float yaw = -80; //!< The angle of rotation, in degrees, on the X axis.

		 glm::vec3 lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);

	public:
		/*!
			The default constructor. To be used if no specific position or target is desired.
			Will default camera position to (0, 0, -5) and the target to (0, 0, 0).
		*/
		Camera();

		/*!
		*	The constructor for this camera.
		*	\param pos The position of the camera.
		*	\param target What this camera will be looking at.
		*/
		Camera(glm::vec3 pos, glm::vec3 target);

		inline glm::vec3 getCameraPos() { return m_cameraPos; }

		float m_aspectRatio; //!< The aspect ratio of this projection matrix.

		float m_sensitivity = 10;

		glm::mat4 viewMatrix; //!< The view matrix.
		glm::mat4 projectionMatrix; //!< The projection matrix.

		float m_drawDistance = 1600.0f; //!< Far clipping plane.

		/*! 
			Computes the view matrix using this camera's direction vectors and glm::lookAt() and stores it
			inside of viewMatrix
		*/
		void computeViewMatrix();

		/*! Computes a projection matrix and stores it as a glm::mat4 inside of projectionMatrix*/
		void computeProjectionMatrix();

		void move(Direction dir, float speed);

		void rotate(float x, float y);
};