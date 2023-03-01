#include "TextReader.h"

#include <fstream>
#include <assert.h>

namespace GM::Utils
{
	std::string TextReader::Read(const std::string& filename)
	{
		std::string result;
		std::ifstream in(filename, std::ios::in | std::ios::binary);

		assert(in && "Failed to load text file ");

		in.seekg(0, std::ios::end);
		result.resize(static_cast<uint32_t>(in.tellg()));
		in.seekg(0, std::ios::beg);
		in.read(result.data(), result.size());
		in.close();

		return result;
	}
}