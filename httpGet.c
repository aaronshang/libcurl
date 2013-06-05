/*
*gcc -I/usr/local/include/curl/ -L/usr/local/lib/ -lcurl -o httpGet httpGet.c
*/
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <unistd.h>

#include "curl.h"
size_t process_data(void *buffer,size_t size,size_t nmemb,void *user_p)
{
    FILE *fp = (FILE*)user_p;
    size_t return_size = fwrite(buffer,size,nmemb,fp);
    printf("%s\n",(char *)buffer);
    return return_size;
}
int GetFileFromServer(char *fileName,char *savePath)
{
        char temp[50];
        char tmp[50];
        strcpy(temp,"http://192.168.0.18:8888/");
    strcat(temp,fileName);
    strcpy(tmp,savePath);
    strcat(tmp,fileName);
    CURL *easy_handle;
    CURLcode return_code;
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != return_code)
    {
        printf("init libcurl failed\n");
        return -1;
    }
    easy_handle = curl_easy_init();
    if (NULL == easy_handle)
    {
        printf("get a easy handle failed\n");
        curl_global_cleanup();
        return -1;
    }


    FILE *fp = fopen(tmp,"ab+");

    curl_easy_setopt(easy_handle,CURLOPT_URL,temp);
    curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION,process_data);
    curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,fp);

    curl_easy_perform(easy_handle);

    fclose(fp);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    return 0;
}

int main(char argc,char **argv)
{
        char fileName[]="a.txt";
        char savePath[]="/usr/local/tmp/";
        GetFileFromServer(fileName,savePath);
}
