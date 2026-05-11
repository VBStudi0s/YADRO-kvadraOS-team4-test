#include <iostream>

#include "CatalogParser.hpp"

int main()
{
    CatalogParser parser("/home");
    parser.parse();
    return 0;
}
