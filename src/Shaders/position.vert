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

const float scale = 0.0015;

mat4 plane(float u, float v, float t) {

	return mat4(
	scale, 0.0,   0.0,   0.0,
	0.0,   scale, 0.0,   0.0,
	0.0,   0.0,   scale, 0.0,
	u,   1.0,   v,   1.0
	);
}

mat4 wave(float u, float v, float t) {
	return mat4(
		scale, 0.0,   0.0,   0.0,
		0.0,   scale, 0.0,   0.0,
		0.0,   0.0,   scale, 0.0,
		u,     sin(PI * (u + v + t)), v, 1.0
	);
}

mat4 multiWave(float u, float v, float t) {
	vec3 p;
	p.x = u;
	p.y = sin(PI * (u + 0.5 * t));
	p.y += 0.5 * sin (2.0 * PI * (v + t));
	p.y += sin(PI * (u + v + 0.25 * t));
	p.y *= 1.0 / 2.5;
	p.z = v;

	return mat4(
		scale, 0.0,   0.0,   0.0,
		0.0,   scale, 0.0,   0.0,
		0.0,   0.0,   scale, 0.0,
		p.x,   p.y,   p.z,   1.0
	);
}

mat4 ripple(float u, float v, float t) {
	float d = sqrt(u * u + v * v);

	vec3 p;
	p.x = u;
	p.y = sin(PI * (4.0 * d - t));
	p.y /= 1.0 + 10.0 * d;
	p.z = v;

	return mat4(
		scale, 0.0,   0.0,   0.0,
		0.0,   scale, 0.0,   0.0,
		0.0,   0.0,   scale, 0.0,
		p.x,   p.y,   p.z,   1.0
	);
}

mat4 sphere(float u, float v, float t) {
	float r = 0.9 + 0.1 * sin(PI * (12.0 * u + 8.0 * v + t));
	float s = r * cos(0.5 * PI * v);

	vec3 p;
	p.x = s * sin(PI * u);
	p.y = r * sin(PI * 0.5 * v);
	p.z = s * cos(PI * u);

	return mat4(
		scale, 0.0,   0.0,   0.0,
		0.0,   scale, 0.0,   0.0,
		0.0,   0.0,   scale, 0.0,
		p.x,   p.y,   p.z,   1.0
	);
}

mat4 torus(float u, float v, float t) {
	float r1 = 0.7 + 0.1 * sin(PI * (8.0 * u + 0.5 * t));
	float r2 = 0.15 + 0.05 * sin(PI * (16.0 * u + 8.0 * v + 3.0 * t));
	float s = 0.5 + r1 + r2 * cos(PI * v);

	vec3 p;
	p.x = s * sin(PI * u);
	p.y = r2 * sin(PI * v);
	p.z = s * cos(PI * u);

	return mat4(
		scale, 0.0,   0.0,   0.0,
		0.0,   scale, 0.0,   0.0,
		0.0,   0.0,   scale, 0.0,
		p.x,   p.y,   p.z,   1.0
	);
}

float u = xTimeZ.x * sqrt(2.0007) / 1000;
float v = xTimeZ.z * sqrt(2.0007) / 1000;

mat4 mixMat4(mat4 matA, mat4 matB, float t) {
	t = smoothstep(0.0, 1.0, t);
	return (matA * (1.0 - t)) + (matB * t);
}

void main() {
	if (int(xTimeZ.y) % 20 < 3) {
		gl_Position = projection * view * wave(u, v, xTimeZ.y) * vec4(aPos, 1.0);
		FragPos = vec3(wave(u, v, xTimeZ.y) * vec4(aPos, 1.0));
	} 
	else if (int(xTimeZ.y) % 20 == 3) {
		gl_Position = projection * view * mixMat4(wave(u, v, xTimeZ.y), multiWave(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0);
		FragPos = vec3(mixMat4(wave(u, v, xTimeZ.y), multiWave(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0));
	}
	else if (int(xTimeZ.y) % 20 < 7) {
		gl_Position = projection * view * multiWave(u, v, xTimeZ.y) * vec4(aPos, 1.0);
		FragPos = vec3(multiWave(u, v, xTimeZ.y) * vec4(aPos, 1.0));
	} 
	else if (int(xTimeZ.y) % 20 == 7) {
		gl_Position = projection * view * mixMat4(multiWave(u, v, xTimeZ.y), ripple(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0);
		FragPos = vec3(mixMat4(multiWave(u, v, xTimeZ.y), ripple(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0));
	}
	else if (int(xTimeZ.y) % 20 < 11) {
		gl_Position = projection * view * ripple(u, v, xTimeZ.y) * vec4(aPos, 1.0);
		FragPos = vec3(ripple(u, v, xTimeZ.y) * vec4(aPos, 1.0));
	} 
	else if (int(xTimeZ.y) % 20 == 11) {
		gl_Position = projection * view * mixMat4(ripple(u, v, xTimeZ.y), sphere(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0);
		FragPos = vec3(mixMat4(ripple(u, v, xTimeZ.y), sphere(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0));
	}
	else if (int(xTimeZ.y) % 20 < 15) {
		gl_Position = projection * view * sphere(u, v, xTimeZ.y) * vec4(aPos, 1.0);
		FragPos = vec3(sphere(u, v, xTimeZ.y) * vec4(aPos, 1.0));
	} 
	else if (int(xTimeZ.y) % 20 == 15) {
		gl_Position = projection * view * mixMat4(sphere(u, v, xTimeZ.y), torus(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0);
		FragPos = vec3(mixMat4(sphere(u, v, xTimeZ.y), torus(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0));
	}
	else if (int(xTimeZ.y) % 20 < 19) {
		gl_Position = projection * view * torus(u, v, xTimeZ.y) * vec4(aPos, 1.0);
		FragPos = vec3(torus(u, v, xTimeZ.y) * vec4(aPos, 1.0));
	} 
	else {
		gl_Position = projection * view * mixMat4(torus(u, v, xTimeZ.y), wave(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0);
		FragPos = vec3(mixMat4(torus(u, v, xTimeZ.y), wave(u, v, xTimeZ.y), xTimeZ.y - floor(xTimeZ.y)) * vec4(aPos, 1.0));
	}
	TexCoords = aTexCoords;
}

)";
#endif