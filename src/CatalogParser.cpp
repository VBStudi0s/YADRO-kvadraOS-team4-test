#include "CatalogParser.hpp"
#include <stdexcept>
#include <vector>

#include "Extensions.hpp"

namespace fs = std::filesystem;
using MediaMap = std::unordered_map<std::string, std::vector<std::string>>;

CatalogParser::CatalogParser(const std::string& path):
    m_root(path)
{
    if(!fs::exists(m_root) || !fs::is_directory(m_root))
        throw std::runtime_error("'" + path + "' does not exist or is not a directory");

    initialize_exts_default();
}

CatalogParser::CatalogParser()
{
    initialize_exts_default();
}

void CatalogParser::set_root_path(const std::string& path)
{
    m_root = path;
    if(!fs::exists(m_root) || !fs::is_directory(m_root))
        throw std::runtime_error("'" + path + "' does not exist or is not a directory");
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

    std::lock_guard lk(m_found_files_mutex);
    m_found_files = std::move(cur_found_files);
}

nlohmann::json CatalogParser::serialize()
{
    std::shared_ptr<std::unordered_map<std::string, std::vector<std::string>>> payload;
    {
        std::lock_guard lk(m_found_files_mutex);
        payload = m_found_files;
    }
    if (!payload) {
        return nlohmann::json::object();
    }
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

    for(const std::string ext: AUDIO_EXTS)
        m_exts[ext] = "audio";
    for(const std::string ext: VIDEO_EXTS)
        m_exts[ext] = "video";
    for(const std::string ext: IMAGES_EXTS)
        m_exts[ext] = "images";
}
