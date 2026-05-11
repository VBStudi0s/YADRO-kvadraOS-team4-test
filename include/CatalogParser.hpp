#ifndef _CATALOG_PARSER_HPP
#define _CATALOG_PARSER_HPP

#include <string>
#include <filesystem>

/* CatalogParser is used to iterate throw directory recursively and list files.
* It receives path to the root directory and then iterates throw it, returning
* JSON with needed content
*/
class CatalogParser
{
public:
    CatalogParser(const std::string& path);

    void parse();

private:
    std::filesystem::path m_root;
};

#endif //_CATALOG_PARSER_HPP
