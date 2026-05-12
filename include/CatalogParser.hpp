#ifndef _CATALOG_PARSER_HPP
#define _CATALOG_PARSER_HPP

#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

#include <nlohmann_json/json.hpp>

/* CatalogParser is used to iterate throw directory recursively and list files.
* It receives path to the root directory and then iterates throw it, returning
* JSON with needed content
*/
class CatalogParser
{
public:
    CatalogParser();
    CatalogParser(const std::string& path);

    void set_root_path(const std::string& path);

    void parse();

    nlohmann::json serialize() const;

private:
    using MediaMap = std::unordered_map<std::string, std::vector<std::string>>;

    std::filesystem::path m_root;
    std::unordered_map<std::string, std::string> m_exts;
    std::shared_ptr<MediaMap> m_found_files;

    mutable std::mutex m_found_files_mutex;

private:

    /* Initializes m_auidio_exts, m_video_exts and m_images_exts
    * with small amount of default known extensions
    */
    void initialize_exts_default();

    MediaMap get_default_media_map() const;

};

#endif //_CATALOG_PARSER_HPP
