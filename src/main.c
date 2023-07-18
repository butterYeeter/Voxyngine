#include <stdio.h>
#include <stdlib.h>
#include <glad.h>
#include <glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
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

// unsigned int *create_texture_array(const char **texture_paths, const int num_textures) {
// 	unsigned int *texture_objects = (int*) malloc(sizeof(int) * num_textures);
// 	glGenTextures(num_textures, texture_objects);

// 	char **textures = (char**) malloc(num_textures * sizeof(char*));
// 	int *widths = (int*) malloc(num_textures * sizeof(int));
// 	int *heights = (int*) malloc(num_textures * sizeof(int));
// 	int *channels = (int*) malloc(num_textures * sizeof(int));

// 	for(int i = 0; i < num_textures; i++) {
// 		textures[i] = stbi_load(texture_paths[i], &widths[i], &heights[i], &channels[i], 0);
// 	}

// 	for(int i = 0; i < num_textures; i++) {
// 		glBindTexture(GL_TEXTURE_2D, texture_objects[i]);
// 		int channel;
// 		// if(channels[i] == 3) channel = GL_RGB; else if(channels[i] == 4) channel = GL_RGBA;
		
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widths[i], heights[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]);
// 		glGenerateMipmap(GL_TEXTURE_2D); 

// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

// 		stbi_image_free(textures[i]);
// 	}

// 	free(textures);
// 	free(widths);
// 	free(heights);
// 	free(channels);

// 	return texture_objects;
// }

void create_texture_object(unsigned int *texture_object, const char* image_path) {
	int width, height, num_channels;
	char *data = stbi_load(image_path, &width, &height, &num_channels, 0);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, texture_object);
	glBindTexture(GL_TEXTURE_2D, *texture_object);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
		-200.0f/800.0f, -200.0f/600.0f, 0.0f,	0.0f, 1.0f,
		200.0f/800.0f, -200.0f/600.0f, 0.0f,	1.0f, 1.0f,
		-200.0f/800.0f, 200.0f/600.0f, 0.0f,	0.0f, 0.0f,
		200.0f/800.0f, 200.0f/600.0f, 0.0f,		1.0f, 0.0f
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	unsigned int *sprite = (unsigned int*) malloc(4 * sizeof(unsigned int));
	create_texture_object(sprite  , "resources/Sprite-0001.png");
	create_texture_object(sprite+1, "resources/Sprite-0002.png");
	create_texture_object(sprite+2, "resources/Sprite-0003.png");
	create_texture_object(sprite+3, "resources/Sprite-0004.png");


	// float alpha = 0;
	float last_time = glfwGetTime();
	int index = 0;
	glBindTexture(GL_TEXTURE_2D, sprite[index]);


    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// set_uniform_float(shader_program, alpha, "nice");
		// alpha += 0.5;

		glBindVertexArray(VAO);

		float time = glfwGetTime();

		if(time - last_time > 0.11f) {
			if(index == 3) index = 0; else index++;
			glBindTexture(GL_TEXTURE_2D, sprite[index]);
			last_time = time;
		}
		
		glUseProgram(shader_program);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


	free(sprite);

    glfwTerminate();
    return 0;
}