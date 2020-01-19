#pragma once

#include "SpvGenTwo/Module.h"
#include "Node.h"

// forward decls
namespace ImNodes
{
	struct CanvasState;
}

namespace proto
{
	class Graph
	{
	public:
		Graph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName);
		~Graph();

		void update();

		void clear();

		void save();

	private:
		void createCanvas();

		void createNodes();

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		ImNodes::CanvasState* m_pCanvas = nullptr;
		spvgentwo::Module m_module;
		const char* m_pName = nullptr;

		spvgentwo::List<Node> m_nodes;
	};
} //!proto