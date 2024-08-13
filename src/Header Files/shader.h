#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
	// The program ID
	unsigned int m_ID{};

public:
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode{};
		std::string fragmentCode{};
		std::ifstream vShaderFile{};
		std::ifstream fShaderFile{};

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream{};
			std::stringstream fShaderStream{};

			// read the file's buffer contents into stream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
		}

		const char* vShaderCode{ vertexCode.c_str() };
		const char* fShaderCode{ fragmentCode.c_str() };

		// 2. compile shaders
		// vertex shader
		unsigned int vertex{ glCreateShader(GL_VERTEX_SHADER) };
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		//print compile errors if any
		int success{};
		char infoLog[512]{};

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << '\n' << infoLog << '\n';
		}

		// fragment shader
		unsigned int fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);

		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << '\n' << infoLog << '\n';
		}

		// shader program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);

		// print linking errors if any
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << '\n' << infoLog << '\n';
		}

		// delete shaders, they're linked to the program and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// constructor for optional geometry shader
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode{};
		std::string fragmentCode{};
		std::string geometryCode{};
		std::ifstream vShaderFile{};
		std::ifstream fShaderFile{};
		std::ifstream gShaderFile{};

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			gShaderFile.open(geometryPath);
			std::stringstream vShaderStream{};
			std::stringstream fShaderStream{};
			std::stringstream gShaderStream{};

			// read the file's buffer contents into stream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			geometryCode = gShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
		}

		const char* vShaderCode{ vertexCode.c_str() };
		const char* fShaderCode{ fragmentCode.c_str() };
		const char* gShaderCode{ geometryCode.c_str() };

		// 2. compile shaders
		// vertex shader
		unsigned int vertex{ glCreateShader(GL_VERTEX_SHADER) };
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);

		//print compile errors if any
		int success{};
		char infoLog[512]{};

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << '\n' << infoLog << '\n';
		}

		// fragment shader
		unsigned int fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);

		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << '\n' << infoLog << '\n';
		}

		// geometry shader
		unsigned int geometry{ glCreateShader(GL_GEOMETRY_SHADER) };
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);

		// shader program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glAttachShader(m_ID, geometry);
		glLinkProgram(m_ID);

		// print linking errors if any
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << '\n' << infoLog << '\n';
		}

		// delete shaders, they're linked to the program and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// use/activate the shade
	void use() {
		glUseProgram(m_ID);
	}

	// utility uniform functions
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void setVec2(const std::string& name, glm::vec2 value) const {
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
	}

	void setVec3(const std::string& name, glm::vec3 value) const {
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
	}

	void setVec4(const std::string& name, glm::vec4 value) const {
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
	}

	void setMat2(const std::string& name, glm::mat2 value) const {
		glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setMat3(const std::string& name, glm::mat3 value) const {
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setMat4(const std::string& name, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	unsigned int getId() const { return m_ID; }
};