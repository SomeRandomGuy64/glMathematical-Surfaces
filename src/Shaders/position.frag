#ifdef CPP_SHADER_INCLUDE
const char* positionFrag = R"(#version 330 core

in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColour;

void main() {
	FragColour = vec4(FragPos + 0.5, 1.0);
}

)";
#endif