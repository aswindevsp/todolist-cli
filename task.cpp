#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void help(){
    cout << "Usage :-" << endl;
    cout << "$ ./task add 2 hello world    # Add a new item with priority 2 and text \"hello world\" to the list" << endl;
    cout << "$ ./task ls                   # Show incomplete priority list items sorted by priority in ascending order" << endl;
    cout << "$ ./task del INDEX            # Delete the incomplete item with the given index" << endl;
    cout << "$ ./task done INDEX           # Mark the incomplete item with the given index as complete" << endl;
    cout << "$ ./task help                 # Show usage" << endl;
    cout << "$ ./task report               # Statistics" << endl;
}

void add(string priority, string task){
    stringstream conv(priority); //Converting prority string to number
    int priorityNum; 
    conv >> priorityNum;

    ifstream taskFile;
    ofstream tempFile;

    taskFile.open("task.txt");
    tempFile.open("temp.txt");

    string line;


    int flag = 0;
    getline(taskFile, line);

    if(taskFile.eof()){
        tempFile << priorityNum << " " << task << endl;
    }else{
        while(!taskFile.eof()){
            int tempPriority = line[0] - '0';
            if(priorityNum == tempPriority && flag == 0){
                tempFile << line << endl;
                tempFile << priorityNum << " " << task << endl;
                flag = 1;
            }else if(tempPriority < priorityNum){
                tempFile << line << endl;
            }else if(priorityNum < tempPriority && flag == 0){
               tempFile << priorityNum << " " << task << endl;
               flag = 1;
            }
            getline(taskFile, line);
        }
        if(flag == 0)
        tempFile << priorityNum << " " << task << endl;
    }

    taskFile.close();
    tempFile.close();

    remove("task.txt");
    rename("temp.txt", "task.txt");

    cout << "Added task: " <<"\"" << task <<"\"" << " with priority " << priority << endl;


}

void ls(){
    string line;
    char priority;

    ifstream taskFile;
    taskFile.open("task.txt");

    int n = 1;
    getline(taskFile, line);
    if(taskFile.eof())
        cout << "There are no pending tasks!" << endl;
    else{
        while(!taskFile.eof()){
            if(line != ""){
                priority = line[0];
                line.erase(0,1);
                line.erase(0,1);
                cout << n << ". ";
                cout << line << " [" << priority << "]"<< endl;
                n++;
                getline(taskFile, line);
            }
        }
    }  
}

void del(int n){
    string line;

    ifstream taskFile;
    ofstream tempFile;

    taskFile.open("task.txt");
    tempFile.open("temp.txt");
    
    int count = -1;
    while(!taskFile.eof()){
        getline(taskFile, line);
        if(n != count){
            tempFile << line << endl;
        }
        count++;
    }
    if(n > count || n == 0)
        cout << "Error: task with index #"<< n << " does not exist. Nothing deleted." << endl;
    else    
        cout << "Deleted task #" << n << endl;

    taskFile.close();
    tempFile.close();
    remove("task.txt");
    rename("temp.txt", "task.txt");

}

void done(int n){
    
    ifstream taskFile("task.txt");
    ofstream tempFile("temp.txt");

    string line; // line in task.txt
    string done; // completed task from task.txt

    getline(taskFile, line);
    int count = 1;
    while(!taskFile.eof()){
        if(n == count){
            done = line;
        }else{
            tempFile << line << endl;
        }
        getline(taskFile, line);
        count++;
    }
    tempFile << line;
    taskFile.close();
    tempFile.close();
    remove("task.txt");
    rename("temp.txt", "task.txt");
    

    // Moving task to completed.txt

    ifstream compFile("completed.txt");
    tempFile.open("temp.txt");


    string str;
    compFile >> str;

    done = done.substr(done.find_first_of(" \t")+1);  // Removing prority from task.

    if(str.empty())
        tempFile << done;
    else{
        compFile.close();
        compFile.open("completed.txt");
        while(!compFile.eof()){
            getline(compFile, line);
            tempFile << line << endl;
        }
        tempFile << done;
    }

    compFile.close();
    tempFile.close();

    remove("completed.txt");
    rename("temp.txt", "completed.txt");

    if(done != "")
        cout << "Marked item as done." << endl;
    else
        cout << "Error: no incomplete item with index #"<< n <<" exists." << endl;

}

void report(){
    
    ifstream taskFile;
    ifstream compFile;

    taskFile.open("task.txt");
    compFile.open("completed.txt");

    int taskCount = 0;
    int compCount = 0;

    string line;

    while(!taskFile.eof()){
        taskCount++;
        getline(taskFile, line);
    }
    while(!compFile.eof()){
        compCount++;
        getline(compFile, line);

    }

    cout << "Pending : " << taskCount-1 << endl;
    ls();
    cout << endl;
    cout << "Completed : " << compCount << endl;

    compFile.close();
    compFile.open("completed.txt");
    int n = 1;
    while(!compFile.eof()){
        compCount++;
        getline(compFile, line);
        if(line != "")
            cout << n << ". " << line << endl;
        n++;
    }

    taskFile.close();
    compFile.close();

}
    
void check(){
    ifstream taskFile; // task.txt
    ifstream completedFile; //completed.txt
    taskFile.open("task.txt");
    completedFile.open("completed.txt");

    if(!taskFile){
        taskFile.close();
        ofstream taskFile("task.txt");
    }
    if(!completedFile){
        completedFile.close();
        ofstream compFile("completed.txt");
    }

    completedFile.close();
    taskFile.close();

}


int main(int argc, char* argv[])
{

    check(); // create task.txt and completed.txt if not exist.

    if (argc == 1 || string(argv[1]) == "help")
        help();

    else if(string(argv[1]) == "add"){
        if(argc <=2 )
            cout << "Error: Missing tasks string. Nothing added!" << endl;
        else
            add(argv[2], argv[3]);
    }

    else if(string(argv[1]) == "del"){
        if(argc <=2 )
            cout << "Error: Missing NUMBER for deleting tasks." << endl;
        else
            del(atoi(argv[2]));
    }

    else if(string(argv[1]) == "done")
        if(argc <=2 )
            cout << "Error: Missing NUMBER for marking tasks as done." << endl;
        else
        done(atoi(argv[2]));

    else if(string(argv[1]) == "ls")
        ls();
    else if(string(argv[1]) == "report")
        report();

    
}
