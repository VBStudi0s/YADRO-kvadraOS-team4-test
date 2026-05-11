#include <iostream>
#include <fstream>

#include "CatalogParser.hpp"

int main()
{
    CatalogParser parser("/home");
    parser.parse();

    std::ofstream to_load(".media_files");
    to_load << parser.serialize();

    return 0;
}
