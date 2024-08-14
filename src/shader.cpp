#include <shader.h>

#include <iostream>

Shader& Shader::use() {
	glUseProgram(m_ID);
	return *this;
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
	unsigned int sVertex{};
	unsigned int sFragment{};
	unsigned int sGeometry{};

	// vertex shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, nullptr);
	glCompileShader(sVertex);
	m_checkCompileErrors(sVertex, "VERTEX");

	// fragment shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, nullptr);
	glCompileShader(sFragment);
	m_checkCompileErrors(sFragment, "FRAGMENT");

	// if geometry shader source code is given, also compile geometry shader
	if (geometrySource != nullptr) {
		sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(sGeometry, 1, &geometrySource, nullptr);
		glCompileShader(sGeometry);
		m_checkCompileErrors(sGeometry, "GEOMETRY");
	}

	// shader program
	m_ID = glCreateProgram();
	glAttachShader(m_ID, sVertex);
	glAttachShader(m_ID, sFragment);
	if (geometrySource != nullptr) {
		glAttachShader(m_ID, sGeometry);
	}
	glLinkProgram(m_ID);
	m_checkCompileErrors(m_ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr) {
		glDeleteShader(sGeometry);
	}
}

void Shader::setFloat(const char* name, float value, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform1f(glGetUniformLocation(m_ID, name), value);
}

void Shader::setInteger(const char* name, int value, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform1i(glGetUniformLocation(m_ID, name), value);
}

void Shader::setVector2f(const char* name, float x, float y, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform2f(glGetUniformLocation(m_ID, name), x, y);
}

void Shader::setVector2f(const char* name, const glm::vec2& value, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform2f(glGetUniformLocation(m_ID, name), value.x, value.y);
}

void Shader::setVector3f(const char* name, float x, float y, float z, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform3f(glGetUniformLocation(m_ID, name), x, y, z);
}

void Shader::setVector3f(const char* name, const glm::vec3& value, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform3f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const char* name, float x, float y, float z, float w, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform4f(glGetUniformLocation(m_ID, name), x, y, z, w);
}

void Shader::setVector4f(const char* name, const glm::vec4& value, bool useShader) {
	if (useShader) {
		use();
	}
	glUniform4f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix2(const char* name, const glm::mat2& matrix, bool useShader) {
	if (useShader) {
		use();
	}
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::setMatrix3(const char* name, const glm::mat3& matrix, bool useShader) {
	if (useShader) {
		use();
	}
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::setMatrix4(const char* name, const glm::mat4& matrix, bool useShader) {
	if (useShader) {
		use();
	}
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, false, glm::value_ptr(matrix));
}

unsigned int Shader::getId() {
	return m_ID;
}

void Shader::m_checkCompileErrors(unsigned int object, std::string type) {
	int success{};
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, nullptr, infoLog);
			std::cerr << "| ERROR::SHADER: Compile-time error: Type: " << type << '\n'
				<< infoLog << "\n -- --------------------------------------------------- -- \n";
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, nullptr, infoLog);
			std::cerr << "| ERROR::Shader: Link-time error: Type: " << type << '\n'
				<< infoLog << "\n -- --------------------------------------------------- -- \n";
		}
	}
}