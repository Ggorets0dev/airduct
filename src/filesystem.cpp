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
