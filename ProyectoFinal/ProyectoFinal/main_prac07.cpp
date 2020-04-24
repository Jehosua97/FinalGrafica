/*---------------------------------------------------------*/
/* ----------------   Práctica 7 --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*------------- Alumno: JOYA VENEGAS JEHOSUA ALAN	-------*/
#include "esfera.h"
#include "camera.h"

Esfera my_sphere(1.0f);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, lightVAO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
/*double	lastX = 0.0f,
		lastY = 0.0f;*/
GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);		//POSICION DE LA FUENTE DE LUZ

void myData(void);
void display(Shader, Shader);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

float	sol = 0.0f,
		year = 0.0f,
		day = 0.0f,
		moon = 0.0f,
		mars_year = 0.0f,
		jupiter_year = 0.0f,
		yearTierra = 0.0f;


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		//cara superior del cubo
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//To configure Second Object to represent Light
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void animate(void)
{
	sol += 0.05f;
	year -= 0.08f;
	yearTierra += 0.05f;
	moon += 0.07f;
	day -= 0.04f;
	mars_year += 0.05f;
	jupiter_year -= 0.04f;




}

void display(Shader shaderProjection, Shader shaderLamp)
{
	
	//To Use Lighting
	shaderProjection.use();
	shaderProjection.setVec3("lightColor", 1.0f, 1.0f, 1.0f);					//COLOR DE LA FUENTE DE LUZ
	shaderProjection.setVec3("lightPos", lightPos);								//LE PASAMOS LA POSICION

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 temporal = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models


	

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shaderProjection.setVec3("viewPos", camera.Position);
	shaderProjection.setMat4("model", model);
	shaderProjection.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shaderProjection.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	shaderLamp.use();												//Para utilizar un shader con color directo
	shaderLamp.setMat4("projection", projection);
	shaderLamp.setMat4("view", view);
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shaderLamp.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);			//COLOR DE LAS QUE ESTAN MENOS ILUMINADAS
	shaderProjection.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	shaderProjection.setVec3("specularColor", 1.0f, 1.0f, 1.0f);		//EL BRILLO QUE LLEGAN A TENER LOS OBJETOS
	my_sphere.render();	//Sol

	shaderProjection.use();
	model = glm::rotate(glm::mat4(1.0f), glm::radians(year), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(0.0f, 0.0f, 1.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);			//COLOR DE LAS QUE ESTAN MENOS ILUMINADAS
	shaderProjection.setVec3("diffuseColor", 1.0f, 0.2f, 0.3f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	shaderProjection.setVec3("specularColor", 0.0f, 1.0f, 0.0f);		//EL BRILLO QUE LLEGAN A TENER LOS OBJETOS
	//shaderProjection.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f));
	my_sphere.render();	//Mercurio

	model = glm::rotate(glm::mat4(1.0f), glm::radians(yearTierra), glm::vec3(0.0f, 1.0f, 0.0f));
	temporal = model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(0.0f, 0.0f, 1.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 0.4f, 1.0f, 1.0f);			//COLOR DE LAS QUE ESTAN MENOS ILUMINADAS
	shaderProjection.setVec3("diffuseColor", 0.1f, 0.3f, 1.0f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	shaderProjection.setVec3("specularColor", 0.0f, 1.0f, 0.5f);		//EL BRILLO QUE LLEGAN A TENER LOS OBJETOS
	my_sphere.render();	//Tierra

	model = glm::rotate(temporal, glm::radians(moon), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(1.0f, 0.0f, 0.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 0.0f, 1.0f, 1.0f);			//COLOR DE LAS QUE ESTAN MENOS ILUMINADAS
	shaderProjection.setVec3("diffuseColor", 0.9f, 0.9f, 1.0f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	shaderProjection.setVec3("specularColor", 0.2f, 1.0f, 0.5f);		//EL BRILLO QUE LLEGAN A TENER LOS OBJETOS
	my_sphere.render();	//Luna

	model = glm::rotate(glm::mat4(1.0f), glm::radians(mars_year), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(16.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(0.0f, 0.0f, 1.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 0.6f, 1.0f, 0.5f);			//COLOR DE LAS QUE ESTAN MENOS ILUMINADAS
	shaderProjection.setVec3("diffuseColor", 1.0f, 0.3f, 1.0f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	shaderProjection.setVec3("specularColor", 0.8f, 1.0f, 0.5f);		//EL BRILLO QUE LLEGAN A TENER LOS OBJETOS
	my_sphere.render();	//Marte

	model = glm::rotate(glm::mat4(1.0f), glm::radians(-14.5f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(jupiter_year), glm::vec3(0.0f, 1.0f, 0.0f)); //GIRANDO SOBRE EL NUEEVO EJE Y
	model = glm::translate(model, glm::vec3(21.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
	model = glm::rotate(model, glm::radians(day), glm::vec3(0.0f, 0.0f, 1.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 0.1f, 1.0f, 0.2f);			//COLOR DE LAS QUE ESTAN MENOS ILUMINADAS
	shaderProjection.setVec3("diffuseColor", 0.1f, 1.0f, 0.4f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	shaderProjection.setVec3("specularColor", 1.0f, 1.0f, 0.4f);		//EL BRILLO QUE LLEGAN A TENER LOS OBJETOS
	my_sphere.render();	//Jupiter

	/*model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("diffuseColor", 1.0f, 1.0f, 0.4f);			//EL COLOR DE LAS CARAS MAS ILUMINADAS
	glDrawArrays(GL_TRIANGLES, 0, 36);*/

	/*shaderLamp.use();
	shaderLamp.setMat4("projection", projection);
	shaderLamp.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.25f));
	shaderLamp.setMat4("model", model);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);	//Light*/


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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 7", NULL, NULL);
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

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_light_Gouraud.vs", "shaders/shader_light_Gouraud.fs");
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");


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
        my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display(projectionShader, lampShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
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
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos.x -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos.x += 0.05f;

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