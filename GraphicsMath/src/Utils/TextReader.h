#pragma once

#include <string>

namespace GM::Utils
{
	class TextReader
	{
	public:
		static std::string Read(const std::string& filename);

	private:
		TextReader() = default;
	};
}