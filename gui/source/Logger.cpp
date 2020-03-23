#include "proto/Logger.h"

#include "imgui.h"

proto::Logger::Logger(unsigned int _maxHistory) :
	m_maxHistory(_maxHistory)
{
}

proto::Logger::~Logger()
{
}

void proto::Logger::log(const spvgentwo::LogLevel _level, const char* _pMsg)
{
	if (m_buffer.size() >= m_maxHistory)
	{
		m_buffer.pop_front();
	}

	m_buffer.emplace_back(_pMsg);
}

void proto::Logger::update()
{
	if (ImGui::Begin("Log", &m_isOpen))
	{
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (const auto& msg : m_buffer)
		{
			ImGui::Text(msg.c_str());
		}

		ImGui::SetScrollHereY();

		ImGui::EndChild();
	}

	ImGui::End();
}

proto::Logger* proto::Logger::instance()
{
	static Logger logger;
	return &logger;
}
