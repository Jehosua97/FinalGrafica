/*-------------------------------------------------------*/
/* ----------------   Proyecto Final --------------------*/
/*--------------------    2020-2   ----------------------*/
/*------- Alumno:                              ----------*/
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
#include <math.h>

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
float movX = 0.0f, movY = 0.0f, movZ = 0.0f, escala = 1.0f, rotacion = 0.0f, rotacionY = 0.0f;
bool preciso = false;
float incPreciso = 0.1f;
float incNormal = 1.0f;
float incRotacion = 90.0f;

//Para cambiar de plano
int valorPlano = 1;


//VARIABLES DE ANIMACIÓN
//Perro
float rotaPata = 0.0f, rotaColita = 0.0f;
bool animacionPerro = false;
float movPerroX = 35.0f, movPerroY = -1.0f, movPerroZ = -65.0f, gradosPerro = 0.0f;
float iniPerroX = -25.0f, iniPerroY = -1.0f, iniPerroZ = -86.0f;
float elipseX = 0.0f, elipseZ = 0.0f;
float anguloPerro = 0.0f, incGradosPerro = 1.0f;
bool estadosPerro[10] = { true };
bool alternaPatas = true;
float escalaPerro = 0.35f;
float incRotacionPerro = 7.5f;
bool pausaPatas = false;
float velMovPerro = 0.5f;

