#include <stdio.h>
#include <stdlib.h>
#include <glad.h>
#include <glfw3.h>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void CheckShader(GLuint id, GLuint type, GLint *ret, const char *onfail) { 
	//Check if something is wrong with the shader 
	switch(type) { 
		case(GL_COMPILE_STATUS): 
			glGetShaderiv(id, type, ret); 
			if(!(*ret)) { 
				int infologLength = 0; 
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength); 
				GLchar buffer[infologLength]; 
				GLsizei charsWritten = 0; 
				printf("%s\n", onfail);  
				glGetShaderInfoLog(id, infologLength, &charsWritten, buffer); 
				printf("%s\n", buffer);  
			} 
			break; 
		case(GL_LINK_STATUS): 
			glGetProgramiv(id, type, ret); 
			if(!(*ret)) { 
				int infologLength = 0; 
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength); 
				GLchar buffer[infologLength]; 
				GLsizei charsWritten = 0; 
				printf("%s\n", onfail); 
				glGetProgramInfoLog(id, infologLength, &charsWritten, buffer); 
				printf("%s\n", buffer); 
			} 
			break; 
		default: 
			break; 
	}; 
}

char *load_from_file(const char* path) {
	char *buffer;
	int size;
	FILE *file = fopen(path, "rb");
	fseek(file,0,SEEK_END);
	size = ftell(file);
	rewind(file);
	buffer = (char*) malloc(size+1);
	buffer[size] = '\0';
	fread(buffer,1,size,file);
	fclose(file);
	return buffer;
}

unsigned int create_shader_program(const char * vert, const char *frag) {
	char *vsrc, *fsrc;
	vsrc = load_from_file(vert);
	fsrc = load_from_file(frag);

	GLint ret;
	unsigned int vertshader, fragshader;
	vertshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertshader, 1, &vsrc, NULL);
	glCompileShader(vertshader);
	CheckShader(vertshader, GL_COMPILE_STATUS, &ret, "unable to compile the vertex shader!");
    printf("penis\n");

	fragshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragshader, 1, &fsrc, NULL);
	glCompileShader(fragshader);
	CheckShader(fragshader, GL_COMPILE_STATUS, &ret, "unable to compile the fragment shader!");

	free(vsrc);
	free(fsrc);
	               
	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vertshader);
	glAttachShader(program, fragshader);
	glLinkProgram(program);

	// Link our program, and set it as being actively used
	CheckShader(program, GL_LINK_STATUS, &ret, "unable to link the program!");
	

	glDeleteShader(vertshader);
	glDeleteShader(fragshader);
	return program;
}


void set_uniform_float(unsigned int program_id, float value, const char *name) {	// Sets a uniform of type float for the specified shader program
	int location = glGetUniformLocation(program_id, name);
	glUniform1f(location, value);
}


int main() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GL functions!\n");
		exit(-1);
	}

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,	0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 2, 3, 0, 1, 3
	};

	unsigned int shader_program;
	shader_program = create_shader_program("resources/vert.glsl", "resources/frag.glsl");

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(shader_program);

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	char *data;
	FILE *f = fopen("pic.bm", "rb");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	data = (char*) malloc(size + 1);
	data[size] = '\0';
	fread(data, 1, size, f);
	fclose(f);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 90, 90, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// float alpha = 0;

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

		// set_uniform_float(shader_program, alpha, "nice");
		// alpha += 0.5;

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(shader_program);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}