#ifndef _CATALOG_PARSER_HPP
#define _CATALOG_PARSER_HPP

#include <string>
#include <filesystem>
#include <unordered_set>

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
    std::unordered_set<std::string> m_audio_exts;
    std::unordered_set<std::string> m_video_exts;
    std::unordered_set<std::string> m_images_exts;

private:

    /* Initializes m_auidio_exts, m_video_exts and m_images_exts
    * with small amount of default known extensions
    */
    void initialize_exts_default();

    /* Checks if ext is in exts set.
    */
    void extension_check(
        const std::unordered_set<std::string>& exts_set,
        const std::string& ext,
        const std::string typo
    );
};

#endif //_CATALOG_PARSER_HPP
