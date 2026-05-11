#include "CatalogParser.hpp"
#include <stdexcept>

#include <iostream> // remove
#include <vector>

namespace fs = std::filesystem;
using MediaMap = std::unordered_map<std::string, std::vector<std::string>>;

CatalogParser::CatalogParser(const std::string& path):
    m_root(path)
{
    if(!fs::exists(m_root) || !fs::is_directory(m_root))
        throw std::runtime_error("'" + path + "' does not exist or is not a directory");

    initialize_exts_default();
}

void CatalogParser::parse()
{
    std::shared_ptr<MediaMap> cur_found_files = std::make_shared<MediaMap>(get_default_media_map());
    for (const auto& entry : fs::recursive_directory_iterator(m_root, fs::directory_options::skip_permission_denied)) {
        std::string ext = entry.path().extension().string();
        if (ext == "") continue;

        auto instance = m_exts.find(ext);
        if(instance != m_exts.end())
            (*cur_found_files)[instance->second].push_back(entry.path().filename().string());
    }

    // consider about setting a mutex here
    m_found_files = std::move(cur_found_files);
}

nlohmann::json CatalogParser::serialize()
{
    std::shared_ptr<std::unordered_map<std::string, std::vector<std::string>>> payload;
    // consider about setting a mutex here
    payload = m_found_files;
    return nlohmann::json(*payload);
}

MediaMap CatalogParser::get_default_media_map()
{
    MediaMap map;
    for(const auto& p: m_exts)
        map[p.second] = {};
    return map;
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
