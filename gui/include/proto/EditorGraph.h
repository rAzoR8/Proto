#pragma once

#include "spvgentwo/Module.h"
#include "AssemblyTextView.h"
#include "OpNodeExpr.h"
#include "Validator.h"

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
		EditorGraph(spvgentwo::ILogger* _pLogger, const char* _pName);
		~EditorGraph();

		void update();

		void clear();

		void save();

	private:
		void createCanvas();

		void updateContextMenu();

		void evaluateExprGraph();

	private:
		ImNodes::CanvasState* m_pCanvas = nullptr;
		spvgentwo::Module m_module;
		spvgentwo::HeapVector<unsigned int> m_moduleBinary;
		spvgentwo::BasicBlock* m_pBB = nullptr;

		Validator m_validator;
		AssemblyTextView m_textView;

		const char* m_pName = nullptr;

		spvgentwo::ExprGraph<OpNodeExpr> m_nodes;
	};
} //!proto