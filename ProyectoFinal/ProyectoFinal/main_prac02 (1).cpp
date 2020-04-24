/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*-------------  Joya Venegas Jehosua Alan ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint shaderProgramRed, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec4 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderRed = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 0.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	

	float vertices[] = 
	{
		4.079f,	15.305f,0.0f,	0.59f, 0.22f, 0.12f,
		7.90f,	15.680f,0.0f,	0.59f, 0.22f, 0.12f,
		8.0f,	10.0f,0.0f,		0.59f, 0.22f, 0.12f,

		4.079f,	15.305f,0.0f,	0.59f, 0.22f, 0.12f,
		7.90f,	15.680f,0.0f,	0.59f, 0.22f, 0.12f,
		4.86f,	9.59f,0.0f,		0.59f, 0.22f, 0.12f,


	};

	unsigned int indices[] =
	{
		0, 1, 4, 2, 3
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

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &myFragmentShaderRed, NULL);
	glCompileShader(fragmentShaderRed);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderRed);
	glDeleteShader(fragmentShaderColor);

}

void display(void)
{
	glUseProgram(shaderProgramColor);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glPointSize(5.0);
	//glDrawElements(GL_POINTS, 5, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINE_STRIP, 0, 5); //como voy a dibujar, vertice incial y cuiantos elemnetos
	glDrawArrays(GL_LINE_LOOP, 0, 8); //Cierra la figura
	glDrawArrays(GL_LINE_LOOP, 8, 12); //Cierra la figura
	glDrawArrays(GL_LINE_LOOP, 20, 12); //Cierra la figura
	glDrawArrays(GL_LINE_LOOP, 62, 10); //Cierra la figura
	//glDrawArrays(GL_TRIANGLES, 16, 5); //genera triangulos
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //genera triangulos siguiendo una continuidad
	//glDrawArrays(GL_TRIANGLE_FAN, 12, 4); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	//glDrawArrays(GL_QUADS, 0, 4); //Genera cuadrados
	//glDrawArrays(GL_POLYGON, 0, 8);//Poligonos
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_LINES, 2, 3);
	//*******USO DE INDICES
	//glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0);

	//#########COLOREANDO C
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_QUADS, 4, 4); //Genera cuadrados
	glDrawArrays(GL_TRIANGLES, 32, 3); //genera triangulos

	//COLOREAR J
	glDrawArrays(GL_TRIANGLE_FAN, 11, 5); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLE_FAN, 35, 5); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLE_FAN, 40, 4); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLES, 45, 3); //genera triangulos

	//COLOREAR J
	glDrawArrays(GL_TRIANGLE_FAN, 23, 5); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLE_FAN, 48, 5); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLE_FAN, 53, 4); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLES, 58, 3); //genera triangulos

	//COLOREANDO ESTRELLA
	glDrawArrays(GL_TRIANGLE_FAN, 61, 11); //Forma triangulos siempre con un mismo vertice (La sgudna posicion)
	glDrawArrays(GL_TRIANGLES, 72, 3); //genera triangulos


	glBindVertexArray(0);

	glUseProgram(0);

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2", NULL, NULL);
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
	//Configurar Shaders
	setupShaders();
    

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}