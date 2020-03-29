#ifndef CURLTOOL_H
#define CURLTOOL_H

#include <string>

using std::string;

class CurlDownload
{
public:
    ~CurlDownload();
    CurlDownload(const CurlDownload&)=delete;
    CurlDownload& operator=(const CurlDownload&)=delete;
    static CurlDownload& get_instance(){
        static CurlDownload instance;
        return instance;
    }

    int downLoad(string url);
    static string rootDir;
private:
    CurlDownload();
};


int32_t createDirectory(const std::string &directoryPath);
size_t process_data(void *buffer, size_t size, size_t nmemb, void *user_p);

#endif // CURLTOOL_H
