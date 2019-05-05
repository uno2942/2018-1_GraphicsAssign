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
			const char* vertexShaderSource = "#version 460 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aNormal;\n"
				"uniform mat4 View;\n"
				"uniform mat4 Projection;\n"
				"uniform mat4 Model;\n"
				"out vec3 worldPos;\n"
				"out vec3 normal;\n"
				"void main()\n"
				"{\n"
				"   gl_Position = (Projection * View * Model) * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
				"	vec4 worldPos4 = Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
				"	worldPos = vec3(worldPos4.x, worldPos4.y, worldPos4.z) / worldPos4.w;\n"
				"	normal = aNormal;\n"
				"}\0";

			const char* fragmentShaderSource = "#version 460 core\n"
				"in vec3 worldPos;\n"
				"in vec3 normal;\n"

				"out vec4 FragColor;\n"
				
				"uniform vec4 myColor;\n"
				"uniform vec3 lightPos;\n"
				"uniform vec3 lightColor;\n"

				"uniform vec4 ambient;\n"
				"uniform vec4 specular;\n"
				"void main()\n"
				"{\n"
				"	vec3 lightDir = vec3(0, 0, 1);\n"
				"	vec3 norm = normalize(normal);\n"
				"	float diff = max(dot(norm, lightDir), 0.0);\n;"
				"   FragColor = (ambient + vec4(diff * lightColor, 1.0) + specular) * myColor;\n"
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
};