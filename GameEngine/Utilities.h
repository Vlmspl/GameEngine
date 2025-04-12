#pragma once
#include <glm/glm.hpp>

typedef glm::vec2 Vector2f;
typedef glm::vec3 Vector3f;
typedef glm::vec4 Vector4f;

typedef glm::mat4 Matrix4f;
typedef glm::mat4x4 Matrix4d;


enum ApplicationStatusCode {
	SUCCESS = 0,
	WINDOW_INITIALIZATION_FAIL = 1,

	UNKNOWN_ERROR = -1
};


inline float volumeBoxPosition[] = {
	1.0, 1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0, 1.0, 1.0,
	1.0, -1.0, 1.0,
	-1.0, 1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, 1.0, 1.0,
	-1.0, -1.0, 1.0,

};

inline float volumeBoxUVW[] = {
    1.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0
};

inline unsigned int volumeBoxIndices[] = {
	4, 2, 0,
	2, 7, 3,

	6, 5, 7,
	1, 7, 5,

	0, 3, 1,
	4, 1, 5,

	4, 6, 2,
	2, 6, 7,

	6, 4, 5,
	1, 3, 7,

	0, 2, 3,
	4, 0, 1
};