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

#include <windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")
void playMusic(void);


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
bool dia = true;
void myData(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);
int hora, minuto;

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

//ANIMACIÓN POR KEYFRAMES
//Reutilizar variables movX, movY, movZ y rotacion
//Keyframes
//Teclado
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f,
movBrazo = 0.0f,
movCabe = 0.0f;

//Incrementos
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f,
movBrazoInc = 0.0f,
movCabeInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazo;
	float movCabe;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) {
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	KeyFrame[FrameIndex].movBrazo = movBrazo;
	KeyFrame[FrameIndex].movCabe = movCabe;

	FrameIndex++;
}

void resetElements(void) {
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazo = KeyFrame[0].movBrazo;
	movCabe = KeyFrame[0].movCabe;
}

void interpolation(void) {
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	movBrazoInc = (KeyFrame[playIndex + 1].movBrazo - KeyFrame[playIndex].movBrazo) / i_max_steps;
	movCabeInc = (KeyFrame[playIndex + 1].movCabe - KeyFrame[playIndex].movCabe) / i_max_steps;
}

//Para cambiar de plano
int valorPlano = 1;

//Lighting
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);
glm::vec3 lightPosition0(0.0f, -1.0f, 0.0f);
glm::vec3 lightPosition(0.0f, 4.0f, 3.0f);
glm::vec3 lightPosition1(-65.0f, 1.0f, 64.0f);
glm::vec3 lightPosition2(-65.0f, 1.0f, 21.0f);
glm::vec3 lightPosition3(-65.0f, 1.0f, -27.0f);
glm::vec3 lightPosition4(-65.0f, 1.0f, -70.0f);
glm::vec3 lightPosition5(-65.0f, 1.0f, -115.0f);
glm::vec3 lightPosition6(-53.0f, 1.0f, 99.0f);
glm::vec3 lightPosition7(-53.0f, 1.0f, 51.0f);
glm::vec3 lightPosition8(-53.0f, 1.0f, 37.0f);
glm::vec3 lightPosition9(-53.0f, 1.0f, -1.0f);
glm::vec3 lightPosition10(-53.0f, 1.0f, -46.0f);
glm::vec3 lightPosition11(-53.0f, 1.0f, -101.0f);
glm::vec3 lightPosition12(-22.0f, 1.0f, -112.0f);
glm::vec3 lightPosition13(22.0f, 1.0f, -112.0f);
glm::vec3 lightPosition14(65.0f, 1.0f, -112.0f);
glm::vec3 lightPosition15(65.0f, 1.0f, -65.0f);
glm::vec3 lightPosition16(65.0f, 1.0f, -21.0f);
glm::vec3 lightPosition17(65.0f, 1.0f, 24.0f);
glm::vec3 lightPosition18(65.0f, 1.0f, 69.0f);
glm::vec3 lightPosition19(65.0f, 1.0f, 112.0f);
glm::vec3 lightPosition20(20.0f, 1.0f, 110.0f);
glm::vec3 lightPosition21(-25.0f, 1.0f, 110.0f);
glm::vec3 lightPosition22(0.0f, 1.0f, -99.0f);
glm::vec3 lightPosition23(53.0f, 1.0f, -99.0f);
glm::vec3 lightPosition24(53.0f, 1.0f, -45.0f);
glm::vec3 lightPosition25(53.0f, 1.0f, -2.0f);
glm::vec3 lightPosition26(53.0f, 1.0f, 40.0f);
glm::vec3 lightPosition27(53.0f, 1.0f, 53.0f);
glm::vec3 lightPosition28(53.0f, 1.0f, 100.0f);
glm::vec3 lightPosition29(-2.0f, 1.0f, 100.0f);
glm::vec3 lightPosition30(52.0f, 1.0f, 100.0f);
glm::vec3 lightPosition31(-2.0f, 1.0f, 100.0f);
glm::vec3 lightPosition32(23.0f, 1.0f, 53.0f);
glm::vec3 lightPosition33(-21.0f, 1.0f, 53.0f);
glm::vec3 lightPosition34(-21.0f, 1.0f, 40.0f);
glm::vec3 lightPosition35(23.0f, 1.0f, 40.0f);
glm::vec3 lightPosition36(-65.0f, 1.0f, 110.0f);

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

