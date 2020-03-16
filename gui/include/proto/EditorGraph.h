#pragma once

#include "spvgentwo/Module.h"
#include "AssemblyTextView.h"
#include "NewFunctionPopup.h"
#include "OpNodeExpr.h"

// forward decls
namespace ImNodes
{
	struct CanvasState;
}

namespace proto
{
	class EditorGraph
	{
	public:
		EditorGraph(spvgentwo::IAllocator* _pAlloc, spvgentwo::ILogger* _pLogger, const char* _pName);
		~EditorGraph();

		void update();

		void clear();

		void save();

	private:
		void createCanvas();

		void updateContextMenu();

		void evaluateExprGraph();

	private:
		spvgentwo::IAllocator* m_pAlloc = nullptr;
		ImNodes::CanvasState* m_pCanvas = nullptr;
		spvgentwo::Module m_module;
		spvgentwo::BasicBlock* m_pBB = nullptr;
		AssemblyTextView m_textView;

		const char* m_pName = nullptr;
		
		NewFunctionPopup m_newFunctionPopup;

		spvgentwo::ExprGraph<OpNodeExpr> m_nodes;
	};
} //!proto