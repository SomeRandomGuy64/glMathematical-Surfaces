#ifdef CPP_SHADER_INCLUDE
const char* positionVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTranslate;
layout (location = 4) in vec3 aScale;

out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

mat4 model;

mat4 transMat = mat4(
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	aTranslate.x, aTranslate.y, aTranslate.z, 1.0
);

mat4 scaleMat = mat4(
	aScale.x, 0.0, 0.0, 0.0,
	0.0, aScale.y, 0.0, 0.0,
	0.0, 0.0, aScale.z, 0.0,
	0.0, 0.0, 0.0, 1.0
);

void main() {
	model = mat4(aScale.x);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
}

)";
#endif