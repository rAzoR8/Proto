#include "proto/Validator.h"
#include "common/HeapVector.h"
#include <spirv-tools/libspirv.hpp>
#include "proto/Logger.h"

proto::Validator::Validator()
{
}

proto::Validator::~Validator()
{
}

void CLIMessageConsumer(spv_message_level_t level, const char* source,  const spv_position_t& position, const char* message)
{
    spvgentwo::LogLevel lvl = spvgentwo::LogLevel::Info;

    switch (level) {
    case SPV_MSG_FATAL:
        lvl = spvgentwo::LogLevel::Fatal;
        break;
    case SPV_MSG_INTERNAL_ERROR:
    case SPV_MSG_ERROR:
        lvl = spvgentwo::LogLevel::Error;
        break;
    case SPV_MSG_WARNING:
        lvl = spvgentwo::LogLevel::Warning;
        break;
    case SPV_MSG_INFO:
        lvl = spvgentwo::LogLevel::Info;
        break;
    default:
        break;
    }

    proto::log(lvl, "line %llu %s", position.index, message);
}

bool proto::Validator::validate(const spvgentwo::Vector<unsigned int>& _module)
{
    //m_moduleBinary
    spvtools::SpirvTools tools(SPV_ENV_UNIVERSAL_1_5);

    //spv_message_level_t level, const char*,
    //    const spv_position_t& position, const char* message
    tools.SetMessageConsumer(CLIMessageConsumer);
    spvtools::ValidatorOptions options;

    bool result = tools.Validate(_module.data(), _module.size(), options);

    if (result)
    {
        proto::logInfo("Generated valid module [%lluB]", _module.size() * sizeof(unsigned int));
    }

    return result;
}
