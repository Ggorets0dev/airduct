#include "filesystem.hpp"

std::vector<std::string> getFileNames(const std::string &path)
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
        Logger::getInstance()->logError("Failed to open the folder " + path + " where the profiles should be located");
    }

    return filenames;
}

std::vector<std::string> getFilePaths(const std::string& path)
{
    auto filenames = getFileNames(path);
    std::for_each(filenames.begin(), filenames.end(), [path](std::string& elem) { elem = path + "/" + elem; });

    return filenames;
}

bool checkPathExist(const char* path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

bool createDirectory(const char* path)
{
    return mkdir(path, 0777) == 0;
}

bool removeFile(const char* path)
{
    return remove(path) == 0;
}

void saveJson(const rapidjson::Document& document, const std::string& path)
{
    FILE* file = fopen(path.c_str(), "w");

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    document.Accept(writer);

    fclose(file);
}

std::shared_ptr<rapidjson::Document> readJson(const std::string& path)
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
