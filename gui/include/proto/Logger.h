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

		void update();

		static Logger* instance();

		void setMaxHistory(unsigned int _maxHistory) { m_maxHistory = _maxHistory; }

	private:
		static void LogImpl(ILogger* _pInstance, spvgentwo::LogLevel _level, const char* _pFormat, ...);

		void addMsg(const spvgentwo::LogLevel _level, const char* _pMsg);

	private:
		unsigned int m_maxHistory = 0u;
		bool m_isOpen = true;
		bool m_autoScroll = true;

		char m_filter[64]{ };

		spvgentwo::HeapList<spvgentwo::HeapString> m_buffer;
	};

	template <typename ...Args>
	void log(spvgentwo::LogLevel _level, const char* _pFormat, Args&& ..._args)
	{
		Logger::instance()->log(_level, _pFormat, spvgentwo::stdrep::forward<Args>(_args)...);
	}

	template <typename ...Args>
	void logInfo(const char* _pFormat, Args&& ..._args)
	{
		Logger::instance()->logInfo(_pFormat, spvgentwo::stdrep::forward<Args>(_args)...);
	}

	template <typename ...Args>
	void logDebug(const char* _pFormat, Args&& ..._args)
	{
		Logger::instance()->logDebug(_pFormat, spvgentwo::stdrep::forward<Args>(_args)...);
	}

	template <typename ...Args>
	void logWarning(const char* _pFormat, Args&& ..._args)
	{
		Logger::instance()->logWarning(_pFormat, spvgentwo::stdrep::forward<Args>(_args)...);
	}

	template <typename ...Args>
	void logError(const char* _pFormat, Args&& ..._args)
	{
		Logger::instance()->logError(_pFormat, spvgentwo::stdrep::forward<Args>(_args)...);
	}

	template <typename ...Args>
	void logFatal(const char* _pFormat, Args&& ..._args)
	{
		Logger::instance()->logFatal(_pFormat, spvgentwo::stdrep::forward<Args>(_args)...);
	}
} // !proto