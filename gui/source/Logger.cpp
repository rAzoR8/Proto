#include "proto/Logger.h"

#include "imgui.h"

#include <functional>
#include "common/HeapCallable.h"
#include "common/HeapAllocator.h"

proto::Logger::Logger(unsigned int _maxHistory) : ILogger(LogImpl),
	m_maxHistory(_maxHistory)
{
	using namespace spvgentwo;

	auto abs = make_callable(fabsf);

	auto val = abs(-1.f);

	auto x = printf;
	//auto x = fabsf;

	auto f = maker_variadic_func(x);

	f("hallo %d", 1);

	using T = decltype(x);

	Callable<int(const char*, ...)> cal(x);

	cal("hallo %d", 1);

	//auto s = sizeof(cal);

	//auto fun = spvgentwo::make_callable(spvgentwo::HeapAllocator::instance(), x);

	//std::function<decltype(x)> f = &printf;
	//f("hallo %d", 43);
}

proto::Logger::~Logger()
{
}

void proto::Logger::addMsg(const spvgentwo::LogLevel _level, const char* _pMsg)
{
	if (m_buffer.size() >= m_maxHistory)
	{
		m_buffer.pop_front();
	}

	m_buffer.emplace_back(_pMsg);
}

void proto::Logger::LogImpl(ILogger* _pInstance, spvgentwo::LogLevel _level, const char* _pFormat, ...)
{
	char buffer[512]{};

	int offset = 0u;

	switch (_level)
	{
	case spvgentwo::LogLevel::Debug:
		offset = sprintf_s(buffer, sizeof(buffer), "Debug: "); break;
	case spvgentwo::LogLevel::Info:
		offset = sprintf_s(buffer, sizeof(buffer), "Info: "); break;
	case spvgentwo::LogLevel::Warning:
		offset = sprintf_s(buffer, sizeof(buffer), "Warning: "); break;
	case spvgentwo::LogLevel::Error:
		offset = sprintf_s(buffer, sizeof(buffer), "Error: "); break;
	case spvgentwo::LogLevel::Fatal:
		offset = sprintf_s(buffer, sizeof(buffer), "Fatal: "); break;
	default:
		break;
	}


	va_list args;
	va_start(args, _pFormat);
	vsprintf_s(&buffer[offset], sizeof(buffer) - offset, _pFormat, args);
	va_end(args);

	reinterpret_cast<Logger*>(_pInstance)->addMsg(_level, buffer);
}

void proto::Logger::update()
{
	if (ImGui::Begin("Log", &m_isOpen))
	{
		if (ImGui::RadioButton("AutoScroll", m_autoScroll))
		{
			m_autoScroll = !m_autoScroll;
		}
		ImGui::SameLine();
		ImGui::InputText("##Filter", m_filter, sizeof(m_filter));

		auto len = strnlen(m_filter, sizeof(m_filter));

		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (const auto& msg : m_buffer)
		{
			if (len == 0 || strstr(msg.c_str(), m_filter) != nullptr)
			{
				bool pop_color = strstr(msg.c_str(), "error") != nullptr || strstr(msg.c_str(), "Error") != nullptr || strstr(msg.c_str(), "ERROR") != nullptr;
				if (pop_color)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); 
				}

				ImGui::Text(msg.c_str());

				if (pop_color)
				{
					ImGui::PopStyleColor();
				}
			}
		}

		if (m_autoScroll)
		{
			ImGui::SetScrollHereY();		
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

proto::Logger* proto::Logger::instance()
{
	static Logger logger;
	return &logger;
}
