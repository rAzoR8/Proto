#include "proto/AssemblyTextView.h"
#include "imgui.h"
#include "spvgentwo/Module.h"
#include <sstream>

using namespace spvgentwo;

proto::AssemblyTextView::AssemblyTextView()
{
}

proto::AssemblyTextView::~AssemblyTextView()
{
}

void proto::AssemblyTextView::update(spvgentwo::Module& _module)
{
    if (ImGui::Begin("Text View"))
    {
        _module.assignIDs();

        std::ostringstream s;

        auto instrPrint = [&s](const Instruction& instr)
        {
            s << instr.getResultId() << " = " << (unsigned int)instr.getOperation();

            for (const Operand& operand : instr)
            {
                switch (operand.type)
                {
                case Operand::Type::Instruction:
                    s << " %" << operand.instruction->getResultId();
                    break;
                case Operand::Type::ResultId:
                    s << " %" << operand.resultId;
                    break;
                case Operand::Type::BranchTarget:
                    s << " %" << operand.branchTarget->front().getResultId();
                    break;
                case Operand::Type::Literal:
                    s << " " << operand.value.value;
                    break;
                default:
                    break;
                }
            }

            s << std::endl;
        };

        _module.iterateInstructions(instrPrint);

        ImGui::Text("%s", s.str().c_str());
    }
    ImGui::End();
}
