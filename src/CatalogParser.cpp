#include "CatalogParser.hpp"
#include <stdexcept>

#include <iostream> // remove

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
    for (const auto& entry : fs::recursive_directory_iterator(m_root)) {
        std::string ext = entry.path().extension().string();
        if (ext == "") continue;
        extension_check(m_images_exts, ext, "Image");
    }
}

void CatalogParser::extension_check(
        const std::unordered_set<std::string>& exts_set,
        const std::string& ext,
        const std::string typo
    )
    {
        if(exts_set.find(ext) != exts_set.end())
            std::cout<<typo<<" : "<< ext<<'\n';
    }

void CatalogParser::initialize_exts_default()
{
    m_audio_exts = {
        ".mp3",
        ".wav"
    };
    m_video_exts = {
        ".mp4",
        ".mpg"
    };
    m_images_exts = {
        ".jpg",
        ".jpeg",
        ".png"
    };
}
