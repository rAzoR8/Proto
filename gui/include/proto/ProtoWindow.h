#pragma once

#include "proto/Window.h"
#include "SpvGenTwo/Module.h"
#include "common/HeapAllocator.h"
#include "common/ConsoleLogger.h"

namespace proto
{
	class ProtoWindow : public Window
	{
	public:
		//using Window::Window;

		ProtoWindow();
		~ProtoWindow();

		bool updateUI() final;

	private:
		bool updateMenu();
		void updateMenuFile();

	private:
		spvgentwo::HeapAllocator m_alloc;
		spvgentwo::ConsoleLogger m_logger;
		spvgentwo::Module m_module;
	};

} // !proto