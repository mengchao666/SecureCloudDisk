#include <XTools.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
std::string GetDirData(std::string path)
{
    string data = "";

    const char *dir = path.c_str();
    DIR *dp = 0;
    struct dirent *entry = 0;
    struct stat statbuf;
    dp = opendir(dir);
    if (dp == NULL)
    {
        return data;
    }
    chdir(dir);
    char buf[1024] = {0};
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            continue;
        }
        sprintf(buf, "%s, %lld", entry->d_name, statbuf.st_size);
        data += buf;
    }

    // 去掉结尾分号
    if (!data.empty())
    {
        data = data.substr(0, data.size() - 1);
    }
    return data;
}
