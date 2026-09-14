#include <iostream>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <map>
#include <string>
#include <utility>
#include <format>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

// inilitiazation
class FileInfo;
pair<double, string> sizeconverter(long long bytesize);
string getsizecategory(long long sizedivider, const FileInfo &currentFile);

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
    void display(long long sizedivider, int nameWidth)
    {
        // formating
        string formatdata = format("{:.2f}{}", sizeconverter(size).first, sizeconverter(size).second);

        cout << setw(nameWidth) << left << filename;
        cout << setw(15) << extension;
        cout << setw(15) << right << formatdata;
        cout << setw(15) << right << getsizecategory(sizedivider, *this);
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

string getsizecategory(long long sizedivider, const FileInfo &currentFile)
{
    if (currentFile.getsize() < sizedivider)
    {
        return "Small";
    }
    else if (currentFile.getsize() < sizedivider * 2)
    {
        return "Medium";
    }
    else
    {
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
        return 0;
    }

    // intitialization
    int folderCounter = 0;
    size_t FiletoDisplay = 0;
    int maxNameLength = 0;
    int currentLength = 0;
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

                    storage[extension] += size;

                    // obj data storing
                    files.push_back(file);
                }

                else
                {
                    if (entry.is_directory())
                    {
                        cout << "THE FOLDER AVALIBLE IN PATH: " << entry.path().filename() << endl;
                        folderCounter++;
                    }
                    else
                    {
                        cout << "UNIDENTFIED OBJECT: " << entry.path().filename() << endl;
                    }
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
        if (folderCounter == 0)
        {
            cout << "NO FOLDER IN THIS DIRECTORY\n";
        }
        else
        {
            cout << "\033[32m" << "TOTAL FOLDER IN THIS DIRECTORY : " << "\033[0m" << folderCounter << endl;
        }
        cout << "NO REGULAR FILES IN THIS DIRECTORY\n";
        return SUCCESS;
    }
    else
    {
        for (const auto &Storedfile : files)
        {
            // maximum file length
            currentLength = Storedfile.getfilename().length();
            if (currentLength > maxNameLength)
            {
                maxNameLength = currentLength;
            }
            // total file size
            TotalSize += Storedfile.getsize();
            // max file details
            if (Storedfile.getsize() > MaxSize)
            {
                MaxSize = Storedfile.getsize();
                MaxFileName = Storedfile.getfilename();
            }
        }
        // sizeDivider calculation after maxSize is calculated
        sizeDivider = MaxSize / 3;
        if (sizeDivider == 0)
        {
            sizeDivider = 1;
        }
        // minimum width
        if (maxNameLength < 25)
        {
            maxNameLength = 25;
        }

        // -----------------------displaying file info------------------------------

        // formating
        string printdatamax = format("{:.2f}{}", sizeconverter(MaxSize).first, sizeconverter(MaxSize).second);
        string printdatatotal = format("{:.2f}{}", sizeconverter(TotalSize).first, sizeconverter(TotalSize).second);

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
        cout << "TOTAL SIZE : " << printdatatotal << endl;
        cout << "MAXIMUM FILE SIZE :" << printdatamax << "\nLARGE FILE NAME : " << MaxFileName << endl;
        cout << "TOTAL EXTENSION : " << storage.size() << endl;
        cout << "TOTAL FOLDER IN THIS DIRECTORY : " << folderCounter << endl;
        for (auto &item : storage)
        {
            string printdata = format("{:.2f}{}", sizeconverter(item.second).first, sizeconverter(item.second).second);
            cout << setw(10) << left << item.first << " : " << printdata << endl;
        }

        // -------------------top large file------------------
        cout << "\033[34m";
        cout << "========================\n";
        cout << "TOP LARGE FILE \n";
        cout << "========================\n";
        cout << "\033[0m";

        // calculating filetodisplay
        FiletoDisplay = min(files.size(), size_t(5));

        // creating a copy vector--> so that the orignal scan order remains same
        vector<FileInfo> copy_fileSorter;
        copy_fileSorter=files;

        sort(copy_fileSorter.begin(), copy_fileSorter.end(),
             [](const FileInfo &a, const FileInfo &b)
             { return a.getsize() > b.getsize(); });
        //  [] -> yeh hai lambda function new chiz sikhe hai...
        for (size_t i = 0; i < FiletoDisplay; i++)
        {
            copy_fileSorter[i].display(sizeDivider, maxNameLength);
        }
        cout << "\n\n";

        // --------------------all file table-------------------------------
        // heading code of table
        cout << "\033[32m";
        cout << "========================\n";
        cout << "ALL FILES AVALIBLE \n";
        cout << "========================\n";
        cout << "\033[0m";
        // future mai yeh hardcoded value ko dynamic bana dege
        int extensionWidth = 15;
        int sizeWidth = 15;
        int categoryWidth = 18;
        cout << setw(maxNameLength) << left << "Name";
        cout << setw(extensionWidth) << "Extension";
        cout << setw(sizeWidth) << right << " Size";
        cout << setw(categoryWidth) << right << " Category";
        cout << endl;
        cout << setfill('-') << setw(maxNameLength + (extensionWidth + sizeWidth + categoryWidth)) << "-" << endl;
        cout << setfill(' ');

        for (auto &Storedfile : files)
        {
            Storedfile.display(sizeDivider, maxNameLength);
        }
        cout << "\nYOUR PATH :  " << path << endl;
    }
    return SUCCESS;
}