//CRASH


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
	if (play) {
		if (i_curr_steps >= i_max_steps) { //end of animation between frames?
			playIndex++;
			if (playIndex > FrameIndex - 2) {	//end of total animation?
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else { //Next frame interpolations
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else {
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;
			movBrazo += movBrazoInc;
			movCabe += movCabeInc;

			i_curr_steps++;
		}
	}


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
	//ILUMINACIÓN DIA Y NOCHE
	if (dia) {
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
		//Luz quién sabe

		shader.setVec3("pointLight[1].position", lightPosition0);
		shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[2].position", lightPosition1);
		shader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[3].position", lightPosition2);
		shader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[3].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[4].position", lightPosition3);
		shader.setVec3("pointLight[4].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[4].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[4].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[5].position", lightPosition4);
		shader.setVec3("pointLight[5].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[5].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[5].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[6].position", lightPosition5);
		shader.setVec3("pointLight[6].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[6].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[6].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[7].position", lightPosition6);
		shader.setVec3("pointLight[7].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[7].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[7].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[8].position", lightPosition7);
		shader.setVec3("pointLight[8].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[8].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[8].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[9].position", lightPosition8);
		shader.setVec3("pointLight[9].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[9].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[9].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[10].position", lightPosition9);
		shader.setVec3("pointLight[10].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[10].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[10].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[11].position", lightPosition10);
		shader.setVec3("pointLight[11].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[11].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[11].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[12].position", lightPosition11);
		shader.setVec3("pointLight[12].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[12].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[12].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[13].position", lightPosition12);
		shader.setVec3("pointLight[13].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[13].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[13].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[14].position", lightPosition13);
		shader.setVec3("pointLight[14].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[14].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[14].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[15].position", lightPosition14);
		shader.setVec3("pointLight[15].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[15].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[15].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[16].position", lightPosition15);
		shader.setVec3("pointLight[16].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[16].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[16].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[17].position", lightPosition16);
		shader.setVec3("pointLight[17].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[17].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[17].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[18].position", lightPosition17);
		shader.setVec3("pointLight[18].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[18].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[18].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[19].position", lightPosition18);
		shader.setVec3("pointLight[19].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[19].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[19].specular", glm::vec3(0.0f, 0.0f, 0.0f));


		shader.setVec3("pointLight[20].position", lightPosition19);
		shader.setVec3("pointLight[20].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[20].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[20].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[21].position", lightPosition20);
		shader.setVec3("pointLight[21].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[21].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[21].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[22].position", lightPosition21);
		shader.setVec3("pointLight[22].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[22].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[22].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[23].position", lightPosition22);
		shader.setVec3("pointLight[23].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[23].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[23].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[24].position", lightPosition23);
		shader.setVec3("pointLight[24].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[24].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[24].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[25].position", lightPosition24);
		shader.setVec3("pointLight[25].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[25].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[25].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[26].position", lightPosition25);
		shader.setVec3("pointLight[26].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[26].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[26].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[27].position", lightPosition26);
		shader.setVec3("pointLight[27].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[27].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[27].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[28].position", lightPosition27);
		shader.setVec3("pointLight[28].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[28].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[28].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[29].position", lightPosition28);
		shader.setVec3("pointLight[29].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[29].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[29].specular", glm::vec3(0.0f, 0.0f, 0.0f));


		shader.setVec3("pointLight[30].position", lightPosition29);
		shader.setVec3("pointLight[30].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[30].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[30].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[31].position", lightPosition30);
		shader.setVec3("pointLight[31].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[31].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[31].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[32].position", lightPosition31);
		shader.setVec3("pointLight[32].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[32].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[32].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[33].position", lightPosition32);
		shader.setVec3("pointLight[33].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[33].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[33].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[34].position", lightPosition33);
		shader.setVec3("pointLight[34].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[34].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[34].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		shader.setVec3("pointLight[35].position", lightPosition);
		shader.setVec3("pointLight[35].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[35].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[35].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		/*
		shader.setVec3("pointLight[36].position", lightPosition0);
		shader.setVec3("pointLight[36].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[36].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("pointLight[36].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		*/


		shader.setFloat("material_shininess", 32.0f);
	}
	else {
		//Setup Advanced Lights
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("dirLight.direction", lightDirection);
		shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		shader.setVec3("pointLight[0].position", lightPosition36);
		shader.setVec3("pointLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));	//Luz ambiental de 0 a 1
		shader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[0].constant", 1.0f);
		shader.setFloat("pointLight[0].linear", 0.001f);
		shader.setFloat("pointLight[0].quadratic", 0.0032f);

		shader.setVec3("pointLight[1].position", lightPosition0);
		shader.setVec3("pointLight[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[1].constant", 1.0f);
		shader.setFloat("pointLight[1].linear", 0.001f);						//2
		shader.setFloat("pointLight[1].quadratic", 0.0032f);

		shader.setVec3("pointLight[2].position", lightPosition1);
		shader.setVec3("pointLight[2].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[2].constant", 1.0f);
		shader.setFloat("pointLight[2].linear", 0.001f);						//3
		shader.setFloat("pointLight[2].quadratic", 0.0032f);

		shader.setVec3("pointLight[3].position", lightPosition2);
		shader.setVec3("pointLight[3].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[3].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[3].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[3].constant", 1.0f);
		shader.setFloat("pointLight[3].linear", 0.001f);						//4
		shader.setFloat("pointLight[3].quadratic", 0.0032f);

		shader.setVec3("pointLight[4].position", lightPosition3);
		shader.setVec3("pointLight[4].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[4].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[4].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[4].constant", 1.0f);
		shader.setFloat("pointLight[4].linear", 0.001f);						//5
		shader.setFloat("pointLight[4].quadratic", 0.0032f);

		shader.setVec3("pointLight[5].position", lightPosition4);
		shader.setVec3("pointLight[5].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[5].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[5].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[5].constant", 1.0f);
		shader.setFloat("pointLight[5].linear", 0.001f);						//6
		shader.setFloat("pointLight[5].quadratic", 0.0032f);

		shader.setVec3("pointLight[6].position", lightPosition5);
		shader.setVec3("pointLight[6].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[6].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[6].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[6].constant", 1.0f);
		shader.setFloat("pointLight[6].linear", 0.001f);						//7
		shader.setFloat("pointLight[6].quadratic", 0.0032f);

		shader.setVec3("pointLight[7].position", lightPosition6);
		shader.setVec3("pointLight[7].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[7].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[7].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[7].constant", 1.0f);
		shader.setFloat("pointLight[7].linear", 0.001f);						//8
		shader.setFloat("pointLight[7].quadratic", 0.0032f);

		shader.setVec3("pointLight[8].position", lightPosition7);
		shader.setVec3("pointLight[8].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[8].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[8].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[8].constant", 1.0f);
		shader.setFloat("pointLight[8].linear", 0.001f);						//9
		shader.setFloat("pointLight[8].quadratic", 0.0032f);

		shader.setVec3("pointLight[9].position", lightPosition8);
		shader.setVec3("pointLight[9].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[9].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[9].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[9].constant", 1.0f);
		shader.setFloat("pointLight[9].linear", 0.001f);						//10
		shader.setFloat("pointLight[9].quadratic", 0.0032f);

		shader.setVec3("pointLight[10].position", lightPosition9);
		shader.setVec3("pointLight[10].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[10].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[10].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[10].constant", 1.0f);
		shader.setFloat("pointLight[10].linear", 0.001f);						//11
		shader.setFloat("pointLight[10].quadratic", 0.0032f);

		shader.setVec3("pointLight[11].position", lightPosition10);
		shader.setVec3("pointLight[11].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[11].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[11].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[11].constant", 1.0f);
		shader.setFloat("pointLight[11].linear", 0.001f);						//12
		shader.setFloat("pointLight[11].quadratic", 0.0032f);

		shader.setVec3("pointLight[12].position", lightPosition11);
		shader.setVec3("pointLight[12].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[12].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[12].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[12].constant", 1.0f);
		shader.setFloat("pointLight[12].linear", 0.001f);						//13
		shader.setFloat("pointLight[12].quadratic", 0.0032f);

		shader.setVec3("pointLight[13].position", lightPosition12);
		shader.setVec3("pointLight[13].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[13].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[13].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[13].constant", 1.0f);
		shader.setFloat("pointLight[13].linear", 0.001f);						//14
		shader.setFloat("pointLight[13].quadratic", 0.0032f);

		shader.setVec3("pointLight[14].position", lightPosition13);
		shader.setVec3("pointLight[14].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[14].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[14].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[14].constant", 1.0f);
		shader.setFloat("pointLight[14].linear", 0.001f);						//15
		shader.setFloat("pointLight[14].quadratic", 0.0032f);

		shader.setVec3("pointLight[15].position", lightPosition14);
		shader.setVec3("pointLight[15].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[15].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[15].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[15].constant", 1.0f);
		shader.setFloat("pointLight[15].linear", 0.001f);						//16
		shader.setFloat("pointLight[15].quadratic", 0.0032f);

		shader.setVec3("pointLight[16].position", lightPosition15);
		shader.setVec3("pointLight[16].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[16].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[16].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[16].constant", 1.0f);
		shader.setFloat("pointLight[16].linear", 0.001f);						//17
		shader.setFloat("pointLight[16].quadratic", 0.0032f);

		shader.setVec3("pointLight[17].position", lightPosition16);
		shader.setVec3("pointLight[17].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[17].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[17].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[17].constant", 1.0f);
		shader.setFloat("pointLight[17].linear", 0.001f);						//18
		shader.setFloat("pointLight[17].quadratic", 0.0032f);

		shader.setVec3("pointLight[18].position", lightPosition17);
		shader.setVec3("pointLight[18].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[18].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[18].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[18].constant", 1.0f);
		shader.setFloat("pointLight[18].linear", 0.001f);						//19
		shader.setFloat("pointLight[18].quadratic", 0.0032f);

		shader.setVec3("pointLight[19].position", lightPosition18);
		shader.setVec3("pointLight[19].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[19].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[19].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[19].constant", 1.0f);
		shader.setFloat("pointLight[19].linear", 0.001f);						//20
		shader.setFloat("pointLight[19].quadratic", 0.0032f);

		shader.setVec3("pointLight[20].position", lightPosition19);
		shader.setVec3("pointLight[20].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[20].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[20].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[20].constant", 1.0f);
		shader.setFloat("pointLight[20].linear", 0.001f);						//21
		shader.setFloat("pointLight[20].quadratic", 0.0032f);

		shader.setVec3("pointLight[21].position", lightPosition20);
		shader.setVec3("pointLight[21].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[21].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[21].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[21].constant", 1.0f);
		shader.setFloat("pointLight[21].linear", 0.001f);						//22
		shader.setFloat("pointLight[21].quadratic", 0.0032f);

		shader.setVec3("pointLight[22].position", lightPosition21);
		shader.setVec3("pointLight[22].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[22].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[22].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[22].constant", 1.0f);
		shader.setFloat("pointLight[22].linear", 0.001f);						//23
		shader.setFloat("pointLight[22].quadratic", 0.0032f);

		shader.setVec3("pointLight[23].position", lightPosition22);
		shader.setVec3("pointLight[23].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[23].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[23].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[23].constant", 1.0f);
		shader.setFloat("pointLight[23].linear", 0.001f);						//24
		shader.setFloat("pointLight[23].quadratic", 0.0032f);

		shader.setVec3("pointLight[24].position", lightPosition23);
		shader.setVec3("pointLight[24].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[24].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[24].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[24].constant", 1.0f);
		shader.setFloat("pointLight[24].linear", 0.001f);						//25
		shader.setFloat("pointLight[24].quadratic", 0.0032f);

		shader.setVec3("pointLight[25].position", lightPosition24);
		shader.setVec3("pointLight[25].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[25].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[25].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[25].constant", 1.0f);
		shader.setFloat("pointLight[25].linear", 0.001f);						//26
		shader.setFloat("pointLight[25].quadratic", 0.0032f);

		shader.setVec3("pointLight[26].position", lightPosition25);
		shader.setVec3("pointLight[26].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[26].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[26].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[26].constant", 1.0f);
		shader.setFloat("pointLight[26].linear", 0.001f);						//27
		shader.setFloat("pointLight[26].quadratic", 0.0032f);

		shader.setVec3("pointLight[27].position", lightPosition26);
		shader.setVec3("pointLight[27].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[27].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[27].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[27].constant", 1.0f);
		shader.setFloat("pointLight[27].linear", 0.001f);						//28
		shader.setFloat("pointLight[27].quadratic", 0.0032f);

		shader.setVec3("pointLight[28].position", lightPosition27);
		shader.setVec3("pointLight[28].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[28].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[28].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[28].constant", 1.0f);
		shader.setFloat("pointLight[28].linear", 0.001f);						//29
		shader.setFloat("pointLight[28].quadratic", 0.0032f);


		shader.setVec3("pointLight[29].position", lightPosition28);
		shader.setVec3("pointLight[29].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[29].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[29].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[29].constant", 1.0f);
		shader.setFloat("pointLight[29].linear", 0.001f);						//30
		shader.setFloat("pointLight[29].quadratic", 0.0032f);

		shader.setVec3("pointLight[30].position", lightPosition29);
		shader.setVec3("pointLight[30].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[30].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[30].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[30].constant", 1.0f);
		shader.setFloat("pointLight[30].linear", 0.001f);						//31
		shader.setFloat("pointLight[30].quadratic", 0.0032f);

		shader.setVec3("pointLight[31].position", lightPosition30);
		shader.setVec3("pointLight[31].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[31].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[31].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[31].constant", 1.0f);
		shader.setFloat("pointLight[31].linear", 0.001f);						//32
		shader.setFloat("pointLight[31].quadratic", 0.0032f);

		shader.setVec3("pointLight[32].position", lightPosition31);
		shader.setVec3("pointLight[32].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[32].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[32].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[32].constant", 1.0f);
		shader.setFloat("pointLight[32].linear", 0.001f);						//33
		shader.setFloat("pointLight[32].quadratic", 0.0032f);

		shader.setVec3("pointLight[33].position", lightPosition32);
		shader.setVec3("pointLight[33].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[33].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[33].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[33].constant", 1.0f);
		shader.setFloat("pointLight[33].linear", 0.001f);						//34
		shader.setFloat("pointLight[33].quadratic", 0.0032f);

		shader.setVec3("pointLight[34].position", lightPosition33);
		shader.setVec3("pointLight[34].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[34].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[34].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[34].constant", 1.0f);
		shader.setFloat("pointLight[34].linear", 0.001f);						//35
		shader.setFloat("pointLight[34].quadratic", 0.0032f);

		shader.setVec3("pointLight[35].position", lightPosition34);
		shader.setVec3("pointLight[35].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[35].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[35].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[35].constant", 1.0f);
		shader.setFloat("pointLight[35].linear", 0.001f);						//36
		shader.setFloat("pointLight[35].quadratic", 0.0032f);

		shader.setVec3("pointLight[36].position", lightPosition35);
		shader.setVec3("pointLight[36].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("pointLight[36].diffuse", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setVec3("pointLight[36].specular", glm::vec3(1.0f, 0.88f, 0.2f));
		shader.setFloat("pointLight[36].constant", 1.0f);
		shader.setFloat("pointLight[36].linear", 0.001f);						//37
		shader.setFloat("pointLight[36].quadratic", 0.0032f);

		shader.setFloat("material_shininess", 32.0f);
	}


	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
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

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-8.19f, -1.9f, -55.59f));
	model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.2f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[71].Draw(shader);

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

	//Sombrillas
	//Sombrillas de día
	if (dia) {		//Cambiar 'true' por variable de control de día y noche en el futuro.
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5f, -2.6f, -77.0f));			//21
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		modelo[47].Draw(shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-21.85f, -2.6f, -71.15f));			//21
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		modelo[47].Draw(shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.2f, -2.6f, -65.3f));			//21
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		//model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		modelo[47].Draw(shader);
	}
	//Sombrillas de noche
	else {
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5f, -2.6f, -77.0f));			//21
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		modelo[48].Draw(shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-21.85f, -2.6f, -71.15f));			//21
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		modelo[48].Draw(shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.2f, -2.6f, -65.3f));			//21
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		//model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		modelo[48].Draw(shader);
	}

	//Camastros
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-22.3f, -1.1f, -78.4f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[42].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.65f, -1.1f, -72.55f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[42].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-19.0f, -1.1f, -66.7f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[42].Draw(shader);



	//ARBUSTOS - 49, 50, 51
	//Primera sección, frente a la alberca
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -1.0f, -37.8f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[49].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, -1.0f, -37.8f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[49].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -37.8f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[49].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, -37.8f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[49].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(13.0f, -1.0f, -37.8f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[49].Draw(shader);
	//Segunda sección, a un lado de las canchas
	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, -25.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[51].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, -18.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[51].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, -11.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[51].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, -4.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[51].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(23.0f, -1.0f, -3.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[51].Draw(shader);

	//Tercera sección, entre casas inferiores
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, -2.6f, 81.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[50].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.6f, 81.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[50].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.6f, 81.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[50].Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, -2.6f, 81.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[50].Draw(shader);



	//CRASH
	temp = model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));				//Cuerpo
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[28].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.35f, 1.74f, 0.0f));				//BrazoDerecho
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[29].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.35f, 1.74f, 0.0f));				//BrazoIzquierdo
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[30].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.50f, 1.55f, -0.05f));				//CodoDerecho
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[62].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.50f, 1.60f, -0.05f));				//Codo Izquierdo
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, -0.1f));
	shader.setMat4("model", model);
	modelo[63].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.65f, 1.20f, -0.05f));				//Mano Derecha
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[64].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.65f, 1.25f, -0.05f));				//Mano Izquierda
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[65].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.09f, 0.99f, 0.0f));				//Pierna Derecha
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[66].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.09f, 1.0f, 0.01f));				//Pierna Izquierda
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.03f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[67].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.17f, 0.73f, 0.0f));				//Rodilla Derecha
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[68].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.17f, 0.72f, 0.0f));				//Rodilla Izquierda
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[69].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.04f, 0.97f, 1.0f));				//Aku Aku
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[27].Draw(shader);

