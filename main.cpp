#include <iostream>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <map>
#include <string>
#include <utility>
#include <format>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>
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
    string relativePath;
    long long size;

public:
    FileInfo(string f, string e, string r, long long s) : filename(f), extension(e), relativePath(r), size(s)
    {
    }
    string getrelativePath() const
    {
        return relativePath;
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
    void display(long long sizedivider, int nameWidth, int relativePathwidth)
    {
        // formating
        string formatdata = format("{:.2f}{}", sizeconverter(size).first, sizeconverter(size).second);

        cout << setw(nameWidth) << left << filename << "  ";
        cout << setw(relativePathwidth) << relativePath << "  ";
        cout << setw(15) << extension << "  ";
        cout << setw(15) << right << formatdata << "  ";
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

string getcategory(string extension, const map<string, string> &category)
{
    auto result = category.find(extension);
    if (result != category.end())
    {
        // ab bus category decide karna hai but how????
        return result->second;
    }
    else
    {
        return "other";
    }
}

void DynamicBar(double percentage, int Max_bar_length)
{
    if (percentage < 0)
    {
        percentage = 0;
    }
    if (percentage > 100)
    {
        percentage = 100;
    }
    int block = round((percentage / 100) * Max_bar_length);
    // scaning bar
    cout << "\r[";
    cout << string(block, '#');
    cout << string(Max_bar_length - block, '-');
    cout << "] " << round(percentage) << "%";
    cout.flush();
    if (percentage == 100)
    {
        cout << "\r" << string(60, ' ') << "\r";
    }
    // this_thread::sleep_for(chrono::milliseconds(50));
    //   --> yeh line program ko slow karta hai(USED FOR TESTING)
}

void StaticBar(double percentage, int Max_bar_length)
{
    int block = round((percentage / 100) * Max_bar_length);
    // static bar
    cout << "[";
    cout << string(block, '#');
    cout << string(Max_bar_length - block, '-');
    cout << "] " << setw(3) << setprecision(6) << (percentage) << "%";
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
    bool scanComplete = true;
    bool countComplete = true;
    const int Max_bar_length = 40;
    int folderCounter = 0;
    size_t FiletoDisplay = 0;
    size_t Totalentries = 0;
    size_t processedEntrycounter = 0;
    long long sizeDivider;
    int SkippedEntries = 0;
    long long TotalSize = 0;
    long long MaxSize = 0;
    string MaxFileName;
    vector<FileInfo> files;
    const int SUCCESS = 0;
    const int filesystemerror = 1;
    const int Invalidpath = 2;

    // maps-------------
    map<string, long long> storage;
    map<string, string> category;
    map<string, long long> categoryStorage;

    // Videos
    category[".mp4"] = "Videos";
    category[".mkv"] = "Videos";
    category[".avi"] = "Videos";
    category[".mov"] = "Videos";
    category[".wmv"] = "Videos";
    category[".webm"] = "Videos";

    // Images
    category[".jpg"] = "Images";
    category[".jpeg"] = "Images";
    category[".png"] = "Images";
    category[".gif"] = "Images";
    category[".bmp"] = "Images";
    category[".webp"] = "Images";

    // Audio
    category[".mp3"] = "Audio";
    category[".wav"] = "Audio";
    category[".flac"] = "Audio";
    category[".aac"] = "Audio";
    category[".ogg"] = "Audio";

    // Documents
    category[".pdf"] = "Documents";
    category[".txt"] = "Documents";
    category[".doc"] = "Documents";
    category[".docx"] = "Documents";
    category[".xls"] = "Documents";
    category[".xlsx"] = "Documents";
    category[".ppt"] = "Documents";
    category[".pptx"] = "Documents";

    // Applications
    category[".exe"] = "Applications";
    category[".msi"] = "Applications";

    // Archives
    category[".zip"] = "Archives";
    category[".rar"] = "Archives";
    category[".7z"] = "Archives";
    category[".tar"] = "Archives";
    category[".gz"] = "Archives";

    // Coding
    category[".cpp"] = "Coding";
    category[".h"] = "Coding";
    category[".hpp"] = "Coding";
    category[".c"] = "Coding";
    category[".py"] = "Coding";
    category[".java"] = "Coding";
    category[".js"] = "Coding";
    category[".ts"] = "Coding";
    category[".html"] = "Coding";
    category[".css"] = "Coding";
    category[".php"] = "Coding";
    category[".cs"] = "Coding";
    category[".go"] = "Coding";
    category[".rs"] = "Coding";

    // table width
    int currentLocationLength = 0;
    int maxLocationLength = 0;
    int maxNameLength = 0;
    int currentLength = 0;

    try
    {
        fs::directory_entry Directory{path};
        // auto check
        // fs::recursive_directory_iterator RECURSIVE{path, fs::directory_options::skip_permission_denied};

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

        // manual iterator
        std::error_code CountEc;
        std::error_code ScanEc;
        fs::recursive_directory_iterator Countit(path, fs::directory_options::skip_permission_denied, CountEc);
        fs::recursive_directory_iterator Scanit(path, fs::directory_options::skip_permission_denied, ScanEc);
        fs::recursive_directory_iterator end;

        // error code constructor exception handling
        if (CountEc)
        {
            cout << "COUNT START ERROR:" << CountEc.message() << endl;
            return filesystemerror;
        }
        if (ScanEc)
        {
            cout << "SCAN START ERROR:" << ScanEc.message() << endl;
            return filesystemerror;
        }

        cout << "\033[33m";
        cout << "ITS A FOLDER DIRECTORY" << endl;
        cout << "\033[0m";

        // DIRECTORY SCANING
        while (Countit != end)
        {
            Totalentries++;
            Countit.increment(CountEc);
            if (CountEc)
            {
                cout << "COUNTING ITERATION ERROR :" << CountEc.message() << endl;
                countComplete = false;
                break;
                // Countit.pop(ec);
                // ec.clear();
                // Countit.increment(ec);
                // return filesystemerror;
            }
        }
        if (Totalentries == 0)
        {
            cout << "Empty Dirrectory\n";
            return 0;
        }
        // DIRECTORY ---------------------------------------------
        // for (const auto &entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied))
        while (Scanit != end)
        {
            const auto &entry = *Scanit;
            try
            {
                if (entry.is_regular_file())
                {

                    auto filename = entry.path().filename().stem().string();
                    auto extension = entry.path().extension().string();
                    auto size = entry.file_size();

                    fs::path parentpath = entry.path().parent_path();
                    fs::path relativefilepath = fs::relative(parentpath, path);

                    FileInfo file(filename, extension, relativefilepath.string(), size);

                    storage[extension] += size;
                    string categoryName = getcategory(extension, category);
                    categoryStorage[categoryName] += size;
                    // cout<<"extension"<<extension<<" | category "<<categoryName<<endl;
                    // category[extension] += getcategory(extension,category);
                    // cout<<getcategory(extension,category);

                    // obj data storing
                    files.push_back(file);
                }
                else
                {
                    if (entry.is_directory())
                    {
                        // cout << "THE FOLDER AVALIBLE IN PATH: " << entry.path().filename() << endl;--> yeh line problem kar raha hai
                        folderCounter++;
                    }
                    else
                    {
                        // cout << "UNIDENTFIED OBJECT : " << entry.path().filename() << endl;
                    }
                }
            }
            catch (const fs::filesystem_error &e)
            {
                // cout << "Skipped entries : " << e.what() << endl;
                SkippedEntries++;
            }
            Scanit.increment(ScanEc);
            if (ScanEc)
            {
                // Scanit.pop(ec);
                // ec.clear();
                cout << "Scan iteration failed : " << ScanEc.message() << endl;
                scanComplete = false;
                break;
                // return filesystemerror;
            }
            // -------------------------scaning bar data--------------------------------------
            processedEntrycounter++;
            if (countComplete)
            {
                double DynamicBarpercentage = ((double(processedEntrycounter) / double(Totalentries)) * 100);

                DynamicBar(DynamicBarpercentage, Max_bar_length);
            }
        }

        if (countComplete && scanComplete)
        {
            cout << "\033[32m" << "Scanning complete!\n"
                 << "\033[0m";
        }
        else
        {
            cout << "\033[31m" << "Scanning interupted!\n"
                 << "\033[0m";
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

        // -----------------------displaying file info------------------------------

        // formating
        string printdatamax = format("{:.2f}{}", sizeconverter(MaxSize).first, sizeconverter(MaxSize).second);
        string printdatatotal = format("{:.2f}{}", sizeconverter(TotalSize).first, sizeconverter(TotalSize).second);

        if (SkippedEntries > 0)
        {
            cout << "\033[31m";
            cout << SkippedEntries << " ENTRIES SKIPPED DUE TO ACCESS/PERMISSION ERROR\n";
            cout << "\033[0m";
        }
        // ---------------------------scan summary----------------------
        cout << "\033[32m";
        cout << "==================================\n";
        cout << "SCAN SUMMARY\n";
        cout << "==================================\n";
        cout << "\033[0m";
        cout << left;
        cout << setw(20) << "FILES FOUND       : " << files.size() << endl;
        cout << setw(20) << "FOLDER FOUND      : " << folderCounter << endl;
        cout << setw(20) << "TOTAL STORAGE     : " << printdatatotal << endl;
        cout << setw(20) << "LARGEST FILE SIZE : " << printdatamax << endl;
        cout << setw(20) << "LARGE FILE NAME   : " << MaxFileName << endl;
        cout << setw(20) << "TOTAL EXTENSION   : " << storage.size() << endl;
        cout << right;

        // -------------------storage analysis---------------------
        cout << "\033[32m";
        cout << "========================\n";
        cout << "STORAGE ANALYSIS \n";
        cout << "========================\n";
        cout << "\033[0m";

        double percentage = 0;

        for (auto &item : categoryStorage)
        {
            string printdata = format("{:.2f}{}", sizeconverter(item.second).first, sizeconverter(item.second).second); // bytes to kb,mb,gb convert
            if (TotalSize == 0)
            {
                percentage = 0;
            }
            else
            {
                percentage = ((double(item.second) / double(TotalSize)) * 100); //-> percentage calculation
            }

            if (item.first == "")
            {
                cout << setw(10) << left << "No Extension";
            }
            else
            {
                cout << setw(12) << left << item.first;
            }

            StaticBar(percentage, Max_bar_length);
            cout << " : " << printdata << endl;
        }

        cout << endl;

        // -------------------top large file------------------
        cout << "\033[32m";
        cout << "========================\n";
        cout << "TOP LARGE FILE \n";
        cout << "========================\n";
        cout << "\033[0m";
        // --------------------------------------------calculation--------------------------------------------
        // calculating filetodisplay
        FiletoDisplay = min(files.size(), size_t(5));

        // creating a copy vector--> so that the orignal scan order remains same
        vector<FileInfo> copy_fileSorter;
        copy_fileSorter = files;

        sort(copy_fileSorter.begin(), copy_fileSorter.end(),
             [](const FileInfo &a, const FileInfo &b)
             { return a.getsize() > b.getsize(); });
        //  [] -> yeh hai lambda function new chiz sikhe hai...

        // ---------------------------------------------printing-----------------------------------------------------

        // setw width calulation
        for (size_t i = 0; i < FiletoDisplay; i++)
        {
            // maximum location length

            currentLocationLength = copy_fileSorter[i].getrelativePath().length();
            if (currentLocationLength > maxLocationLength)
            {
                maxLocationLength = currentLocationLength;
            }

            // maximum filename  length
            currentLength = copy_fileSorter[i].getfilename().length();
            if (currentLength > maxNameLength)
            {
                maxNameLength = currentLength;
            }
        }
        // minimum width
        if (maxNameLength < 25)
        {
            maxNameLength = 25;
        }
        if (maxLocationLength < 15)
        {
            maxLocationLength = 15;
        }
        // -------heading-------
        // // future mai yeh hardcoded value ko dynamic bana dege
        int extensionWidth = 15;
        int sizeWidth = 15;
        int categoryWidth = 18;

        cout << setw(maxNameLength) << left << "Name" << "  ";
        cout << setw(maxLocationLength) << "Location" << "  ";
        cout << setw(extensionWidth) << "Extension" << "  ";
        cout << setw(sizeWidth) << right << "Size" << "  ";
        cout << setw(categoryWidth) << right << "Category" << "  ";
        cout << endl;
        cout << setfill('-') << setw(maxNameLength + (extensionWidth + sizeWidth + categoryWidth + (maxLocationLength) + 8)) << "-" << endl; //+8 because of gaps.
        cout << setfill(' ');

        for (size_t i = 0; i < FiletoDisplay; i++)
        {
            copy_fileSorter[i].display(sizeDivider, maxNameLength, maxLocationLength);
        }
        cout << "\n\n";

        // // --------------------all file table-------------------------------
        // // heading code of table
        // cout << "\033[32m";
        // cout << "========================\n";
        // cout << "ALL FILES AVALIBLE \n";
        // cout << "========================\n";
        // cout << "\033[0m";
        // // future mai yeh hardcoded value ko dynamic bana dege
        // int extensionWidth = 15;
        // int locationWidth = 15;
        // int sizeWidth = 15;
        // int categoryWidth = 18;

        // cout << setw(maxNameLength) << left << "Name";
        // cout << setw(locationWidth) << "Location";
        // cout << setw(extensionWidth) << "Extension";
        // cout << setw(sizeWidth) << right << " Size";
        // cout << setw(categoryWidth) << right << " Category";
        // cout << endl;
        // cout << setfill('-') << setw(maxNameLength + (extensionWidth + sizeWidth + categoryWidth+locationWidth)) << "-" << endl;
        // cout << setfill(' ');

        // for (auto &Storedfile : files)
        // {
        //     Storedfile.display(sizeDivider, maxNameLength);
        // }
        cout << "\nYOUR PATH :  " << path << endl;
    }
    return SUCCESS;
}