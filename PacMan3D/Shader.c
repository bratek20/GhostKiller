#include <GL/glew.h>
#include <stdio.h>

GLuint makeProgram(const GLchar* vShaderCode, const GLchar * fShaderCode){
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s \n" ,infoLog);
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s \n", infoLog);
    }
    // Shader Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // Print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s \n", infoLog);
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}


void useProgram(GLuint program) {
    glUseProgram(program);
}