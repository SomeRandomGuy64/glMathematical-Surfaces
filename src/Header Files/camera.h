#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// Default camera values
namespace cameraValues {
    constexpr float YAW{ -90.0f };
    constexpr float PITCH{ 0.0f };
    constexpr float SPEED{ 4.0f };
    constexpr float SENSITIVITY{ 0.1f };
    constexpr float ZOOM{ 45.0f };
}


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Constructor with vectors and eulers
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = cameraValues::YAW, float pitch = cameraValues::PITCH)
        : m_front{ glm::vec3(0.0f, 0.0f, -1.0f) }
        , m_movementSpeed{ cameraValues::SPEED }
        , m_mouseSensitivity{ cameraValues::SENSITIVITY }
        , m_zoom{ cameraValues::ZOOM }
    {

        m_position = position;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : m_front{ glm::vec3(0.0f, 0.0f, -1.0f) }
        , m_movementSpeed{ cameraValues::SPEED }
        , m_mouseSensitivity{ cameraValues::SENSITIVITY }
        , m_zoom{ cameraValues::ZOOM }
    {
        m_position = glm::vec3(posX, posY, posZ);
        m_worldUp = glm::vec3(upX, upY, upZ);
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }

    // Returns the vew matrix calculated using Euler angles and the lookAt Matrix
    glm::mat4 getViewMatrix() {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    virtual void processKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        switch (direction) {
        case FORWARD:   m_position += m_front * velocity;   break;
        case BACKWARD:  m_position -= m_front * velocity;   break;
        case LEFT:      m_position -= m_right * velocity;   break;
        case RIGHT:     m_position += m_right * velocity;   break;
        case UP:        m_position += m_up * velocity;      break;
        case DOWN:      m_position -= m_up * velocity;      break;
        default:        m_position;                         break;
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (m_pitch > 89.0f) {
                m_pitch = 89.0f;
            }
            else if (m_pitch < -89.0f) {
                m_pitch = -89.0f;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) {
        m_zoom -= (float)yoffset;
        if (m_zoom < 1.0f) {
            m_zoom = 1.0f;
        }
        else if (m_zoom > 90.0f) {
            m_zoom = 90.0f;
        }
    }

    // getters and setters
    float getZoom() { return m_zoom; }
    glm::vec3 getPosition() { return m_position; }
    glm::vec3 getFront() { return m_front; }

protected:
    // camera Attributes
    glm::vec3 m_position{ glm::vec3(0.0f, 0.0f, 0.0f) };
    glm::vec3 m_front{ glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };
    glm::vec3 m_right{};
    glm::vec3 m_worldUp{};
    // euler Angles
    float m_yaw{ cameraValues::YAW };
    float m_pitch{ cameraValues::PITCH };
    // camera options
    float m_movementSpeed{ cameraValues::SPEED };
    float m_mouseSensitivity{ cameraValues::SENSITIVITY };
    float m_zoom{ cameraValues::ZOOM };

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
};

class FPSCamera : public Camera {
public:
    // Constructor with vectors and eulers
    FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = cameraValues::YAW, float pitch = cameraValues::PITCH)
        : Camera(position, up, yaw, pitch)
    {
    }

    // Constructor with scalar values
    FPSCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Camera(posX, posY, posZ, upX, upY, upZ, yaw, pitch)
    {
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    // This time it makes sure that the player is restricted to the XZ plane
    void processKeyboard(Camera_Movement direction, float deltaTime) override {
        float velocity = m_movementSpeed * deltaTime;
        switch (direction) {
        case FORWARD:   m_position += m_front * velocity;   break;
        case BACKWARD:  m_position -= m_front * velocity;   break;
        case LEFT:      m_position -= m_right * velocity;   break;
        case RIGHT:     m_position += m_right * velocity;   break;
        default:        m_position;                         break;
        }

        m_position.y = 0;
    }

private:
    glm::vec3 m_xzPlaneMovement{ 1.0f, 0.0f, 1.0f };

};