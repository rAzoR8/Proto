#pragma once

#include "ImNodesEz.h"
#include "SpvGenTwo/List.h"
#include <vector>

namespace proto
{
	class Node
	{
		enum Kind : int
		{
			In,
			Out
		};

	public:
		Node(spvgentwo::IAllocator* _pAlloc, const char* _pTitle, ImVec2 _pos);
		~Node();

		const char* getTitle() const { return m_pTitle; }

		void update();

		void addInputSlot(const char* _pSlotTitle, Kind _kind, Node* _pInput = nullptr);
		void addOutputSlot(const char* _pSlotTitle, Kind _kind, Node* _pOutput = nullptr);

		void clear();

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		const char* m_pTitle = "Node";

		ImVec2 m_pos{};
		bool m_selected = false;
		Kind m_kind = In;

		std::vector<ImNodes::Ez::SlotInfo> m_inputSlots;
		spvgentwo::List<Node*> m_inputs;

		std::vector<ImNodes::Ez::SlotInfo> m_outputSlots;
		spvgentwo::List<Node*> m_outputs;
	};
} // !proto