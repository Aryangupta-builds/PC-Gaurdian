#include <iostream>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <map>
#include <string>
using namespace std;
namespace fs = std::filesystem;

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
    void display()
    {
        cout << setw(25) << left << filename;
        cout << setw(15) << extension;
        cout << setw(15) << right << size;
        cout << endl;
    }
};

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
    int SkippedEntriesEntries = 0;
    long long TotalSize = 0;
    long long MaxSize = 0;
    string MaxFileName;
    map<string, int> m;
    vector<FileInfo> files;
    const int SUCCESS = 0;
    const int filesystemerror = 1;
    const int Invalidpath = 2;

    try
    {
        fs::directory_entry Directory{path};

        // all checks
        if (Directory.is_regular_file())
        {
            cout << "ITS A FILE PATH\n";
            return Invalidpath;
        }
        if (!Directory.exists())
        {
            cout << "DIRECTORY DOES NOT EXIST\n";
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
                    // obj data storing
                    files.push_back(file);
                    // large file check

                    if (file.getsize() > MaxSize)
                    {
                        MaxSize = file.getsize();
                        MaxFileName = file.getfilename();
                    }

                    m[extension]++;
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
        cout << "TOTAL SIZE : " << TotalSize << endl;
        cout << "MAXIMUM FILE SIZE :" << MaxSize << " NAMED : " << MaxFileName << endl;
        cout << "TOTAL EXTENSION : " << m.size() << endl;
        for (auto &item : m)
        {
            cout << setw(10) << left << item.first << " : " << item.second << endl;
        }
        // heading code of table
        cout << setw(25) << left << "Name";
        cout << setw(15) << "Extension";
        cout << setw(15) << right << " Size";
        cout << endl;
        cout << setfill('-') << setw(55) << "-" << endl;
        cout << setfill(' ');
        for (auto &Storedfile : files)
        {
            Storedfile.display();
        }
        cout << "\nYOUR PATH :  " << path << endl;
    }
    return SUCCESS;
}