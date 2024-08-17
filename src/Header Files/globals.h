#pragma once

namespace GLOBALS {
	// window settings
	static inline constexpr unsigned int SCR_WIDTH{ 1600 };
	static inline constexpr unsigned int SCR_HEIGHT{ 900 };

	// constants
	static inline constexpr float PI{ 3.1415926f };

	// mouse stuff
	static inline float lastX{ SCR_WIDTH / 2.0f };
	static inline float lastY{ SCR_HEIGHT / 2.0f };
	static inline bool firstMouse{ true };

	// delta time
	static inline float deltaTime{ 0.0f };
	static inline float lastFrame{ 0.0f };
}