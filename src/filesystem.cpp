#include "filesystem.hpp"

std::vector<std::string> filesystem::getFileNames(const std::string &path)
{
    std::vector<std::string> filenames;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path.c_str())) != NULL)
    {
      while ((ent = readdir (dir)) != NULL)
      {
          if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
              continue;

          filenames.push_back(ent->d_name);
      }

      closedir(dir);
    }
    else
    {
        Logger::getInstance()->logError("Failed to open the folder " + path + " for reading filenames");
    }

    return filenames;
}

std::vector<std::string> filesystem::getFilePaths(const std::string& path)
{
    auto filenames = filesystem::getFileNames(path);
    std::for_each(filenames.begin(), filenames.end(), [path](std::string& elem) { elem = path + "/" + elem; });

    return filenames;
}

bool filesystem::checkPathExist(const char* path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

bool filesystem::createDirectory(const char* path)
{
    return mkdir(path, 0777) == 0;
}

bool filesystem::removeFile(const char* path)
{
    return remove(path) == 0;
}

bool filesystem::removeDirFiles(const char* path)
{
    bool status(true);

    for (auto& elem: filesystem::getFilePaths(path))
        status *= removeFile(elem.c_str());

    return status;
}

void filesystem::saveJson(const rapidjson::Document& document, const std::string& path)
{
    FILE* file = fopen(path.c_str(), "w");

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    document.Accept(writer);

    fclose(file);
}

std::shared_ptr<rapidjson::Document> filesystem::readJson(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "rb");

    // NOTE - Check if the file was opened successfully
    if (!fp)
    {
        Logger::getInstance()->logError("Failed to open file at path " + path + " to read JSON object");
        return nullptr;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    auto profile_file = std::make_shared<rapidjson::Document>();

    profile_file->ParseStream(is);

    // NOTE - Check if the document is valid
    if (profile_file->HasParseError())
    {
        Logger::getInstance()->logError("Failed to parse JSON object in file at path " + path);
        fclose(fp);

        return nullptr;
    }

    fclose(fp);
    return profile_file;
}

std::string filesystem::executeCommand(const char* cmd)
{
    char buffer[128];

    std::string result = "";
    FILE* pipe = popen(cmd, "r");

    if (!pipe)
    {
        throw std::runtime_error("Failed to open pipe to execute command");
    }

    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    pclose(pipe);
    return result;
}