/*--------------------RICK--------------------------------------------------------*/

	temp=model = glm::translate(glm::mat4(1.0f), glm::vec3(10.30f, 0.3f, 0.0f));				//Torso			
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[6].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.10f, 0.3f, 0.0f));				//Brazo Derecho		
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[7].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.10f, 0.3f, 0.0f));				//Brazo Izquierdo	
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[8].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.47f, 0.305f, -0.01));				//Codo Derecho	
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[9].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.47f, 0.305f, -0.01));				//Codo Izquierdo	
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[10].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.07f, -0.21f, 0.0f));				//Pierna Derecha
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[11].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.05f, -0.21f, 0.0f));				//Pierna Izquierdo	
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[12].Draw(shader);

	model = glm::translate(temp, glm::vec3(-0.09f, -0.63f, 0.0f));				//Brazo Izquierdo	
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[13].Draw(shader);

	model = glm::translate(temp, glm::vec3(0.07f, -0.63f, 0.0f));				//Brazo Izquierdo	
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	modelo[14].Draw(shader);

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
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
	Model modelo[77] = {
		((char *)"../../FinalGrafica/Models/bocetoMedidas.obj"),							//0 - Boceto, piso
		((char *)"../../FinalGrafica/Models/pruebaAlberca.obj"),							//1 - Plano con hueco de alberca y texturas

		//De 2 a 16 sigue Mildred...
		((char *)"../../FinalGrafica/Models/contorno/reja.obj"), //2
		((char *)"../../FinalGrafica/Models/challenger/CHALLENGER71.obj"), //3
		((char *)"../../FinalGrafica/Models/carro/clasico.obj"), //4
		((char *)"../../FinalGrafica/Models/mercedes/mercedes.obj"), //5
		((char *)"../../FinalGrafica/Models/Rick/Torso.obj"), //6
		((char *)"../../FinalGrafica/Models/Rick/BrazoDer.obj"), //7
		((char *)"../../FinalGrafica/Models/Rick/BrazoIzq.obj"), //8
		((char *)"../../FinalGrafica/Models/Rick/CodoDere.obj"), //9
		((char *)"../../FinalGrafica/Models/Rick/CodoIzq.obj"), //10
		((char *)"../../FinalGrafica/Models/Rick/PiernaDer.obj"), //11
		((char *)"../../FinalGrafica/Models/Rick/PiernaIzq.obj"), //12
		((char *)"../../FinalGrafica/Models/Rick/RodillaDer.obj"), //13
		((char *)"../../FinalGrafica/Models/Rick/RodillaIzq.obj"), //14
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
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),//26
		((char *)"../../FinalGrafica/Models/Crash/AkuAku.obj"),//27
		((char *)"../../FinalGrafica/Models/Crash/torso.obj"),//28
		((char *)"../../FinalGrafica/Models/Crash/BrazoDerecho.obj"),//29
		((char *)"../../FinalGrafica/Models/Crash/BrazoIzq.obj"),//30
		((char *)"../../FinalGrafica/Models/Crash/CejasAku.obj"),//31

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
		((char *)"../../FinalGrafica/Models/camastro/camastro.obj"),						//42
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//43
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//44
		((char *)"../../FinalGrafica/Models/alberca/alberca.obj"),				//45
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),				//46

		//Extensión Chavira 47 a 61
		((char *)"../../FinalGrafica/Models/sombrilla/sombrillaB.obj"),  //47
		((char *)"../../FinalGrafica/Models/sombrilla/sombrillaC.obj"),  //48
		((char *)"../../FinalGrafica/Models/flores/arbustos1.obj"),  //49
		((char *)"../../FinalGrafica/Models/flores/arbustos2.obj"),  //50
		((char *)"../../FinalGrafica/Models/flores/arbustos3.obj"),  //51
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //52
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //53
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //54
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //55
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //56
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //57
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //58
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //59
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //60
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //61

		//Extensión Joya 62 a 76
		((char *)"../../FinalGrafica/Models/Crash/CodoDerecho.obj"),  //62
		((char *)"../../FinalGrafica/Models/Crash/CodoIzq.obj"),  //63
		((char *)"../../FinalGrafica/Models/Crash/ManoDerecho.obj"),  //64
		((char *)"../../FinalGrafica/Models/Crash/ManoIzq.obj"),  //65
		((char *)"../../FinalGrafica/Models/Crash/PiernaDerecho.obj"),  //66
		((char *)"../../FinalGrafica/Models/Crash/PiernaIzq.obj"),  //67
		((char *)"../../FinalGrafica/Models/Crash/RodillaDerecho.obj"),  //68
		((char *)"../../FinalGrafica/Models/Crash/RodillaIzq.obj"),  //69
		((char *)"../../FinalGrafica/Models/Rick/Rick.obj"),  //70
		((char *)"../../FinalGrafica/Models/Candy/Candy.obj"),  //71
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //72
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //73
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //74
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //75
		((char *)"../../FinalGrafica/Models/DUMMY.obj"),  //76

	};

	//Inicializacion de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++) {
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
		KeyFrame[i].movBrazo = 0;
		KeyFrame[i].movCabe = 0;
	}

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

	//Música
	//play();

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
			posY += incPreciso;
		else
			posY += incNormal;

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		if (preciso)
			posY -= incPreciso;
		else
			posY -= incNormal;

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		if (preciso)
			posX -= incPreciso;
		else
			posX -= incNormal;

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		if (preciso)
			posX += incPreciso;
		else
			posX += incNormal;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		if (preciso)
			posZ += incPreciso;
		else
			posZ += incNormal;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		if (preciso)
			posZ -= incPreciso;
		else
			posZ -= incNormal;

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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//animacion = true;
		animacionPerro = !animacionPerro;
		preciso = !preciso;
	}
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		valorPlano = 1 - valorPlano;

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		playMusic();

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		dia = !dia;


	//To play KeyFrame animation 
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		if (play == false && (FrameIndex > 1)) {
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
			play = false;
	}

	//To Save a KeyFrame
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
		if (FrameIndex < MAX_FRAMES)
			saveFrame();


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

void playMusic() {
	std::cout << "Reproduciendo música";
	PlaySound("..\\..\\a.wav", NULL, SND_ASYNC);
}