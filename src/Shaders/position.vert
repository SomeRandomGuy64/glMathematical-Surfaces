#ifdef CPP_SHADER_INCLUDE
const char* positionVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 xTimeZ; // x = xIndex, y = deltaTime, z = zIndex

const float PI = 3.1415926;

out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

float u = xTimeZ.x * 0.005;
float v = xTimeZ.z * 0.01;
float r1 = 0.7 + 0.1 * sin(PI * (8.0 * u + 0.5 * xTimeZ.y));
float r2 = 0.15 + 0.05 * sin(PI * (16.0 * u + 8.0 * v + 3.0 * xTimeZ.y));
float s = 0.5 + r1 + r2 * cos(PI * v);

const float scale = 0.0075;

mat4 modelMat = mat4(
scale, 0.0,   0.0,   0.0,
0.0,   scale, 0.0,   0.0,
0.0,   0.0,   scale, 0.0,
s * sin(PI * u), r2 * sin(PI * v), s * cos(PI * u), 1.0
);

void main() {
	gl_Position = projection * view * modelMat * vec4(aPos, 1.0);
	FragPos = vec3(modelMat * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
}

)";
#endif