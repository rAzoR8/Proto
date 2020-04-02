#pragma once

#include "proto/Window.h"
#include "proto/EditorGraph.h"

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
		bool m_shouldClose = false;

		EditorGraph m_graph;		
	};

} // !proto