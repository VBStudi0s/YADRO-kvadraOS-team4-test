#include <iostream>
#include <chrono>
#include <thread>

#include "http_lib/httplib.h"

#include "CatalogParser.hpp"

void USAGE()
{
    std::cout<<"media_list --path <path_to_dir> --period <search_period>\n";
}

struct SetupSettings
{
    std::string path = "/home";
    std::chrono::milliseconds period_ms{5000};
};

void setup(int argc, char** argv, SetupSettings& settings)
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

    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--path") settings.path = need_arg(i, "--path");
        else if (a == "--period")
        {
            int period = std::stoi(need_arg(i, "--period"));
            if(period <= 0)
            {
                std::cerr << "Invalid period value\n";
                std::exit(2);
            }
            settings.period_ms = std::chrono::milliseconds(period);
        }
        else {
            std::cerr << "Unknown arg: " << a << "\n";
            USAGE();
            std::exit(2);
        }
    }
}

void parser_thread_worker(const SetupSettings& settings, CatalogParser& parser)
{
    try
    {
        parser.set_root_path(settings.path);
        auto next_run = std::chrono::steady_clock::now();
        while(1)
        {
            next_run += settings.period_ms;
            parser.parse();

            std::this_thread::sleep_until(next_run);
        }
    }
    catch(std::exception& e)
    {
        std::cerr<<"Program error: " << e.what()<<'\n';
        std::cerr<<"Directory parsing terminates\n";
        return;
    }
}

int main(int argc, char** argv)
{
    SetupSettings settings;
    setup(argc, argv, settings);

    CatalogParser parser;
    httplib::Server svr;
    svr.Get("/media_files", [&parser](const httplib::Request &, httplib::Response &res) {
        nlohmann::json response_json = parser.serialize();
        res.set_content(response_json.dump(), "application/json");
    });

    // starting parser thread
    std::thread parser_thread(parser_thread_worker, std::ref(settings), std::ref(parser));
    parser_thread.detach();

    svr.listen("127.0.0.1", 1234);

    return 0;
}
