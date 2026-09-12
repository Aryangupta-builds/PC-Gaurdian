#include <iostream>
#include <filesystem>
#include <vector>
#include <iomanip>
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
        cout << setw(15) <<right<< size;
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

    // intitialization
    fs::directory_entry Directory{path};
    vector<FileInfo> files;

    if (Directory.exists())
    {
        if (Directory.is_directory())
        {
            cout<<"\033[33m";
            cout << "ITS A FOLDER DIRECTORY" << endl;
            cout<<"\033[0m";
            for (const auto &entry : fs::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    auto filename = entry.path().filename().string();
                    auto extension = entry.path().extension().string();
                    auto size = entry.file_size();

                    FileInfo file(filename, extension, size);

                    // obj data storing
                    files.push_back(file);
                }
                else
                {
                    cout << "THE FOLDER AVALIBLE IN PATH: " << entry.path().filename() << endl;
                }
            }
        }
        else if (Directory.is_regular_file())
        {
            cout << "ITS A FILE PATH" << endl;
        }
        else
        {
            cout << "SOMETHING WENT WRONG!!" << endl;
        }
    }

    else
    {
        cout << "DIRECTORY DOES NOT EXIST!!" << endl;
    }
    // displaying file info
    cout<<"\033[32m";
    cout<<"\nTHE FILES AVALIBE IN THE PATH :\n\n";
    cout<<"\033[0m";
    // heading code
    cout << setw(25) << left << "Name";
        cout << setw(15) << "Extension";
        cout << setw(15) <<right<< " Size";
        cout << endl;
        cout << setfill('-') << setw(55) << "-" << endl;
        cout << setfill(' ');
    for (auto &Storedfile : files)
    {
        Storedfile.display();
    }
    cout << "\nYOUR PATH :  " << path << endl;
    return 0;
}