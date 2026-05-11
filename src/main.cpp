#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "CatalogParser.hpp"

void USAGE()
{
    std::cout<<"media_list --path <path_to_dir> --period <search_period>\n";
}

int main(int argc, char** argv)
{
    /**
     * Helper function to get arguments from argv
     */
    auto need_arg = [&](int& i, const char* k) -> std::string
    {
        if (i + 1 >= argc) {
            std::cerr << "Missing value for " << k << "\n";
            std::exit(2);
        }
        return std::string(argv[++i]);
    };
    std::string path = "/home";
    std::chrono::milliseconds period_ms(5000);
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--path") path = need_arg(i, "--path");
        else if (a == "--period")
        {
            int period = std::stoi(need_arg(i, "--period"));
            if(period <= 0)
            {
                std::cerr << "Invalid period value\n";
                return(2);
            }
            period_ms = std::chrono::milliseconds(period);
        }
        else {
            std::cerr << "Unknown arg: " << a << "\n";
            USAGE();
            return 2;
        }
    }

    try
    {
        CatalogParser parser(path);

        while(1)
        {
            parser.parse();

            std::ofstream to_load(".media_files");
            to_load << parser.serialize();
            std::cout<<parser.serialize()<<'\n';

            std::this_thread::sleep_for(period_ms);
        }
    }
    catch(std::exception& e)
    {
        std::cerr<<"Program error: " << e.what()<<'\n';
        return 1;
    }

    return 0;
}
