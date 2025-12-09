#ifndef CAMERA_H
#define CAMERA_H

#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// abstract away from window-specific input methods using enum directions
enum Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};
// camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
// const float SPEED = 2.5f;
const float SPEED = 4.317 * 2; // minecraft speed times 2 cause scale is *2
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// abstract camera class
class Camera
{
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // fps cam option
  float FPSCam;
  // constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
  {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    FPSCam = false;
    updateCameraVectors();
  }
  // constructor with scalars
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
  {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
  }
  // return view matrix
  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }
  // process keyboard
  void ProcessKeyboard(Camera_Movement direction, float deltaTime)
  {
    glm::vec3 flatfront = glm::normalize(glm::vec3(Front.x, 0.0, Front.z));
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
      Position += flatfront * velocity;
    if (direction == BACKWARD)
      Position -= flatfront * velocity;
    if (direction == LEFT)
      Position -= Right * velocity;
    if (direction == RIGHT)
      Position += Right * velocity;
    if (direction == UP)
      Position.y += 1.0 * velocity;
    if (direction == DOWN)
      Position.y -= 1.0 * velocity;
    // fps cam
    if (FPSCam)
      Position.y = 0.0f;
  }
  // process mouse
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
  {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    Yaw += xoffset;
    Pitch += yoffset;
    // clip pitch
    if (constrainPitch)
    {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }
    // update vectors
    updateCameraVectors();
  }
  // process mouse scroll
  void ProcessMouseScroll(float yoffset)
  {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if (Zoom > 45.0f)
      Zoom = 45.0f;
  }
  void setFPSCam()
  {
    FPSCam = true;
  }
  // look at
  void lookAt(glm::vec3 dir) {
    Front = glm::normalize(dir - Position);
    Right = glm::normalize(glm::cross(Right, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));

    Yaw = glm::degrees(atan2(Front.z, Front.x));
    Pitch = glm::degrees(asin(Front.y));
  }

private:
  void updateCameraVectors()
  {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // recalute up and right vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
    
  }
};
#endif