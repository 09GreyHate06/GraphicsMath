#pragma once
#include <exception>
#include <string>
#include <sstream>

#include "NativeWindow.h"
#include "Rendering/DXError/dxerr.h"

#ifdef GM_DEBUG
#define HR(hrcall) \
{ \
	HRESULT hr = hrcall; \
	if(FAILED(hr)) \
	{ \
		DXTraceA(__FILE__, __LINE__, hr, #hrcall, true); \
	} \
}

#define HR_MSG(hrcall, msg) \
{ \
	HRESULT hr = hrcall; \
	if(FAILED(hr)) \
	{ \
		DXTraceA(__FILE__, __LINE__, hr, std::string(#hrcall).append("\nError msg: ").append(msg).c_str(), true); \
	} \
}
#else
#define HR(hrcall) hrcall;
#define HR_MSG(hrcall,)
#endif // GM_DEBUG

namespace GM
{
	class GMException : public std::exception
	{
	public:
		GMException(int line, const std::string& file)
			: m_line(line), m_file(file)
		{
		}

		virtual const char* what() const override
		{
			std::ostringstream oss;
			oss << GetType() << '\n'
				<< GetOriginString();

			m_whatBuffer = oss.str();

			return m_whatBuffer.c_str();
		}

		virtual const char* GetType() const { return "GMException"; }

		std::string GetOriginString() const
		{
			std::ostringstream oss;
			oss << "[File] " << m_file << '\n'
				<< "[Line] " << m_line;

			return oss.str();
		}

		int GetLine() const { return m_line; }

	protected:
		mutable std::string m_whatBuffer;

	private:
		int m_line;
		std::string m_file;
	};
}