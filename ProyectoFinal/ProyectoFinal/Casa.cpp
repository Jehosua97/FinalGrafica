/*---------------------------------------------------------*/
/* ----------------   Práctica 3 --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*-------------- Joya Venegas Jehosua Alan ---------------*/
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

float movX = 0, 
	  movY = 0, 
	  movZ = -15.0;
float eliseS = 180.0;
float eliseD = 90.0;
float eliseI = -90.0;
float circulo = 0.0, infe = 0.0;
float comun = 0.0;


void myData(void);
void display(void);
void getResolution(void);

//For Keyboard


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
		//Trapecio
		-1.0f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//A0
		1.0f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//B1
		0.5f,0.5f,0.0f,			0.0f, 0.0f, 0.0f,	//C2
		0.5f,0.5f,0.0f,			0.0f, 0.0f, 0.0f,	//C3
		-0.5f,0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//D4
		-1.0f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//A5
		//Triangulo
		-0.5f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//A6
		0.5f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//B7	
		0.5f,0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//C8
		//Rectangulo
		-0.5f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//A9
		0.5f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//B10
		0.5f,0.5f,0.0f,			0.0f, 0.0f, 0.0f,	//C11
		0.5f,0.5f,0.0f,			0.0f, 0.0f, 0.0f,	//C12
		-0.5f,0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//D13
		-0.5f,-0.5f,0.0f,		0.0f, 0.0f, 0.0f,	//A14
		//Circulo
		0.0f,0.0f,0.0f,0.0f,0.0f,0.0f, //A15
		0.9566376313267f,0.0822263604963f,0.0f,0.0f,0.0f,0.0f,
		0.9431612161849f,0.1576942852902f,0.0f,0.0f,0.0f,0.0f,
		0.9269895180148f,0.2304669270557f,0.0f,0.0f,0.0f,0.0f,
		0.9108178198447f,0.3059348518495f,0.0f,0.0f,0.0f,0.0f,
		0.8865602725895f,0.3679263615016f,0.0f,0.0f,0.0f,0.0f,
		0.859607442306f,0.4353084372104f,0.0f,0.0f,0.0f,0.0f,
		0.8326546120225f,0.4865188147491f,0.0f,0.0f,0.0f,0.0f,
		0.8057017817389f,0.5458150413729f,0.0f,0.0f,0.0f,0.0f,
		0.7625772532853f,0.5916348528548f,0.0f,0.0f,0.0f,0.0f,
		0.7248432908884f,0.6374546643368f,0.0f,0.0f,0.0f,0.0f,
		0.676328196378f,0.6886650418755f,0.0f,0.0f,0.0f,0.0f,
		0.622422535811f,0.7398754194142f,0.0f,0.0f,0.0f,0.0f,
		0.5678463414174f,0.7877857187084f,0.0f,0.0f,0.0f,0.0f,
		0.5169383614868f,0.8196032061651f,0.0f,0.0f,0.0f,0.0f,
		0.4533033865735f,0.857784191113f,0.0f,0.0f,0.0f,0.0f,
		0.3992136578972f,0.8896016785697f,0.0f,0.0f,0.0f,0.0f,
		0.3196699392556f,0.9182374172807f,0.0f,0.0f,0.0f,0.0f,
		0.2528532155966f,0.943691407246f,0.0f,0.0f,0.0f,0.0f,
		0.182854743192f,0.9596001509743f,0.0f,0.0f,0.0f,0.0f,
		0.1224015170244f,0.9691453972113f,0.0f,0.0f,0.0f,0.0f,
		0.0396760496371f,0.9786906434483f,0.0f,0.0f,0.0f,0.0f,
		-0.0303224227675f,0.9786906434483f,0.0f,0.0f,0.0f,0.0f,
		-0.0907756489352f,0.9755088947026f,0.0f,0.0f,0.0f,0.0f,
		-0.1703193675768f,0.9596001509743f,0.0f,0.0f,0.0f,0.0f,
		-0.281680573675f,0.934146161009f,0.0f,0.0f,0.0f,0.0f,
		-0.367587789808f,0.905510422298f,0.0f,0.0f,0.0f,0.0f,
		-0.4439497597039f,0.86732943735f,0.0f,0.0f,0.0f,0.0f,
		-0.5107664833629f,0.8291484524021f,0.0f,0.0f,0.0f,0.0f,
		-0.5744014582761f,0.7909674674541f,0.0f,0.0f,0.0f,0.0f,
		-0.6284911869524f,0.7432412362691f,0.0f,0.0f,0.0f,0.0f,
		-0.6793991668831f,0.6986967538298f,0.0f,0.0f,0.0f,0.0f,
		-0.7239436493224f,0.6446070251535f,0.0f,0.0f,0.0f,0.0f,
		-0.774851629253f,0.5777903014946f,0.0f,0.0f,0.0f,0.0f,
		-0.813032614201f,0.5109735778356f,0.0f,0.0f,0.0f,0.0f,
		-0.8575770966403f,0.4473386029223f,0.0f,0.0f,0.0f,0.0f,
		-0.8925763328426f,0.383703628009f,0.0f,0.0f,0.0f,0.0f,
		-0.9180303228079f,0.3137051556044f,0.0f,0.0f,0.0f,0.0f,
		-0.9339390665362f,0.2500701806911f,0.0f,0.0f,0.0f,0.0f,
		-0.9434843127732f,0.1641629645582f,0.0f,0.0f,0.0f,0.0f,
		-0.9530295590102f,0.1037097383906f,0.0f,0.0f,0.0f,0.0f,
		-0.9657565539929f,0.0432565122229f,0.0f,0.0f,0.0f,0.0f,
		-0.9530295590102f,-0.033105457673f,0.0f,0.0f,0.0f,0.0f,
		-0.9562113077559f,-0.0935586838406f,0.0f,0.0f,0.0f,0.0f,
		-0.9307573177906f,-0.2144651361759f,0.0f,0.0f,0.0f,0.0f,
		-0.9053033278253f,-0.2876453573262f,0.0f,0.0f,0.0f,0.0f,
		-0.8830310866056f,-0.3480985834938f,0.0f,0.0f,0.0f,0.0f,
		-0.8575770966403f,-0.3958248146787f,0.0f,0.0f,0.0f,0.0f,
		-0.832123106675f,-0.459459789592f,0.0f,0.0f,0.0f,0.0f,
		-0.7971238704727f,-0.5103677695227f,0.0f,0.0f,0.0f,0.0f,
		-0.765306383016f,-0.5580940007076f,0.0f,0.0f,0.0f,0.0f,
		-0.7303071468137f,-0.6026384831469f,0.0f,0.0f,0.0f,0.0f,
		-0.6921261618657f,-0.6471829655862f,0.0f,0.0f,0.0f,0.0f,
		-0.6443999306808f,-0.6917274480255f,0.0f,0.0f,0.0f,0.0f,
		-0.5966736994958f,-0.7330901817192f,0.0f,0.0f,0.0f,0.0f,
		-0.5521292170565f,-0.7680894179215f,0.0f,0.0f,0.0f,0.0f,
		-0.4980394883802f,-0.8062704028695f,0.0f,0.0f,0.0f,0.0f,
		-0.4344045134669f,-0.8349061415804f,0.0f,0.0f,0.0f,0.0f,
		-0.3834965335363f,-0.8667236290371f,0.0f,0.0f,0.0f,0.0f,
		-0.3166798098773f,-0.8858141215111f,0.0f,0.0f,0.0f,0.0f,
		-0.2689535786924f,-0.9049046139851f,0.0f,0.0f,0.0f,0.0f,
		-0.2021368550334f,-0.923995106459f,0.0f,0.0f,0.0f,0.0f,
		-0.1162296389005f,-0.9271768552047f,0.0f,0.0f,0.0f,0.0f,
		-0.0653216589698f,-0.9271768552047f,0.0f,0.0f,0.0f,0.0f,
		-0.0112319302935f,-0.9367221014417f,0.0f,0.0f,0.0f,0.0f,
		0.0714935370937f,-0.9303586039504f,0.0f,0.0f,0.0f,0.0f,
		0.1319467632614f,-0.9303586039504f,0.0f,0.0f,0.0f,0.0f,
		0.2051269844116f,-0.9208133577134f,0.0f,0.0f,0.0f,0.0f,
		0.2814889543076f,-0.9017228652394f,0.0f,0.0f,0.0f,0.0f,
		0.3578509242035f,-0.8667236290371f,0.0f,0.0f,0.0f,0.0f,
		0.4373946428451f,-0.8317243928348f,0.0f,0.0f,0.0f,0.0f,
		0.5169383614868f,-0.7839981616498f,0.0f,0.0f,0.0f,0.0f,
		0.5964820801284f,-0.7362719304648f,0.0f,0.0f,0.0f,0.0f,
		0.6659863753178f,-0.6694552068059f,0.0f,0.0f,0.0f,0.0f,
		0.7251129489887f,-0.604953490074f,0.0f,0.0f,0.0f,0.0f,
		0.7842395226596f,-0.5270139156897f,0.0f,0.0f,0.0f,0.0f,
		0.846053667861f,-0.4490743413053f,0.0f,0.0f,0.0f,0.0f,
		0.8836796692879f,-0.376509909982f,0.0f,0.0f,0.0f,0.0f,
		0.9132429561233f,-0.2851324779451f,0.0f,0.0f,0.0f,0.0f,
		0.9347435283673f,-0.2018177604998f,0.0f,0.0f,0.0f,0.0f,
		0.9508689575503f,-0.1185030430545f,0.0f,0.0f,0.0f,0.0f,
		0.9616192436722f,-0.0459386117311f,0.0f,0.0f,0.0f,0.0f,
		0.957557208227f,0.005529647531f,0.0f,0.0f,0.0f,0.0f,
		0.9566376313267f, 0.0822263604963f, 0.0f, 0.0f, 0.0f, 0.0f

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 temporal = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES
	glm::mat4 Centro = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES
	glm::mat4 circuloA = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES
	glm::mat4 EdiIzquierdo = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES
	glm::mat4 EdifDerecho = glm::mat4(1.0f);	//DECLARACIÓN DE MATRICES

	
	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
							//(izq, der, inf, sup, cerca, lejos)
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f); //Tambien modiificamos el origen de los objetos
	
	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));	//Matriz de traslación (x,y,z) Mueve las imagenes
	// pass them to the shaders
	projectionShader.setMat4("model", model);	//Matrices de 4x4
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//projectionShader.setVec3("aColor", glm::uvec3(1.0f, 1.0f, 0.0f));
	//Trapesio
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	//glDrawArrays(GL_TRIANGLE_FAN, 3, 3);
	//TRIANGULO
	//glDrawArrays(GL_TRIANGLE_FAN, 6, 3);
	//CIRCULO
	//glDrawArrays(GL_TRIANGLE_FAN, 15, 84);
	//RECTANGULO
	//glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	//glDrawArrays(GL_TRIANGLE_FAN, 12, 3);
	//Rectangulo
	Centro = temporal = model = glm::translate(model, glm::vec3(0.0f, 3.5f, 0.0f));
	model = glm::scale(model, glm::vec3(6.0f, 7.0f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);
	
	//Rectangulo Puerta
	temporal = model = glm::translate(temporal, glm::vec3(-0.5f, -2.0f, 0.001f));
	model = glm::scale(model, glm::vec3(1.0f, 3.0f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.2f, 0.1f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	//Triangulo puerta
	temporal = model = glm::translate(temporal, glm::vec3(0.0f, 1.63f, 0.001f));
	model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.2f, 0.1f));
	glDrawArrays(GL_TRIANGLE_FAN, 6, 3);

	//Triangulo Casa
	model = glm::translate(Centro, glm::vec3(0.0f, 3.9f, 0.001f));
	model = glm::scale(model, glm::vec3(6.0f, 0.8f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 6, 3);

	//Circulo centro
	temporal = model = glm::translate(Centro, glm::vec3(0.0f, 1.7f, 0.001f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.7f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 15, 84);

	//Circulo izq
	temporal = model = glm::translate(temporal, glm::vec3(-1.8f, 0.0f, 0.001f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.7f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 15, 84);

	//Circulo derecho
	temporal = model = glm::translate(temporal, glm::vec3(3.6f, 0.0f, 0.001f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.7f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 15, 84);

	//Rectangulo Ventana chiquita centro
	temporal = model = glm::translate(Centro, glm::vec3(2.5f, -2.0f, 0.001f));
	model = glm::scale(model, glm::vec3(1.0f, 1.5f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.7f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	//Rectangulo Edificio izaiero
	EdiIzquierdo = model = glm::translate(Centro, glm::vec3(-4.5f, -1.75f, 0.000f));
	model = glm::scale(model, glm::vec3(3.0f, 3.5f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	//Rectangulo ventana izquierda
	model = glm::translate(EdiIzquierdo, glm::vec3(0.0f, 0.0f, 0.001f));
	model = glm::scale(model, glm::vec3(2.0f, 1.5f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.7f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	//Rectangulo Edificio derecjo
	EdifDerecho = model = glm::translate(Centro, glm::vec3(4.8f, -1.85f, 0.000f));
	model = glm::scale(model, glm::vec3(3.6f, 3.3f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	//Triangulo garage
	model = glm::translate(EdifDerecho, glm::vec3(0.0f, 2.1f, 0.000f));
	model = glm::scale(model, glm::vec3(3.6f, 0.9f, 0.0f));
	model = glm::rotate(model, 3.1416f, glm::vec3(0.0f, 1.0f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_TRIANGLE_FAN, 6, 3);
	
	//Garage
	model = glm::translate(EdifDerecho, glm::vec3(0.0f, -0.15f, 0.001f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 0.0f));
	model = glm::rotate(model, 3.1416f, glm::vec3(0.0f, 1.0f, 0.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.55f, 0.55f, 0.55f));
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);


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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 3", NULL, NULL);
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
		display();

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)  //Cada vez que presionamos la tecla A, decrementa el valor en 0.1
		movX -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)  //Cada vez que presionamos la tecla A, decrementa el valor en 0.1
		movX += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)  //Cada vez que presionamos la tecla A, decrementa el valor en 0.1
		movY += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)  //Cada vez que presionamos la tecla A, decrementa el valor en 0.1
		movY -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)  //Cada vez que presionamos la tecla A, decrementa el valor en 0.1
		movZ += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  //Cada vez que presionamos la tecla A, decrementa el valor en 0.1
		movZ -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		circulo += 1.08f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		circulo -= 1.08f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}