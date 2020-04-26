/*---------------------------------------------------------*/
/* ----------------   Práctica 9 --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*------------- Alumno:                     ---------------*/
//#define STB_IMAGE_IMPLEMENTATION

//Para texturas, modelos, skybox
//../../FinalGrafica/Texturas
//../../FinalGrafica/Models
//../../FinalGrafica/SkyBox

//FinalGrafica <- REPO (Texturas, Models, Skybox)
//ProyectoFinal <- Solución de Visual Studio

#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

#include "../ProyectoFinal/ProyectoFinal/camera.h"
#include "../ProyectoFinal/ProyectoFinal/Model.h"
#include "../ProyectoFinal/ProyectoFinal/Texture.h"

// Other Libs
#include "../ProyectoFinal/ProyectoFinal/SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint skyboxVBO, skyboxVAO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, 3.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

void myData(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//Texture
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_panda,
				t_cubo,
				t_caja,
				t_caja_brillo;

//For model
bool animacion = false;
float movAuto_z = 0.0f;
bool avanza = true;

//Para movimiento y escalado de modelos (registro de coordenadas y escalas)
float movX = 0.0f, movY = 3.0f, movZ = 0.0f, escala = 1.0f;
bool preciso = false;

//Para escalamiento de todo
float a = 0.2f;


unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	
}

void myData()
{	
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	// SkyBox
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

}

void animate(void)
{
	if (animacion)
	{
		movAuto_z += 0.03f;

		printf("Posicion %f \n", movAuto_z);
	}
}

void display(Shader shader, Shader skyboxShader, GLuint skybox, Model modelo[])
{
	shader.use();

	//Setup Advanced Lights
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("pointLight[0].position", lightPosition);
	shader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));	//Luz ambiental de 0 a 1
	shader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointLight[0].constant", 1.0f); 
	shader.setFloat("pointLight[0].linear", 0.001f);  
	shader.setFloat("pointLight[0].quadratic", 0.000032f); 

	shader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.009f);						//Luz direccional
	shader.setFloat("pointLight[1].quadratic", 0.032f);						//Luz quién sabe
	
	shader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(290.0f, 290.0f, 1.0f));
	shader.setMat4("model", model);
	modelo[0].Draw(shader);

	//model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
	model = glm::scale(model, glm::vec3(escala, escala, escala));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Para casa4
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	/**
	model = glm::translate(tmp, glm::vec3(0.85f, 0.25f, 1.29f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Izq delantera
	*/


	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 9", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);
	
	//For Models
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	//For Primitives
	//Shader primitivasShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	// Load model
	Model modelo[47] = {
		((char *)"../../FinalGrafica/Models/bocetoMedidas.obj"),							//0 - Boceto, piso
		((char *)"../../FinalGrafica/Models/Basic House 1/Basic House 1.obj"),				//1 - Casita de prueba (reservado pa cualquier cosa)

		//De 2 a 16 sigue Mildred...
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),

		//17 empieza Joya hasta 31...
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),

		//De 32 a 46 Chavira...
		((char *)"../../FinalGrafica/Models/casita/dom.obj"),				//32
		((char *)"../../FinalGrafica/Models/casa2/untitled2.obj"),				//33
		((char *)"../../FinalGrafica/Models/casa3/untitled.obj"),				//34
		((char *)"../../FinalGrafica/Models/casa4/Cyprys_House.obj"),				//35
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//36
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//37
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//38
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//39
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//40
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//41
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//42
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//43
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//44
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//45
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//46
	};


	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("../../FinalGrafica/SkyBox/right.tga");
	faces.push_back("../../FinalGrafica/SkyBox/left.tga");
	faces.push_back("../../FinalGrafica/SkyBox/top.tga");
	faces.push_back("../../FinalGrafica/SkyBox/bottom.tga");
	faces.push_back("../../FinalGrafica/SkyBox/back.tga");
	faces.push_back("../../FinalGrafica/SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
    
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 600.0f);
	// render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        //my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel);
		display(modelShader, SkyBoxshader, cubemapTexture, modelo);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPosition.z -=0.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPosition.z += 0.5f;


	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		if (preciso)
			movY += 0.1f;
		else
			movY += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		if (preciso)
			movY -= 0.1f;
		else
			movY -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		if (preciso)
			movX -= 0.1f;
		else
			movX -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		if (preciso)
			movX += 0.1f;
		else
			movX += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		if (preciso)
			movZ += 0.1f;
		else
			movZ += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		if (preciso)
			movZ -= 0.1f;
		else
			movZ -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		if (preciso)
			escala -= 0.1f;
		else
			escala -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		if (preciso)
			escala += 0.1f;
		else
			escala += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		//animacion = true;
		preciso = !preciso;

	printf("Posicion: %f, %f, %f \tEscala: %f\n", movX, movY, movZ, escala);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}