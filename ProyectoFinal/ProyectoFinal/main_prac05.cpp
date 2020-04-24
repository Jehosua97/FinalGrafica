/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*-------------  JOYA VENEGAS JEHOSUA ALAN ----------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(Shader);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f;

//For Keyboard
float	rotX = 0.0f,
rotY = 1.0f,
rotZ = 0.0f;

float hombro = 0.0f;
float codo = 0.0f;
float dedoC = 0.0f;
float dedoD = 0.0f;
float dedoE = 0.0f;
float dedoF = 0.0f;
float dedoG = 0.0f;
float dedoH = 0.0f;
float dedoI = 0.0f;
float dedoJ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(Shader shader)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");


	shader.use();

	// create transformations and Projection
	glm::mat4 temporal = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES
	glm::mat4 temporalPollo = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 temporalA = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 temporalEF = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 temporalGH = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 temporalIJ = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models

	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ)); //Me ayuda a modificar todos los objtos del ecenario
	view = glm::rotate(view, //Rotando la imagen
		glm::radians(rotY),  //Variable que modifica los angulos
		glm::vec3(0.0f, 0.1f, 0.0f)); //Eje en el que se va a rotar 

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//Colocar código aquí

	/*
	model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f)); //Mueve la figura para qjue no se muestren encimadas
	model = glm::scale(model, glm::vec3(4.0f, 2.0f, 1.0f));		//Factor de escala, si ocupo negativos se ven de forma simétrica
	shader.setMat4("model", model);								//Muestra el objeto
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));		//Modificando el color
	glDrawArrays(GL_QUADS, 0, 24); //B

	//model = glm::translate(model, glm::vec3(-4.0f/4.0F, 4.5f/2.0F, 0.0f/1.0F)); //Se ve afectado por la esclaa y la traslación
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f)); //Reiniciamos el valor de origen
	glDrawArrays(GL_QUADS, 0, 24); //C
	shader.setMat4("model", model);								//Muestra el objeto
	*/

	model = glm::scale(model, glm::vec3(5.0f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //Pecho

	temporal = model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Cuello

	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
	shader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Cabeza desde el centro */

	model = glm::translate(temporal, glm::vec3(0.0f, 1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.2f, 1.0f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //Cabeza desde el cuello

	temporal = model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.9f, 0.2f, 0.6f));
	glDrawArrays(GL_QUADS, 0, 24); //Brazo izquierdo

	model = glm::translate(temporal, glm::vec3(-0.25f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //Mano desde brazo izquierdo

	//ROTANDO HOMBRO
	temporal = glm::translate(temporal, glm::vec3(6.0f, 0.0f, 0.0f));
	model = glm::rotate(temporal, glm::radians(hombro),
		glm::vec3(0.0f, 0.0f, 1.0f));
	temporalA = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //A

	//ROTANDO CODO
	model = glm::translate(temporalA, glm::vec3(1.25f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(codo),
		glm::vec3(0.0f, 1.0f, 0.0f));
	temporalIJ = temporalGH = temporalEF = temporalA = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //B

	//ROTANDO DEDO C
	model = glm::translate(temporalA, glm::vec3(1.0f, -0.35f, 0.375f));
	model = glm::rotate(model, glm::radians(dedoC),
		glm::vec3(0.0f, 1.0f, 0.0f));
	//PULGAR DEDO C
	temporalA = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //C

	//ROTANDO DEDO D
	model = glm::translate(temporalA, glm::vec3(0.25f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(dedoD),
		glm::vec3(0.0f, 1.0f, 0.0f));
	//PULGAR DEDO	D
	temporalA = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //D

	//ROTANDO PULGAR DEDO E
	model = glm::translate(temporalEF, glm::vec3(1.0f, 0.35f, 0.375f));
	model = glm::rotate(model, glm::radians(dedoE),
		glm::vec3(0.0f, 0.0f, 1.0f));
	//DIBUJANDO DEDO
	temporalEF = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //E

		//ROTANDO PULGAR DEDO F
	model = glm::translate(temporalEF, glm::vec3(0.25f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(dedoF),
		glm::vec3(0.0f, 0.0f, 1.0f));
	//DIBUJANDO DEDO F
	temporalEF = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //F

	//ROTANDO PULGAR DEDO G
	model = glm::translate(temporalGH, glm::vec3(1.0f, 0.35f, 0.0f));
	model = glm::rotate(model, glm::radians(dedoG),
		glm::vec3(0.0f, 0.0f, 1.0f));
	//DIBUJANDO DEDO G
	temporalGH = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //G

		//ROTANDO PULGAR DEDO H
	model = glm::translate(temporalGH, glm::vec3(0.375f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(dedoH),
		glm::vec3(0.0f, 0.0f, 1.0f));
	//DIBUJANDO DEDO H
	temporalGH = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //H

		//ROTANDO PULGAR DEDO I
	model = glm::translate(temporalIJ, glm::vec3(1.0f, 0.35f, -0.375f));
	model = glm::rotate(model, glm::radians(dedoI),
		glm::vec3(0.0f, 0.0f, 1.0f));
	//DIBUJANDO DEDO I
	temporalIJ = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //I

		//ROTANDO PULGAR DEDO J
	model = glm::translate(temporalIJ, glm::vec3(0.375f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(dedoJ),
		glm::vec3(0.0f, 0.0f, 1.0f));
	//DIBUJANDO DEDO J
	temporalIJ = model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.3f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //J










	/*temporal = model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.9f, 0.2f, 0.6f));
	glDrawArrays(GL_QUADS, 0, 24); //Brazo derecho

	temporal = model = glm::translate(temporal, glm::vec3(0.25f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //Mano desde brazo derecho

	temporal = model = glm::translate(temporal, glm::vec3(0.25f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.2f, 0.2f, 0.1f));
	glDrawArrays(GL_QUADS, 0, 24); //Mango de Espada desde mano derecho

	temporal = model = glm::translate(temporal, glm::vec3(1.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 2.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.1f, 0.8f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //Tope de espada desde mango

	temporalPollo = temporal = model = glm::translate(temporal, glm::vec3(2.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.5f, 0.2f, 0.6f));
	glDrawArrays(GL_QUADS, 0, 24); //Filo de la espada desde Tope
	*/
	temporal = model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.3f, 0.3f, 0.6f));
	glDrawArrays(GL_QUADS, 0, 24); //Cintura

	temporal = model = glm::translate(temporal, glm::vec3(-1.75f, -2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.2f, 0.9f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //Pierna izquierdo desde cintura

	model = glm::translate(temporal, glm::vec3(-0.5f, -2.75f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.8f));
	glDrawArrays(GL_QUADS, 0, 24); //Pie dede piernaa derecha

	temporal = model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 4.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.2f, 0.9f, 0.2f));
	glDrawArrays(GL_QUADS, 0, 24); //Pierna izquierdo desde pecho

	temporal = model = glm::translate(temporal, glm::vec3(0.5f, -2.75f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.8f));
	glDrawArrays(GL_QUADS, 0, 24); //Pie dede piernaa derecha

	//POLLITOO

	temporalPollo = temporal = model = glm::translate(temporal, glm::vec3(10.0f, 8.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.8f, 5.7f, 2.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); //Cuerpo de pollo

	model = glm::translate(temporal, glm::vec3(0.0f, 3.2f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.7f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24); //Cabeza desde Cuerpo

	temporal = model = glm::translate(temporal, glm::vec3(-1.9f, 1.8f, 0.0f));
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	shader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Pico desde cuerpo

	model = glm::translate(temporal, glm::vec3(0.2f, -0.8f, 0.0f));
	model = glm::scale(model, glm::vec3(0.7f, 0.8f, 0.8f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.5f));
	glDrawArrays(GL_QUADS, 0, 24); //Gogote desde pico

	model = glm::translate(temporalPollo, glm::vec3(-0.5f, 1.95f, -1.30f)); //0.05
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //ojo derecho desde cuerpo*/

	model = glm::translate(temporalPollo, glm::vec3(-0.5f, 1.95f, 1.30f)); //0.05
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.05f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); //ojo izq desde cuerpo

	model = glm::translate(temporalPollo, glm::vec3(0.4f, -1.8f, 1.65f));
	model = glm::scale(model, glm::vec3(2.5f, 1.3f, 0.9f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.95f, 0.95f, 0.95f));
	glDrawArrays(GL_QUADS, 0, 24); //Ala izq desde cuerpo

	model = glm::translate(temporalPollo, glm::vec3(0.4f, -1.8f, -1.65f));
	model = glm::scale(model, glm::vec3(2.5f, 1.3f, 0.9f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.95f, 0.95f, 0.95f));
	glDrawArrays(GL_QUADS, 0, 24); //Ala derecha desde cuerpo

	temporal = model = glm::translate(temporalPollo, glm::vec3(1.9f, -1.9f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 2.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); //Precolita desde cuerpo

	model = glm::translate(temporal, glm::vec3(0.6f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 2.0f, 2.1f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.95f, 0.95f, 0.95f));
	glDrawArrays(GL_QUADS, 0, 24); //colita desde precolita

	temporal = model = glm::translate(temporalPollo, glm::vec3(0.6f, -3.35f, -0.6f));
	model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.4f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //pierna desde cuerpo derecha

	temporal = model = glm::translate(temporal, glm::vec3(0.0f, -0.7f, 0.0f));
	model = glm::scale(model, glm::vec3(1.8f, 0.4f, 0.9f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //pata desde pierna

	model = glm::translate(temporal, glm::vec3(-1.1f, 0.0f, -0.285f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.3f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //uña derecha desde pierna dere

	model = glm::translate(temporal, glm::vec3(-1.1f, 0.0f, 0.285f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.3f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //uña izquierda desde pierna dere

	temporal = model = glm::translate(temporalPollo, glm::vec3(0.6f, -3.35f, 0.6f));
	model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.4f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //pierna desde cuerpo izquierda

	temporal = model = glm::translate(temporal, glm::vec3(0.0f, -0.7f, 0.0f));
	model = glm::scale(model, glm::vec3(1.8f, 0.4f, 0.9f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //pata desde pierna

	model = glm::translate(temporal, glm::vec3(-1.1f, 0.0f, -0.285f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.3f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //uña derecha desde pierna dere

	model = glm::translate(temporal, glm::vec3(-1.1f, 0.0f, 0.285f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.3f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.72f, 0.066f));
	glDrawArrays(GL_QUADS, 0, 24); //uña izquierda desde pierna dere


	

	glBindVertexArray(0);

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		my_input(window);

		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display(projectionShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant as are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rotY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		rotY -= 0.08f;
	//hombro
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && 90.0 > hombro)
		hombro += 1.08f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && -120.0 < hombro)
		hombro -= 1.08f;
	//codo
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && -110.0 < codo)
		codo -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && 0.0 > codo)
		codo += 1.08f;
	//Dedo C
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && -20.0 < dedoC)
		dedoC -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && 110.0 > dedoC)
		dedoC += 1.08f;
	//Dedo D
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 0.0 < dedoD)
		dedoD -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && 110.0 > dedoD)
		dedoD += 1.08f;
	//Dedo E
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && -110.0 < dedoE)
		dedoE -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && 20.0 > dedoE)
		dedoE += 1.08f;
	//Dedo F
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && -110.0 < dedoF)
		dedoF -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && 0.0 > dedoF)
		dedoF += 1.08f;
	//Dedo G
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && -110.0 < dedoG)
		dedoG -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && 20.0 > dedoG)
		dedoG += 1.08f;
	//Dedo H
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && -110.0 < dedoH)
		dedoH -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && 0.0 > dedoH)
		dedoH += 1.08f;
	//Dedo I
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && -110.0 < dedoI)
		dedoI -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && 20.0 > dedoI)
		dedoI += 1.08f;
	//Dedo J
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && -110.0 < dedoJ)
		dedoJ -= 1.08f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && 0.0 > dedoJ)
		dedoJ += 1.08f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}