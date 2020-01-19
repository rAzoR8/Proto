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

		void updateNodes();

		void updateContextMenu();

		template<class ModuleObjContainer, class CreateFunc, class RemoveFunc>
		void updateNodeFromContainer(ModuleObjContainer& _container, const CreateFunc& _create, const RemoveFunc& _remove);

		void addFunction();
		void addEntryPoint();

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		ImNodes::CanvasState* m_pCanvas = nullptr;
		spvgentwo::Module m_module;
		const char* m_pName = nullptr;
		spvgentwo::List<Node> m_nodes;

		bool m_addFunctionModal = false;
	};

	// ModuleObjContainer = List<Function> etc, CreateFunc(const Function&) = nodes.emplace_back(...), RemoveFunc(Node& n) = n.clearConnections etc
	template<class ModuleObjContainer, class CreateFunc, class RemoveFunc>
	inline void Graph::updateNodeFromContainer(ModuleObjContainer& _container, const CreateFunc& _create, const RemoveFunc& _remove)
	{
		// remove
		for (auto it = m_nodes.begin(); it != m_nodes.end();)
		{
			Node& n = *it;
			auto spvIt = _container.find_if([&n](const auto& spv) {return n.getSpvObj() == &spv; });
			if (spvIt == nullptr) // visible node n is not in the module _container anymore
			{
				_remove(n);
				it = m_nodes.erase(it);
			}
			else
			{
				++it;
			}
		}

		// add:
		for (auto& spv : _container)
		{
			auto it = m_nodes.find_if([&spv](const Node& n) {return n.getSpvObj() == &spv; });
			if (it == nullptr) // spv _container object is not in the visible nodes yet
			{
				_create(spv);
			}
		}
	}
} //!proto