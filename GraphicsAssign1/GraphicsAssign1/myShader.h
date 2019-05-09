#pragma once
#include<string>
#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyShader {
public:
	static GLuint myshader;
	static GLuint GetShader() {
		static bool isShaderGenerated = false;
		if (!isShaderGenerated)
		{
			const char* vertexShaderSource = "#version 450 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aNormal;\n"
				"layout (location = 2) in vec2 aTexCoord;\n"
				"uniform mat4 View;\n"
				"uniform mat4 Projection;\n"
				"uniform mat4 Model;\n"
				"uniform vec3 LightPosition;\n"
				"uniform vec3 ViewPos;\n"
				"out vec2 TexCoord;\n"
				"out vec3 FragPos;\n"
				"out vec3 fN;\n"
				"out vec3 fE;\n"
				"out vec3 fL;\n"
				"void main()\n"
				"{\n"
				"   FragPos = vec3(Model * vec4(aPos, 1.0));\n"
				"	TexCoord = aTexCoord;\n"
				"	fN = aNormal;\n"
				"	fE = ViewPos-aPos;\n"
				"	fL = LightPosition-aPos;\n"
				"   gl_Position = (Projection * View * vec4(FragPos, 1.0));\n"
				"}\0";

			const char* fragmentShaderSource = "#version 450 core\n"
				"out vec4 FragColor;\n"
				"in vec2 TexCoord;\n"
				"in vec3 FragPos;\n"
				"in vec3 fN;\n"
				"in vec3 fE;\n"
				"in vec3 fL;\n"
				"uniform vec4 myColor;\n"

				"uniform float ambientIaka;\n"
				"uniform float diffuseIpkd;\n"
				"uniform float specularIpks;\n"

				"uniform int numOfTexture;\n"
				"uniform sampler2D diffuseTexture;\n"
				"uniform sampler2D specularTexture;\n"
				"uniform sampler2D normalTexture;\n"
				"void main()\n"
				"{\n"
				"   vec3 N = normalize(fN);\n"
				"   vec3 E = normalize(fE);\n"
				"   vec3 L = normalize(fL);\n"
				"	vec3 H = normalize( L + E );\n"
				"   if(numOfTexture>0)\n"
				"		FragColor = texture(diffuseTexture, TexCoord);\n"
				"   else\n"
				"		FragColor = myColor;\n"
				"   FragColor = (ambientIpkp + diffuseIpkp + specular) * FragColor;\n"
				"}\n\0";
			//code copy from here

	/* make shaders */
	//compile vertex shader
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			//compile fragment shader
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			//debug compile completed
			{
				int success;
				char infoLog[512];
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				assert(success == GL_TRUE);
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				assert(success == GL_TRUE);
			}
			//shader link
			myshader = glCreateProgram();
			glAttachShader(myshader, vertexShader);
			glAttachShader(myshader, fragmentShader);
			glLinkProgram(myshader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			isShaderGenerated = true;
		}
		return myshader;
	}

	static void setInt(const std::string& name, const GLuint value) //https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	{
		glUniform1i(glGetUniformLocation(myshader, name.c_str()), value);
	}
	static void setVec3(const std::string& name, const glm::vec3& value) //https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	{
		glUniform3fv(glGetUniformLocation(myshader, name.c_str()), 1, &value[0]);
	}
	static void setVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(myshader, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	static void setVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(myshader, name.c_str()), 1, &value[0]);
	}
	static void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(myshader, name.c_str()), x, y, z, w);
	}
	static void setMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(myshader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	static void setMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(myshader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	static void setTexture(const std::string& name, const GLuint n)
	{
		glUniform1i(glGetUniformLocation(myshader, name.c_str()), n);
	}
};