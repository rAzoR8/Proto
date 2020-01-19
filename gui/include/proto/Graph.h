#pragma once

#include "SpvGenTwo/Module.h"
#include "Node.h"
#include "AssemblyTextView.h"
#include <unordered_map>

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
		void updateNodeFromContainer(ModuleObjContainer& _container, Type _type, const CreateFunc& _create, const RemoveFunc& _remove);

		void addFunction();
		void addEntryPoint();

		Node* getNode(SpvObj _obj);

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		ImNodes::CanvasState* m_pCanvas = nullptr;
		spvgentwo::Module m_module;
		AssemblyTextView m_textView;

		const char* m_pName = nullptr;
		//ptr to spv obj -> Node
		// TODO: replace with spvgentwo hashmap to avoid reallocation by rehashing
		std::unordered_map<SpvObj, Node, spvgentwo::Hasher<SpvObj>> m_nodes;

		bool m_addFunctionModal = false;
	};

	// ModuleObjContainer = List<Function> etc, CreateFunc(const Function&) = nodes.emplace_back(...), RemoveFunc(Node& n) = n.clearConnections etc
	template<class ModuleObjContainer, class CreateFunc, class RemoveFunc>
	inline void Graph::updateNodeFromContainer(ModuleObjContainer& _container, Type _type, const CreateFunc& _create, const RemoveFunc& _remove)
	{
		// add:
		for (auto& spv : _container)
		{
			auto it = m_nodes.find(SpvObj(&spv));
			if (it == m_nodes.end())
			{
				_create(spv);
			}
		}
	}
} //!proto