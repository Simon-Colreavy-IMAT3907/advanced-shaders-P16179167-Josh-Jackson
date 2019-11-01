#include "Camera.h"

Camera::Camera()
{
	calculateDirections();
	computeViewMatrix();
	computeProjectionMatrix();
}

Camera::Camera(glm::vec3 pos, glm::vec3 target)
{
	m_cameraPos = pos;
	m_cameraTarget = target;

	calculateDirections();
	computeViewMatrix();
	computeProjectionMatrix();
}

void Camera::calculateDirections()
{
	//The direction of the camera to the target. This is the reverse of that.
	m_cameraDir = glm::normalize(m_cameraPos - m_cameraTarget);

	/*We work out the right vector for the camera by finding the cross product
	between our camera direction and up (0, 1, 0). This produces a vector
	that is perpendicular to both of those vectors.*/
	m_cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_cameraDir));

	/*To find the cameras positive y axis we take the cross product of our cameraRight vector
	and our cameraDir vector.*/
	m_cameraUp = glm::normalize(glm::cross(m_cameraDir, m_cameraRight));
}

void Camera::computeViewMatrix()
{
	/*Using a transformation matrix and the directions of the camera defined within the constructor,
	the lookAt function will transform any vector to the camera's coordinate space.*/

	//lookAt Transformation Matrix

	// [Rx Ry Rz 0]		[1 0 0 -Px]
	// [Ux Uy Uz 0]		[0 1 0 -Py]
	//               *	
	// [Dx Dy Dz 0]		[0 0 1 -Pz]
	// [0  0  0  1]		[0 0 0   1]

	/*
	  R = m_cameraRight
	  U = m_cameraUp
	  D = m_cameraDir
	  P = m_cameraPos

	  The camera position is inverted because the world is translated in the opposite direction that the 
	  camera will be moving.
	*/

	/*Work out the angle of rotation using trig on the x, y and z according to the pitch and yaw values.*/
	lookDirection.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	lookDirection.y = sin(glm::radians(pitch));
	lookDirection.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + glm::normalize(lookDirection), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::computeProjectionMatrix()
{
	// we will look at this later in the course
	// in Modern OpenGL we must supply a projection matrix
	const float oneDegreeInRadians = static_cast<float>((2.0 * M_PI) / 360.0);

	//ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

	// input variables
	float zNear = 0.1f;		// clipping plane
	float zFar = m_drawDistance;	// clipping plane
	float fov = static_cast<float>(67.0f * oneDegreeInRadians); // convert 67 degrees to radians

	float range = tan(fov * 0.5f) * zNear;
	float Sx = (2.0f * zNear) / (range * m_aspectRatio + range * m_aspectRatio);
	float Sy = zNear / range;
	float Sz = -(zFar + zNear) / (zFar - zNear);
	float Pz = -(2.0f * zFar * zNear) / (zFar - zNear);

	GLfloat proj_mat[] = 
	{
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};

	projectionMatrix = glm::make_mat4(proj_mat);
}

void Camera::move(Direction dir, float speed)
{
	if (dir == Direction::forwards)
	{
		m_cameraPos += speed * lookDirection;
	}
	else if (dir == Direction::backwards)
	{
		m_cameraPos -= speed * lookDirection;
	}

	if (dir == Direction::right)
	{
		m_cameraPos += glm::normalize(glm::cross(lookDirection, m_cameraUp)) * speed;
	}
	else if (dir == Direction::left)
	{
		m_cameraPos -= glm::normalize(glm::cross(lookDirection, m_cameraUp)) * speed;
	}

	computeViewMatrix();
}

void Camera::rotate(float x, float y)
{
	yaw += x / m_sensitivity;
	pitch += y / m_sensitivity;

	if (pitch > 89.0f) //Stop the camera from rotating up and around 360 degrees.
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	computeViewMatrix();
}