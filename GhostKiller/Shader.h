#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

static const GLchar* vertexCode3D =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec2 texCoord;\n"
"out vec4 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform vec4 color;\n"
"uniform vec2 coordScale;\n"
"void main(){\n"
"gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"ourColor = color;\n"
"TexCoord = vec2(texCoord.x * coordScale.x, (1.0 - texCoord.y) * coordScale.y);\n"
";}";

static const GLchar* fragmentCode3D =
"#version 330 core\n"
"in vec4 ourColor;\n"
"in vec2 TexCoord;\n"
"out vec4 color;\n"
"uniform sampler2D ourTexture;\n"
"void main(){\n"
"color = texture(ourTexture, TexCoord) * ourColor ;}";

static const GLchar* vertexCode2D =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec2 texCoord;\n"
"out vec4 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform vec4 color;\n"
"uniform vec2 coordScale;\n"
"void main(){\n"
"gl_Position = model * vec4(position, 1.0f);\n"
"ourColor = color;\n"
"TexCoord = vec2(texCoord.x * coordScale.x, (1.0 - texCoord.y) * coordScale.y);\n"
";}";

static const GLchar* fragmentCode2D =
"#version 330 core\n"
"in vec4 ourColor;\n"
"in vec2 TexCoord;\n"
"out vec4 color;\n"
"uniform sampler2D ourTexture;\n"
"void main(){\n"
"color = texture(ourTexture, TexCoord) * ourColor;}";

GLuint makeProgram(const GLchar* vertexCode, const GLchar* fragmentCode);
void useProgram(GLuint program);

#endif