//temporal
int ba = 37;

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
	if (animacionPerro) {

		//iniPerroX = -25.0f, iniPerroY = -1.0f, iniPerroZ = -86.0f;
		//La elipse inicia en x = 35.0f, y = -1.0f, z = -65.0f
		if (estadosPerro[0]) {
			elipseX = (1 + cos(glm::radians(gradosPerro)));
			elipseZ = (0.7 + (0.7 * sin(glm::radians(gradosPerro))));
			movPerroX = iniPerroX + elipseX * 30;
			movPerroZ = iniPerroZ + elipseZ * 30;
			gradosPerro = (float)((int)(gradosPerro + incGradosPerro) % 360);
			if (gradosPerro == 0.0f) {
				estadosPerro[0] = false;
				estadosPerro[1] = true;
			}
		}
	
		//Se detiene frente a la casa:
		//	Posicion: 36.999992, -1.100000, -24.000017
		if (estadosPerro[1]) {
			movPerroZ += velMovPerro;
			if (movPerroZ > -24.0f) {
				estadosPerro[1] = false;
				estadosPerro[2] = true;
				gradosPerro += 90.0f; //El valor ya se niega en el modelo
			}
		}

		//Se detiene antes de dar el salto en:
		//	Posicion: 28.999992, -1.100000, -24.000017 
		if (estadosPerro[2]) {
			movPerroX -= velMovPerro;
			if (movPerroX < 30.0f) {
				estadosPerro[2] = false;
				estadosPerro[3] = true;
				pausaPatas = true;
			}
		}

		//Cae en:
		//	Posicion: 17.999992, -1.100000, -24.000017 
		if (estadosPerro[3]) {
			movPerroX -= velMovPerro;
			movPerroY = -0.04f * pow(movPerroX - 24, 2) + 1.5f;
			//Ecuación Parábola
			if (movPerroX < 18.0f) {
				estadosPerro[3] = false;
				estadosPerro[4] = true;
				pausaPatas = false;
				movPerroY = -1.0f;
			}
		}

		//Camina hacia:
		//	Posicion: 13.999992, -1.100000, -24.000017 
		if (estadosPerro[4]) {
			movPerroX -= velMovPerro;
			if (movPerroX < 14.0f) {
				estadosPerro[4] = false;
				estadosPerro[5] = true;
				gradosPerro += 90.0f + 27.1213f;	//tan^-1 (1.7826) = 62.8786... -90 ~= 27.1213
													//El valor ya se niega en el modelo
			}
		}
		//Atraviesa hacia:
		//	Posicion: 34.899986, -1.100000, -24.000017 
		//Sube adonde empezó:
		//	x = 35.0f, y = -1.0f, z = -65.0f
		if (estadosPerro[5]) {
			movPerroX += velMovPerro * 0.5121;
			movPerroZ -= velMovPerro;	//1.95 veces /\ X
			if (movPerroX > 35.0f) {
				estadosPerro[5] = false;
				estadosPerro[0] = true;
				gradosPerro = 0.0f;
			}
		}


		
	}

	if (!pausaPatas) {
		if (alternaPatas) {
			if (rotaPata < 45.0f)
				rotaPata += incRotacionPerro;
			else
				alternaPatas = false;
			if (rotaColita < 20.0f)
				rotaColita += incRotacionPerro * 1.2;
		}
		else
		{
			if (rotaPata > -20.0f)
				rotaPata -= incRotacionPerro;
			else
				alternaPatas = true;
			if (rotaColita > -20.0f)
				rotaColita -= incRotacionPerro * 1.2;
		}
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


	//PLANO DE REFERENCIA

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.09f, 0.0f));
	//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(290.0f, 1.0f, 290.0f));
	shader.setMat4("model", model);
	modelo[valorPlano].Draw(shader);

	/*
	32
	Escala: 1.2f en todas
	Pos Y: -1.0f

	33
	Escala: 1.0f en todas, o comentado
	Pos Y: -0.5f

	34
	Escala: 1.5f en todas
	Pos Y: -1.0f

	35
	Escala: 1.3f en todas
	Pos Y: -1.0f
	*/

	//CASAS DE ARRIBA - JEHOSUA
	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));				//Ejemplo.
	model = glm::scale(model, glm::vec3(escala, escala, escala));						//Utilizar sólo un modelo con las variables por vez.
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);*/


	model = glm::translate(glm::mat4(1.0f), glm::vec3(-46.0f, -0.5f, -121.0f));				//1
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -120.0f));				//2
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	//model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(46.0f, -1.0f, -121.0f));				//3
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	//model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.0f, -1.0f, -88.8f));				//4
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, -1.0f, -92));				//5
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(22.0f, -0.5f, -94.0f));				//6
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(69.0f, -1.0f, -95.0f));				//7
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-51.0f, -1.0f, -74.0f));				//8
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.0f, -68.0f));				//9
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-72.0f, -0.5f, -44.0f));				//10
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	//CASAS DE EN MEDIO - MILDRED
	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));				//Ejemplo.
	model = glm::scale(model, glm::vec3(escala, escala, escala));						//Utilizar sólo un modelo con las variables por vez.
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);*/

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 20.9f));				//17
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	//model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.59f, -1.0f, 21.3f));				//16
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(73.6f, -0.5f, 44.6f));				//20
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.2f, -1.0f, 21.0f));				//18
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-68.3f, -1.0f, 50.5f));				//19
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.4f, -1.0f, 0.20f));				//14
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(68.8f, -1.0f, -6.3f));				//15
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(74.1f, -1.0f, -44.4f));				//11
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.9f, -1.0f, -29.5f));				//12
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.9f, -0.5f, -23.5f));				//13
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	//CASAS DE ABAJO - CHAVIRA
	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));				//Ejemplo.
	model = glm::scale(model, glm::vec3(escala, escala, escala));						//Utilizar sólo un modelo con las variables por vez.
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);*/

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.1f, -0.5f, 65.0f));			//21
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));		
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 64.0f));				//22
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(50.1f, -1.0f, 71.5f));				//23
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.6f, -1.0f, 88.2f));				//24
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -1.0f, 90.0f));				//25
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(22.1f, -1.0f, 89.2f));				//26
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[32].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(74.3f, -1.0f, 88.8f));				//27
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[34].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.8f, -0.5f, 119.0f));				//28
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 113.8f));				//29
	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[35].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.2f, -0.5f, 119.0f));				//30
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[33].Draw(shader);

	//ÁRBOLES

	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, -0.7f, movZ));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, -1.1f, movZ));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);

	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, -1.1f, movZ));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader); 

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, -1.0f, movZ));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);*/

	
	//modelo[18].Draw(shader);*/

	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, -1.1f, movZ));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);*/


	model = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.1f, -12.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.1f, -6.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.1f, 0.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.1f, 6.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.1f, -41.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.1f, -48.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.1f, -41.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.1f, -46.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.1f, -51.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.1f, 91.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 91.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -1.1f, 91.0f));				//Arbol 19
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[19].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -0.7f, 104.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -0.7f, 58.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.0f, -0.7f, 31.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.0f, -0.7f, -30.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.0f, -0.7f, -74.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-74.0f, -0.7f, -122.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-13.0f, -0.7f, 121.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-13.0f, -0.7f, 121.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(61.0f, -0.7f, 121.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -0.7f, 103.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -0.7f, 75.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -0.7f, 30.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -0.7f, -28.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -0.7f, -58.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -0.7f, -105.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(63.0f, -0.7f, -123.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, -0.7f, -123.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-31.0f, -0.7f, -123.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -0.7f, 90.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-26.0f, -0.7f, 63.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-21.0f, -0.7f, 63.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-16.0f, -0.7f, 63.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(16.0f, -0.7f, 21.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -0.7f, 21.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(24.0f, -0.7f, 21.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(28.0f, -0.7f, 21.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-44.0f, -0.7f, 5.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-44.0f, -0.7f, 1.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-44.0f, -0.7f, -3.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-44.0f, -0.7f, -7.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-44.0f, -0.7f, -93.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, -0.7f, -93.0f));				//Arbol 17
	model = glm::scale(model, glm::vec3(0.03f, 0.05f, 0.03f));
	shader.setMat4("model", model);
	modelo[17].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(31.0f, -1.1f, 120.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(13.0f, -1.1f, 120.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-31.0f, -1.1f, 120.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-59.0f, -1.1f, 120.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -1.1f, 74.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -1.1f, 16.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -1.1f, -13.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -1.1f, -58.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-76.0f, -1.1f, -104.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, -1.1f, -123.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, -1.1f, -123.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -1.1f, -123.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.1f, -75.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.1f, -16.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.1f, 13.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, -1.1f, 59.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.1f, 89.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(28.0f, -1.1f, 63.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.1f, 63.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(18.0f, -1.1f, 63.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -1.1f, 21.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-19.0f, -1.1f, 21.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -1.1f, 21.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-27.0f, -1.1f, 21.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(46.0f, -1.1f, 4.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(46.0f, -1.1f, 0.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(46.0f, -1.1f, -4.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(46.0f, -1.1f, -8.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-13.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, -1.1f, -30.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, -1.1f, -93.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.1f, -93.0f));				//Arbol 18
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[18].Draw(shader);

	//FAROS

	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(movX, -1.2f, movZ));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);*/

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-64.5, -1.2f, 110.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-64.5, -1.2f, 66.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-64.5, -1.2f, 23.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-64.5, -1.2f, -24.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-64.5, -1.2f, -68.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-64.5, -1.2f, -112.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -1.2f, -112.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.2f, -112.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.2f, -112.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.2f, -66.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.2f, -21.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.2f, 24.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.2f, 68.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.2f, 111.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-24.0f, -1.2f, 110.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);
	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(22.0f, -1.2f, 110.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(54.0f, -1.2f, 99.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.2f, 99.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, 99.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, 99.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(22.0f, -1.2f, 110.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-21.0f, -1.2f, 52.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.2f, 52.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(53.0f, -1.2f, 52.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, 52.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, 0.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, -45.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, -100.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.2f, -100.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(54.0f, -1.2f, -100.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(54.0f, -1.2f, -46.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(54.0f, -1.2f, -3.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(53.0f, -1.2f, 39.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.2f, 39.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-21.0f, -1.2f, 39.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-53.0f, -1.2f, 39.0f));				//Lamp 21
	model = glm::scale(model, glm::vec3(0.4f, 0.25f, 0.4f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[21].Draw(shader);

	//FLORES

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-22.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(18.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(22.0f, -1.0f, -42.0f));				//Flower 20
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[20].Draw(shader);

	//PERRO

	model = glm::translate(glm::mat4(1.0f), glm::vec3(movPerroX, movPerroY, movPerroZ));				//Cuerpo
	tmp = model = glm::rotate(model, glm::radians(-gradosPerro), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(escalaPerro, escalaPerro, escalaPerro));
	shader.setMat4("model", model);
	modelo[36].Draw(shader);

	model = glm::translate(tmp, glm::vec3(0.095f, 0.39f, 0.27f));						//Pata frontal izquierda
	model = glm::scale(model, glm::vec3(escalaPerro, escalaPerro, escalaPerro));
	model = glm::rotate(model, glm::radians(40.0f - rotaPata), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[37].Draw(shader);

	model = glm::translate(tmp, glm::vec3(-0.115f, 0.39f, 0.28f));							//Pata frontal derecha
	model = glm::scale(model, glm::vec3(escalaPerro, escalaPerro, escalaPerro));
	model = glm::rotate(model, glm::radians(rotaPata), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[38].Draw(shader);

	model = glm::translate(tmp, glm::vec3(0.095f, 0.325f, -0.19f));							//Pata trasera izquierda
	model = glm::scale(model, glm::vec3(escalaPerro, escalaPerro, escalaPerro));
	model = glm::rotate(model, glm::radians(rotaPata), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[39].Draw(shader);

	model = glm::translate(tmp, glm::vec3(-0.0775f, 0.335f, -0.19f));						//Pata trasera derecha
	model = glm::scale(model, glm::vec3(escalaPerro, escalaPerro, escalaPerro));
	model = glm::rotate(model, glm::radians(40.0f - rotaPata), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[40].Draw(shader);

	model = glm::translate(tmp, glm::vec3(0.0f, 0.485f, -0.2f));							//Colita
	model = glm::scale(model, glm::vec3(escalaPerro, escalaPerro, escalaPerro));
	model = glm::rotate(model, glm::radians(20.0f - rotaColita), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	modelo[41].Draw(shader);

	//ALBERCA
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.975f, -2.49f, -64.25f));
	model = glm::scale(model, glm::vec3(289.775f, 289.775f, 289.775f));		
	shader.setMat4("model", model);
	modelo[45].Draw(shader);

	//CANCHAS FUTBOL BASKETBALL

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -23.5f));				//Porteria
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[22].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 5.0f));				//Porteria
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[22].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -8.0f));				//BalonSoccer
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[23].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.6f, -1.0f, 5.0f));				//Canasta
	model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[24].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.6f, -1.0f, -23.6f));				//Canasta
	model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[24].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -1.0f, -4.7f));				//BalonBasket
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[25].Draw(shader);

	//BARDAS

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-82.0f, -1.0f, -22.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-82.0f, -1.0f, 23.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-82.0f, -1.0f, -67.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-82.0f, -1.0f, 66.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(72.0f, -1.0f, -66.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(72.0f, -1.0f, 67.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(72.0f, -1.0f, -23.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(72.0f, -1.0f, 22.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -1.0f, -129.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, -129.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, 118.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -1.0f, 118.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[2].Draw(shader);


	//autos

	model = glm::translate(glm::mat4(1.0f), glm::vec3(58.0f, -1.0f, -31.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-91.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[3].Draw(shader); //challenger

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-58.39f, -0.5f, -51.19f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[4].Draw(shader); //pink sedan 

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.20, -0.6, -108.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	modelo[5].Draw(shader); //mercedes

	model = glm::translate(glm::mat4(1.0f), glm::vec3(27.0f, -0.6, 47.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	modelo[5].Draw(shader); //mercedes

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-29.6f, -0.5f, 106.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[4].Draw(shader); //challenger


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
		((char *)"../../FinalGrafica/Models/pruebaAlberca.obj"),							//1 - Casita de prueba (reservado pa cualquier cosa)

		//De 2 a 16 sigue Mildred...
		((char *)"../../FinalGrafica/Models/contorno/reja.obj"), //2
		((char *)"../../FinalGrafica/Models/challenger/CHALLENGER71.obj"), //3
		((char *)"../../FinalGrafica/Models/carro/clasico.obj"), //4
		((char *)"../../FinalGrafica/Models/mercedes/mercedes.obj"), //5
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //6
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //7
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //8
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //9
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //10
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //11
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //12
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //13
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //14
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //15
		((char *)"../../FinalGrafica/Models/DUMMY.obj"), //16

		//17 empieza Joya hasta 31...
		((char *)"../../FinalGrafica/Models/Tree2/N64_Tree.obj"), //17
		((char *)"../../FinalGrafica/Models/Tree1/n64tree.obj"), //18
		((char *)"../../FinalGrafica/Models/Tree3/Tree3.obj"),	//19
		((char *)"../../FinalGrafica/Models/Flowers/flowers.obj"), //20
		((char *)"../../FinalGrafica/Models/StreetLight/lamp.obj"),//21
		((char *)"../../FinalGrafica/Models/Porteria/Porteria.obj"),//22
		((char *)"../../FinalGrafica/Models/SoccerBall/SoccerBall.obj"),//23
		((char *)"../../FinalGrafica/Models/Basket/Basketball.obj"),//24
		((char *)"../../FinalGrafica/Models/Basketball/Basketball.obj"),//25
		((char *)"../../FinalGrafica/Models/Fence/fobj_fence.obj"),//26
		((char *)"../../FinalGrafica/Models/Brick/Brick.obj"),//27
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),

		//De 32 a 46 Chavira...
		((char *)"../../FinalGrafica/Models/casa1/casa1.obj"),				//32
		((char *)"../../FinalGrafica/Models/casa2/casa2.obj"),				//33
		((char *)"../../FinalGrafica/Models/casa3/casa3.obj"),				//34
		((char *)"../../FinalGrafica/Models/casa4/casa4.obj"),				//35
		((char *)"../../FinalGrafica/Models/dog/test/cuerpo.obj"),						//36
		((char *)"../../FinalGrafica/Models/dog/test/frontalIzquierda.obj"),			//37
		((char *)"../../FinalGrafica/Models/dog/test/frontalDerecha.obj"),				//38
		((char *)"../../FinalGrafica/Models/dog/test/traseraIzquierda.obj"),			//39
		((char *)"../../FinalGrafica/Models/dog/test/traseraDerecha.obj"),				//40
		((char *)"../../FinalGrafica/Models/dog/test/colita.obj"),						//41
		((char *)"../../FinalGrafica/Models/tree/n64tree.obj"),						//42
		((char *)"../../FinalGrafica/Models/flores/untitled.obj"),				//43
		((char *)"../../FinalGrafica/Models/flores/untitled2.obj"),				//44
		((char *)"../../FinalGrafica/Models/alberca/alberca.obj"),				//45
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//46
	};


	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("../../FinalGrafica/SkyBox/derecha.tga"); //derecha
	faces.push_back("../../FinalGrafica/SkyBox/izquierda.tga"); //izquierda
	faces.push_back("../../FinalGrafica/SkyBox/arriba.tga"); //arriba
	faces.push_back("../../FinalGrafica/SkyBox/abajo.tga"); //abajo
	faces.push_back("../../FinalGrafica/SkyBox/frente.tga"); //frente
	faces.push_back("../../FinalGrafica/SkyBox/atras.tga"); //atrás

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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
		lightPosition.z -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPosition.z += 0.5f;


	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		if (preciso)
			movY += incPreciso;
		else
			movY += incNormal;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		if (preciso)
			movY -= incPreciso;
		else
			movY -= incNormal;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		if (preciso)
			movX -= incPreciso;
		else
			movX -= incNormal;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		if (preciso)
			movX += incPreciso;
		else
			movX += incNormal;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		if (preciso)
			movZ += incPreciso;
		else
			movZ += incNormal;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		if (preciso)
			movZ -= incPreciso;
		else
			movZ -= incNormal;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		if (preciso)
			escala -= incPreciso;
		else
			escala -= incNormal;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		if (preciso)
			escala += incPreciso;
		else
			escala += incNormal;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotacion = (float)((int)(rotacion + incRotacion) % 360);
		if (rotaPata < 45.0f)
			rotaPata += incRotacion;
		if (rotaColita < 20.0f)
			rotaColita += incRotacion * 1.2;
		ba--;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotacion = (float)((int)(rotacion - incRotacion) % 360);
		if (rotaPata > -20.0f)
			rotaPata -= incRotacion;
		if (rotaColita > -20.0f)
			rotaColita -= incRotacion * 1.2;
		ba++;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		//animacion = true;
		animacionPerro = !animacionPerro;
		preciso = !preciso;
	}
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		valorPlano = 1 - valorPlano;

	printf("Posicion: %f, %f, %f\tEscala: %f\tRotacion:%f\n", movX, movY, movZ, escala, rotacion);
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