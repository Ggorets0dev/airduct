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


