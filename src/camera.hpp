#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float lastX, lastY;
    float sensitivity;
    float speed;

public:
    Camera(float initialX = 400.0f, float initialY = 300.0f) 
        : position(0.0f, 1000.0f, 5000.0f),
          front(0.0f, 0.0f, -1.0f),
          up(0.0f, 1.0f, 0.0f),
          yaw(-90.0f),
          pitch(0.0f),
          lastX(initialX),
          lastY(initialY),
          sensitivity(0.1f),
          speed(10000.0f) {}

    void processMouseMovement(float xpos, float ypos) {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Constrain pitch
        pitch = std::max(-89.0f, std::min(89.0f, pitch));

        updateCameraVectors();
    }

    void processKeyboard(int direction, float deltaTime) {
        float velocity = speed * deltaTime;
        
        if (direction == GLFW_KEY_W)
            position += velocity * front;
        if (direction == GLFW_KEY_S)
            position -= velocity * front;
        if (direction == GLFW_KEY_A)
            position -= velocity * glm::normalize(glm::cross(front, up));
        if (direction == GLFW_KEY_D)
            position += velocity * glm::normalize(glm::cross(front, up));
        if (direction == GLFW_KEY_SPACE)
            position += velocity * up;
        if (direction == GLFW_KEY_LEFT_SHIFT)
            position -= velocity * up;
    }

    void processScroll(float yoffset, float deltaTime) {
        float velocity = 250000.0f * deltaTime;
        if (yoffset > 0)
            position += velocity * front;
        else if (yoffset < 0)
            position -= velocity * front;
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getFront() const { return front; }
    const glm::vec3& getUp() const { return up; }

private:
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
    }
};
