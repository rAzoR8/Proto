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

        auto nameOrId = [&s](const Instruction* instr)
        {
            const char* resName = instr->getName();
            if (resName != nullptr && *resName != '\0')
            {
                s << resName;
            }
            else
            {
                s << instr->getResultId();
            }
        };

        auto instrPrint = [&](const Instruction& instr)
        {
            if (instr.hasResult())
            {
                nameOrId(&instr);
                s << " = ";
            }
            s << (unsigned int)instr.getOperation();

            for (const Operand& operand : instr)
            {
                if (operand.type == Operand::Type::Literal)
                {
                    s << " ";
                }
                else
                {
                    s << " %";
                }

                switch (operand.type)
                {
                case Operand::Type::Instruction:
                    nameOrId(operand.instruction);
                    break;
                case Operand::Type::ResultId:
                    s << operand.resultId;
                    break;
                case Operand::Type::BranchTarget:
                    nameOrId(&operand.branchTarget->front());
                    break;
                case Operand::Type::Literal:
                    s << operand.value.value;
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
