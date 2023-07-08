#include <stdio.h>
#include <stdlib.h>
#include <glad.h>
#include <glfw3.h>


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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0,0,width,height);
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





int main() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
    glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GL functions!\n");
		exit(-1);
	}

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.37f, 0.0f
	};

	unsigned int shader_program;
	shader_program = create_shader_program("resources/vert.glsl", "resources/frag.glsl");

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(shader_program);


    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}