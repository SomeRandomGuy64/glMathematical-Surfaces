#pragma once

namespace Shapes {
	static inline constexpr float cube[]{
		// positions; offset: 0		//normals; offset: 12		//texture coordinates; offset: 24
		// back face
		-0.5f, -0.5f, -0.5f,		0.0f,	0.0f,  -1.0f,		0.0f,	0.0f,
		 0.5f,  0.5f, -0.5f,		0.0f,	0.0f,  -1.0f,		1.0f,	1.0f,
		 0.5f, -0.5f, -0.5f,		0.0f,	0.0f,  -1.0f,		1.0f,	0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f,	0.0f,  -1.0f,		0.0f,	1.0f,

		// front face
		-0.5f, -0.5f,  0.5f,		0.0f,	0.0f,   1.0f,		0.0f,	0.0f,
		 0.5f, -0.5f,  0.5f,		0.0f,	0.0f,   1.0f,		1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,		0.0f,	0.0f,   1.0f,		1.0f,	1.0f,
		-0.5f,  0.5f,  0.5f,		0.0f,	0.0f,   1.0f,		0.0f,	1.0f,

		// left face
		-0.5f,  0.5f,  0.5f,	   -1.0f,	0.0f,   0.0f,		1.0f,	0.0f,
		-0.5f,	0.5f, -0.5f,	   -1.0f,	0.0f,   0.0f,		1.0f,	1.0f,
		-0.5f, -0.5f, -0.5f,	   -1.0f,	0.0f,   0.0f,		0.0f,	1.0f,
		-0.5f, -0.5f,  0.5f,	   -1.0f,	0.0f,   0.0f,		0.0f,	0.0f,

		// right face
		 0.5f,  0.5f,  0.5f,	    1.0f,	0.0f,   0.0f,		1.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,	    1.0f,	0.0f,   0.0f,		0.0f,	1.0f,
		 0.5f,  0.5f, -0.5f,	    1.0f,	0.0f,   0.0f,		1.0f,	1.0f,
		 0.5f, -0.5f,  0.5f,	    1.0f,	0.0f,   0.0f,		0.0f,	0.0f,

		 // bottom face
		-0.5f, -0.5f, -0.5f,	    0.0f,  -1.0f,   0.0f,		0.0f,	1.0f,
		 0.5f, -0.5f, -0.5f,	    0.0f,  -1.0f,   0.0f,		1.0f,	1.0f,
		 0.5f, -0.5f,  0.5f,	    0.0f,  -1.0f,   0.0f,		1.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	    0.0f,  -1.0f,   0.0f,		0.0f,	0.0f,

		// top face
		-0.5f,  0.5f, -0.5f,	    0.0f,   1.0f,   0.0f,		0.0f,	1.0f,
		 0.5f,  0.5f,  0.5f,	    0.0f,   1.0f,   0.0f,		1.0f,	0.0f,
		 0.5f,  0.5f, -0.5f,	    0.0f,   1.0f,   0.0f,		1.0f,	1.0f,
		-0.5f,  0.5f,  0.5f,	    0.0f,   1.0f,   0.0f,		0.0f,	0.0f
	};

	static inline constexpr unsigned int cubeIndices[]{
		// back face
		0, 1, 2,
		1, 0, 3,

		// front frace
		4, 5, 6,
		6, 7, 4,

		// left face
		8, 9, 10,
		10, 11, 8,

		// right face
		12, 13, 14,
		13, 12, 15,

		// bottom face
		16, 17, 18,
		18, 19, 16,

		// top face
		20, 21, 22,
		21, 20, 23
	};
}