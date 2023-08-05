#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS
#define VULKAN_HPP_NO_SMART_HANDLE
#define VULKAN_HPP_NO_NODISCARD_WARNINGS
#define VULKAN_HPP_HAS_SPACESHIP_OPERATOR
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define GLFW_INCLUDE_NONE

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vma/vk_mem_alloc.h>
#include <map>