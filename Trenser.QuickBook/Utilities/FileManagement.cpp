#include "FileManagement.h"

/*
 * Function: FileManagement::readlines
 * Description: Reads all non-empty lines from a file into a vector of strings.
 *              Opens the file at the given PATH, throws a runtime_error if the file
 *              cannot be opened. Skips empty lines to avoid unnecessary entries.
 * Parameters:
 *    PATH - The file path to read from.
 * Returns:
 *    A vector of strings containing all non-empty lines from the file.
 */
std::vector<std::string> FileManagement::readlines(const std::string& PATH)
{
    std::ifstream file(PATH);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    return lines;
}

/*
 * Function: FileManagement::writeLines
 * Description: Writes a vector of strings to a file, one line per entry.
 *              Opens the file at the given PATH in truncate mode, overwriting
 *              existing content. Throws a runtime_error if the file cannot be opened.
 * Parameters:
 *    PATH  - The file path to write to.
 *    lines - A vector of strings to be written into the file.
 * Returns:
 *    None
 */
void FileManagement::writeLines(const std::string& PATH, const std::vector<std::string>& lines)
{
    std::ofstream file(PATH,std::ios::trunc);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    for (std::vector<std::string>::const_iterator iterator = lines.begin(); iterator != lines.end(); ++iterator)
    {
        file << (*iterator) << "\n";
    }
}
