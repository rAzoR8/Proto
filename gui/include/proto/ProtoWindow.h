#pragma once

#include "proto/Window.h"
#include "Graph.h"
#include "common/HeapAllocator.h"
#include "common/ConsoleLogger.h"

namespace proto
{
	class ProtoWindow : public Window
	{
	public:
		ProtoWindow();
		~ProtoWindow();

		bool updateUI() final;

	private:
		void updateMenu();
		void updateMenuFile();

		void updateGraphs();

	private:
		spvgentwo::HeapAllocator m_alloc;
		spvgentwo::ConsoleLogger m_logger;

		bool m_shouldClose = false;

		Graph m_graph;		
	};

} // !proto