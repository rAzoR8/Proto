#include "proto/Validator.h"
#include "common/HeapVector.h"
#include <spirv-tools/libspirv.hpp>
#include "proto/Logger.h"

#include "spvgentwo/FNV1aHasher.h"

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
    spvgentwo::FNV1aHasher hash;

    if (auto h = hash.add(_module.data(), _module.size() * sizeof(unsigned int)); h != m_hash)
    {
        m_hash = h;

        spvtools::SpirvTools tools(SPV_ENV_UNIVERSAL_1_5);

        tools.SetMessageConsumer(CLIMessageConsumer);
        spvtools::ValidatorOptions options;

        m_valid = tools.Validate(_module.data(), _module.size(), options);
    }

    return m_valid;
}
