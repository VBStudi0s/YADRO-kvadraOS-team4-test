#include "CatalogParser.hpp"
#include <stdexcept>

#include <iostream> // remove
#include <vector>

namespace fs = std::filesystem;

CatalogParser::CatalogParser(const std::string& path):
    m_root(path)
{
    if(!fs::exists(m_root) || !fs::is_directory(m_root))
        throw std::runtime_error("'" + path + "' does not exist or is not a directory");

    initialize_exts_default();
}

void CatalogParser::parse()
{
    for (const auto& entry : fs::recursive_directory_iterator(m_root, fs::directory_options::skip_permission_denied)) {
        std::string ext = entry.path().extension().string();
        if (ext == "") continue;

        auto instance = m_exts.find(ext);
        if(instance != m_exts.end())
            std::cout<<instance->second<<" : "<<instance->first<<'\n';
    }
}

void CatalogParser::initialize_exts_default()
{
    std::vector<std::string> audio_exts = {
        ".mp3",
        ".wav"
    };
    for(const std::string ext: audio_exts)
        m_exts[ext] = "audio";
    std::vector<std::string> video_exts = {
        ".mp4",
        ".mpg"
    };
    for(const std::string ext: video_exts)
        m_exts[ext] = "video";
    std::vector<std::string> images_exts = {
        ".jpg",
        ".jpeg",
        ".png"
    };
    for(const std::string ext: images_exts)
        m_exts[ext] = "images";
}
