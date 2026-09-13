#include <iostream>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <map>
#include <string>
#include <utility>
using namespace std;
namespace fs = std::filesystem;

// inilitiazation
class FileInfo;
pair<double, string> sizeconverter(long long bytesize);
string getsizecategory(long long sizedivider,const FileInfo &currentFile);

class FileInfo
{
    string filename;
    string extension;
    long long size;
   

public:
    FileInfo(string f, string e, long long s) : filename(f), extension(e), size(s)
    {
    }
    string getfilename() const
    {
        return filename;
    }
    string getextension() const
    {
        return extension;
    }
    long long getsize() const
    {
        return size;
    }
    void display(long long sizedivider)
    {
        cout << setw(25) << left << filename;
        cout << setw(15) << extension;
        cout << setw(15)  <<right<< sizeconverter(size).first << sizeconverter(size).second;
        cout << setw(15) << right<<getsizecategory(sizedivider, *this);
        cout << endl;
    }
};

pair<double, string> sizeconverter(long long bytesize)
{
    double kb = bytesize / 1024.0;
    double mb = kb / 1024.0;
    double gb = mb / 1024.0;
    pair<double, string> data;
    if (bytesize < 1024 * 1024)
    {
        data.first = kb;
        data.second = "KB";
        return data;
    }
    else if (bytesize < (1024 * 1024 * 1024))
    {
        data.first = mb;
        data.second = "MB";
        return data;
    }
    else
    {
        data.first = gb;
        data.second = "GB";
        return data;
    }
}

string getsizecategory(long long sizedivider,const FileInfo &currentFile){
    if(currentFile.getsize()<sizedivider){
        return "Small";
    }else if(currentFile.getsize()<sizedivider*2){
        return "Medium";
    }
    else{
        return "Large";
    }
}



int main()
{
    // startup
    cout << "=============================" << endl;
    cout << "      |PC GUARDIAN v1.0|     " << endl;
    cout << "=============================" << endl;
    cout << "ENTER DIRECTORY PATH : ";
    string path;
    getline(cin, path);
    string empty = "";
    if (path.compare(empty) == 0)
    {
        cout << "EMPTY PATH\n";
    }

    // intitialization
    long long sizeDivider;
    int SkippedEntriesEntries = 0;
    long long TotalSize = 0;
    long long MaxSize = 0;
    string MaxFileName;
    map<string, long long> storage;
    vector<FileInfo> files;
    const int SUCCESS = 0;
    const int filesystemerror = 1;
    const int Invalidpath = 2;

    try
    {
        fs::directory_entry Directory{path};

        // all checks
        if (!Directory.exists())
        {
            cout << "DIRECTORY DOES NOT EXIST\n";
            return Invalidpath;
        }
        if (Directory.is_regular_file())
        {
            cout << "ITS A FILE PATH\n";
            return Invalidpath;
        }
        if (!Directory.is_directory())
        {
            cout << "INVALID PATH\n";
            return Invalidpath;
        }

        cout << "\033[33m";
        cout << "ITS A FOLDER DIRECTORY" << endl;
        cout << "\033[0m";

        // DIRECTORY SCANING
        for (const auto &entry : fs::directory_iterator(path))
        {
            try
            {
                if (entry.is_regular_file())
                {

                    auto filename = entry.path().filename().string();
                    auto extension = entry.path().extension().string();
                    auto size = entry.file_size();

                    FileInfo file(filename, extension, size);
                    TotalSize += file.getsize();

                    
                    // large file check
                    if (file.getsize() > MaxSize)
                    {
                        MaxSize = file.getsize();
                        MaxFileName = file.getfilename();
                    }
  
                    storage[extension] += size;

                    // obj data storing
                    files.push_back(file);
                    
                }

                else
                {
                    cout << "THE FOLDER AVALIBLE IN PATH: " << entry.path().filename() << endl;
                }
            }
            catch (const fs::filesystem_error &e)
            {
                cout << "Skipped entries :" << e.what() << endl;
                SkippedEntriesEntries++;
            }
        }
        sizeDivider = MaxSize/3;
        if(sizeDivider==0){
            sizeDivider=1;
        }
    }
    catch (const fs::filesystem_error &e)
    {
        cout << "FILE SYSTEM ERROR :" << e.what() << endl;
        return filesystemerror;
    }
    if (files.size() == 0)
    {
        cout << "NO REGULAR FILES IN THIS DIRECTORY\n";
        return SUCCESS;
    }
    else
    {
        // displaying file info
        if (SkippedEntriesEntries > 0)
        {
            cout << "\033[31m";
            cout << SkippedEntriesEntries << " ENTRIES SKIPPED DUE TO ACCESS/PERMISSION ERROR\n";
            cout << "\033[0m";
        }
        cout << "\033[32m";
        cout << "\nTHE FILES AVALIBE IN THE PATH :\n\n";
        cout << "\033[0m";
        cout << "THE TOTAL NO. OF FILES AVALIBE : " << files.size() << endl;
        cout << "TOTAL SIZE : " << sizeconverter(TotalSize).first << sizeconverter(TotalSize).second << endl;
        cout << "MAXIMUM FILE SIZE :" << sizeconverter(MaxSize).first << sizeconverter(MaxSize).second << "\nLARGE FILE NAME : " << MaxFileName << endl;
        cout << "TOTAL EXTENSION : " << storage.size() << endl;
        for (auto &item : storage)
        {
            cout << setw(10) << left << item.first << " : " << sizeconverter(item.second).first << sizeconverter(item.second).second << endl;
        }
        // heading code of table
        cout << setw(25) << left << "Name";
        cout << setw(15) << "Extension";
        cout << setw(15) <<right<< " Size";
        cout << setw(18) << right << " Category";
        cout << endl;
        cout << setfill('-') << setw(75) << "-" << endl;
        cout << setfill(' ');
        for (auto &Storedfile : files)
        {
            Storedfile.display(sizeDivider);
        }
        cout << "\nYOUR PATH :  " << path << endl;
    }
    return SUCCESS;
}