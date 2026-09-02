#include <iostream>
#include<filesystem>
using namespace std;
namespace fs = std::filesystem;

int main(){
    cout<<"============================="<<endl;
    cout<<"     |PC GUARDIAN v1.0|  "<<endl;
    cout<<"============================="<<endl;
    cout<<"ENTER DIRECTORY PATH : ";
    string path;
    getline(cin,path);
    fs::directory_entry Directory{path};
    if(Directory.exists()){
        if(Directory.is_directory()){
            cout<<"ITS A FOLDER DIRECTORY"<<endl;
        }
        else if(Directory.is_regular_file()){
            cout<<"ITS A FILE DIRECTORY"<<endl;
        }
        else{
            cout<<"SOMETHING WENT WRONG!!"<<endl;
        }
    }
    else{
        cout<<"DIRECTORY DOES NOT EXIST!!"<<endl;
    }
    cout<<"YOUR PATH :  "<<path<<endl;
    return 0;
}