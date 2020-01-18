#pragma once

#include "imgui.h"
#include "imgui_impl_vulkan.h"

// foward decls:
struct GLFWwindow;
//struct ImGui_ImplVulkanH_Window;

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

	private:
		GLFWwindow* m_pGLFWWindow = nullptr;
		ImGui_ImplVulkanH_Window m_VulkanWindow{};

		bool m_SwapChainRebuild = false;
		int  m_SwapChainResizeWidth = 0;
		int  m_SwapChainResizeHeight = 0;
	};
} // !proto