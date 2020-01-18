#pragma once

#include "imgui.h"
#include "imgui_impl_vulkan.h"

// foward decls:
struct GLFWwindow;

namespace proto
{
	class Window
	{
	public:
		Window();
		Window(int _width, int _height);
		virtual ~Window();

		bool init(int _width, int _height);

		virtual bool updateUI() = 0;

		int exec();

	private:
		static void glfwResizeCallback(GLFWwindow* _pWindow, int w, int h);
		static void glfwErrorCallback(int error, const char* description);

		void setupVulkanWindow(VkSurfaceKHR surface, int width, int height);
		void frameRender();
		void cleanupVulkan();
		void framePresent();

		VkResult setupVulkan(const char** extensions, uint32_t extensions_count);

	private:
		GLFWwindow* m_pGLFWWindow = nullptr;
		ImGui_ImplVulkanH_Window m_VulkanWindow{};

		ImVec4 m_clearColor = { 0.45f, 0.55f, 0.60f, 1.00f };

		bool m_SwapChainRebuild = false;
		int  m_SwapChainResizeWidth = 0;
		int  m_SwapChainResizeHeight = 0;

		const int m_minImageCount = 2;
	};
} // !proto