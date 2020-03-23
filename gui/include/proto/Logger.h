#pragma once

#include "spvgentwo/Logger.h"
#include "common/HeapString.h"
#include "common/HeapList.h"

namespace proto
{
	class Logger : public spvgentwo::ILogger
	{
	public:
		Logger(unsigned int _maxHistory = 100u);
		~Logger();

		void log(const spvgentwo::LogLevel _level, const char* _pMsg) final;

		void update();

		static Logger* instance();

		void setMaxHistory(unsigned int _maxHistory) { m_maxHistory = _maxHistory; }

	private:
		unsigned int m_maxHistory = 0u;
		bool m_isOpen = true;
		bool m_autoScroll = true;

		char m_filter[64]{ };

		spvgentwo::HeapList<spvgentwo::HeapString> m_buffer;
	};
} // !proto