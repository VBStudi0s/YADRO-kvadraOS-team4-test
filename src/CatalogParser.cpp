#include "CatalogParser.hpp"
#include <stdexcept>

#include <iostream> // remove

namespace fs = std::filesystem;

CatalogParser::CatalogParser(const std::string& path):
    m_root(path)
{
    if(!fs::exists(m_root) || !fs::is_directory(m_root))
        throw std::runtime_error("'" + path + "' does not exist or is not a directory");
}

void CatalogParser::parse()
{
    for (const auto& entry : fs::recursive_directory_iterator(m_root)) {
        std::cout << entry.path() << "\n";
    }
}
