#include "curltool.h"
#include <curl/curl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

using std::cerr;
using std::endl;

string CurlDownload::rootDir = "./";
const int MAX_PATH_LEN = 256;

CurlDownload::CurlDownload()
{
    CURLcode return_code;
    return_code = curl_global_init(CURL_GLOBAL_WIN32);
    if (CURLE_OK != return_code)
        throw string("init libcurl failed");
}

CurlDownload::~CurlDownload()
{
    curl_global_cleanup();
}

int CurlDownload::downLoad(std::string url)
{
    string::size_type pos = url.find('/', 9);
    if(pos == string::npos){
        cerr << "don't find file name" << endl;
        return -1;
    }

    string fileName = url.substr(pos+1);
    createDirectory(fileName);
    FILE *fp = fopen((rootDir+fileName).c_str() , "w");
    if(fp == nullptr){
        cerr << "open file false" << endl;
        return -1;
    }

    CURL *easy_handle = curl_easy_init();
    if (easy_handle == nullptr){
        cerr << "get a easy handle failed." << endl;
        curl_global_cleanup();
        return -1;
    }

    //设置easy handle属性
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &process_data);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);
    // 执行数据请求
    curl_easy_perform(easy_handle);
    // 释放资源
    fclose(fp);
    curl_easy_cleanup(easy_handle);
    return 0;
}


int32_t createDirectory(const string &path)
{
    auto dirPathLen = path.size();
    if (dirPathLen > MAX_PATH_LEN)
        return -1;

    string::size_type pos1 = 0;
    string::size_type pos2 = path.find('/');

    string dir = CurlDownload::rootDir;
    while (pos2 != string::npos) {

        dir += path.substr(pos1, pos2-pos1+1);
        if (access(dir.c_str(), F_OK) == 0)//目录存在
            goto finaly;

        if(mkdir(dir.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
            cerr << "创建目录失败" << endl;
            return -1;
        }
        finaly:
        pos1 = pos2+1;
        pos2 = path.find('/', pos1);
    }
    return 0;
}

size_t process_data(void *buffer, size_t size, size_t nmemb, void *user_p)
{
    FILE *fp = static_cast<FILE *>(user_p);
    size_t return_size = fwrite(buffer, size, nmemb, fp);
    return return_size;
}
