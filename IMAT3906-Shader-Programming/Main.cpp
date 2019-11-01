#include "stdafx.h"
#include "Main.h"
#include "Model.h"


Main::Main()
{
	scene = Scene(); //!< Create an empty scene passing in the shader program.
}

Main::~Main()
{
}

/*! Creates and sets up an OpenGL Context within the Window that was created within InitProgram.*/
void Main::CreateGLWindow(HWND hWnd, RECT rect)
{
	m_win32OpenGL.CreateGLContext(hWnd); //Creates an OpenGL context in the current window received from InitProgram

	scene.camera.m_aspectRatio = static_cast<float>(rect.right / rect.bottom); //Sets up the aspect ratio.

	m_win32OpenGL.CreateShadersAndProgram("phongTexture2"); //Setup the shader program with the given shader file[s].

	m_win32OpenGL.SetupDisplay();

	glEnable(GL_BLEND);


	m_win32OpenGL.GetError(); //Checks everything is ok.
}

/*! Destroys the OpenGL Context.*/
void Main::DestroyGLWindow()
{
	m_win32OpenGL.TearDownGLContext();
}

/*! Initiates the computation of both the View and Projection matrices. Sends both the
*	matrices as well as the lighting vectors to the shader and then setups the Scene object
*	which controls the models within the scene.
*/
void Main::PrepareToDraw()
{
	glClearColor(0, 92, 103, 255);
	//View and Projection Matrices are computed in Camera class when it is constructed.
	GLuint program = m_win32OpenGL.GetShaderProgram(); //Get the shader program.

	/*Send the both the projection and view matrices as well as the lighting vectors to the shader.*/

	/*  -----------------------------------------------------
		| How this all works using a very basic flat shader |
		-----------------------------------------------------

		Say we have a shader (vertex and fragment (also called pixel)) called flat.vert and flat.frag respectively

		The Vertex Shader manipulates the attributes of vertices whereas the fragment shader takes care of how the
		pixels between the verticies look.

		
		flat.vert
		----------
		// tell the shader where the vertices are in the vao
		layout(location = 0) in vec3 vertex_position;

		uniform mat4 projection_matrix, view_matrix, model_matrix;

		void main() {
			gl_Position = projection_matrix * view_matrix * model_matrix * vec4 (vertex_position, 1.0);
		}
		-----------

		In the above vertex shader, we first start by telling the shader where the vertices of our model
		are in the VAO (The models are setup in Scene.start() after all of our other data is sent to the
		shader).

		We then say that we want the shader to be given 3 different matrices. The projection_matrix, the view_matrix
		and the model_matrix. We send these to the shader by calling:

		---------
		Win32OpenGL::SendUniformMatrixToShader(program, scene.camera.projectionMatrix, "projection_matrix");
		Win32OpenGL::SendUniformMatrixToShader(program, scene.camera.viewMatrix, "view_matrix");
		----------

		Notice the names at the end are the exact same. We get our matrices from our scene camera (abstracted away).

		Our code then sets the glPosition based on the matrices we sent it.

		flat.frag
		----------
		uniform vec3 surface_colour;

		out vec4 frag_colour;

		void main() {
			frag_colour = vec4 (surface_colour, 1.0);
		}
		----------

		In our fragment shader, we are saying that we expect a vector 3 called surface_colour to be passed in
		and then we set the fragment colour to that surface colour (each pixel in between vertices).

		Our surface_colour is sent to the shader in the Model drawing process
	*/

	//Positional Matrices
	Win32OpenGL::SendUniformMatrixToShader(program, scene.camera.projectionMatrix, "projection_matrix");
	Win32OpenGL::SendUniformMatrixToShader(program, scene.camera.viewMatrix, "view_matrix");


	//Lighting Vectors (This is for the more advanced shaders)
	Win32OpenGL::SendUniformVector3ToShader(program, m_lightPosition, "light_position_world");
	Win32OpenGL::SendUniformVector3ToShader(program, m_lightColourSpecular, "light_colour_specular");
	Win32OpenGL::SendUniformVector3ToShader(program, m_lightColourDiffuse, "light_colour_diffuse");
	Win32OpenGL::SendUniformVector3ToShader(program, m_lightColourAmbient, "light_colour_ambient");

	scene.Start(); //Start the Scene.
}

/*! Clears the OpenGL display and then gets the shader program. It then calls Scene::Render()
*	which will call the draw functions of any models within the scene.*/
void Main::Draw()
{
	m_win32OpenGL.ClearGLDisplay(); //Clear the OpenGL Display.

	GLuint program = m_win32OpenGL.GetShaderProgram(); //Get the shader program.

	scene.Render(program); //Draw all models in the scene.

	m_win32OpenGL.FinishedDrawing();
}

/*! Calls the Scene::Update() method which will carry out any model transformations and
*	other actions that need to occur every tick but aren't directly related to drawing.*/
void Main::Update()
{
	scene.Update(); //Update the scene.
}

/*! Receives input from InitProgram and carries out actions related to that input.
*	For now it handles moving the camera around the scene.*/
void Main::HandleInput(unsigned char virtualKeyCode)
{
	scene.keyIsPressed(virtualKeyCode);

	MoveLightPosition(virtualKeyCode);
}

void Main::MoveLightPosition(unsigned char key)
{
	bool keyHasBeenPressed = false;

	if (key == 0x25) //Left Arrow
	{
		m_lightPosition = glm::vec3{ m_lightPosition.x - m_lightMovementSpeed, m_lightPosition.y, m_lightPosition.z };
		keyHasBeenPressed = true;
	}
	
	if (key == 0x27) //Right Arrow
	{
		m_lightPosition = glm::vec3{ m_lightPosition.x + m_lightMovementSpeed, m_lightPosition.y, m_lightPosition.z };
		keyHasBeenPressed = true;
	}

	if (key == 0x26) //Up Arrow
	{
		m_lightPosition = glm::vec3{ m_lightPosition.x, m_lightPosition.y + m_lightMovementSpeed, m_lightPosition.z };
		keyHasBeenPressed = true;
	}

	if (key == 0x28) //Down Arrow
	{
		m_lightPosition = glm::vec3{ m_lightPosition.x, m_lightPosition.y - m_lightMovementSpeed, m_lightPosition.z };
		keyHasBeenPressed = true;
	}

	if (keyHasBeenPressed)
		Win32OpenGL::SendUniformVector3ToShader(m_win32OpenGL.GetShaderProgram(), m_lightPosition, "light_position_world");
}

void Main::HandleMouseInput(int xPosRelative, int yPosRelative)
{
	scene.mouseIsMoved(xPosRelative, yPosRelative);
}

/*! Handles the window being resized and updating OpenGL of the action so it can
*	compute the correct projection matrix.*/
void Main::Resize(HDC hdc, RECT rect)
{
	// if the display is resized then OpenGL needs to know about the apect ratio
	// to compute the correct projection matrix
	m_win32OpenGL.Reshape(hdc, rect.right, rect.bottom);
	scene.camera.m_aspectRatio = (float)rect.right / rect.bottom;
	scene.camera.computeProjectionMatrix();
	GLuint program = m_win32OpenGL.GetShaderProgram();
	Win32OpenGL::SendUniformMatrixToShader(program, scene.camera.projectionMatrix, "projection_matrix");
}