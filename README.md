# IMAT3906 - Advanced Shader Programming Coursework
## P16179167 - Joshua Jackson

### GLSL Code can be found in the `IMAT3906-Shader-Programming\Shaders` folder where the `BlinnPhong.vert` and `BlinnPhong.frag` are the files being used in the demo


### Textures and Models
The application needs to be run in `x86` mode and you will need all of the texture files and models which aren't included due to their filesize, please contact me at `p16179167@my365.dmu.ac.uk` to get access to these files so the application will run. Alternatively, use your own files and put them in the respective files.

### External Scene Loading
There is a `testScene.txt` file in the root of the project that decides how the scene should be laid out.

### External Dependencies
The project uses Assimp to load models and uses glm for opengl maths.

If you cannot get setup, you can download a full working version of the project at the following link: https://drive.google.com/open?id=1DY66OTbkxkLKWrn3UZZ00RKyejOuxLk9


### How the project works
The project sets up opengl through the use of win32 in the Main.cpp file, in this file it also sends all of the uniforms to the shader (except for the model matrix).

Models are loaded through the use of the Model.cpp file in the Scene.cpp where an external scene file is read into the program containing the file paths of the models and where to position those models in the scene.

There are two shader files BlinnPhong.vert and BlinnPhong.frag that implement blinn-phong lighting as well as specular and diffuse mapping and normal and parralax mapping.