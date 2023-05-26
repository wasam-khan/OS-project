#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <fstream>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>


#define TOKENSIZE 100

using namespace std;


void StrTokenizer(char *line, char **argv);
void myExecvp(char **argv);
int GetEnv();

void ChangeDirectory(char **argv);
void RunCatCommand(char **argv);
void RunCopyCommand(char **argv);
void FreeMemory(char **argv);
void AppendStringToFile(const string& filename, const string& content);
void RunEchoCommand(char **argv);
void RunMkdirCommand(char **argv);
void RunTouchCommand(char **argv);
void RunSleepCommand(char **argv);
void RunAlarmCommand(char **argv);
void myGrep(const string &pattern, const string &filename);
void SwapFiles(const string &file1, const string &file2);
void KillProcess(const char *pid);
void GetSystemInfo();
void DeleteEmptyFiles();
void SortedListWithSize();


int main()
{
    char *path2;
    char *arr[250];
    char *Tokenized;
    char input[250];
    string timestamp;
    vector<pair<string,string>> commandHistory;
    char *argv[TOKENSIZE];

    while (true)
    {	
    	
    	time_t now = time(0);
	tm *ltm = localtime(&now);

	// Format the timestamp	
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);

	timestamp = buffer;
    
 
        cout << "Pathan Shell-> ";
        cin.getline(input, 250);
        StrTokenizer(input, argv);
        if (strcmp(input, "exit") == 0)
        {
            break;
        }
        else if (strcmp(input, "\n") == 0)
        {
            continue;
        }
        else if (strcmp(input, "history") == 0)
        {
            cout << "Command History:" << endl;
            for (const auto& command : commandHistory)
            {
		    

                // Print the timestamp and command
                cout << "[" << command.first << "] " << command.second << endl;
            }
            continue;
        }

        
        else if (strcmp(argv[0], "cd") == 0)
        {
            ChangeDirectory(argv);
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "mkdir") == 0)
    	{
        	RunMkdirCommand(argv);
        	commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
        	continue;
    	}
    	else if (strcmp(argv[0], "grep") == 0)
        {
            if (argv[1] == nullptr || argv[2] == nullptr)
            {
                cout << "Error: grep <pattern> <file> is missing" << endl;
                continue;
            }

            string pattern(argv[1]);
            string filename(argv[2]);
            myGrep(pattern, filename);
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
        	continue;
        }
    	else if (strcmp(argv[0], "alarm") == 0)
    	{
        	RunAlarmCommand(argv);
        	commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
        	continue;
    	}
    	else if (strcmp(argv[0], "sleep") == 0)
    	{
        	RunSleepCommand(argv);
        	commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
        	continue;
    	}
        
        else if (strcmp(argv[0], "cat") == 0)
        {
            RunCatCommand(argv);
      	    commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "swap") == 0)
        {
            if (argv[1] == nullptr || argv[2] == nullptr)
            {
                cout << "Error: swapfiles <file1> <file2> is missing" << endl;
                continue;
            }

            string file1(argv[1]);
            string file2(argv[2]);
            SwapFiles(file1, file2);
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "echo") == 0)
        {
            RunEchoCommand(argv);
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "touch") == 0)
    	{
        	RunTouchCommand(argv);
        	commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
        	continue;
    	}
    	
    	else if (strcmp(argv[0], "kill") == 0)
    	{
        	if (argv[1] != NULL)
        	{   
        	    KillProcess(argv[1]);
        	    commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            
           
        	}
        	else
        	{
        	    cout << "Missing process ID for kill command!" << endl;
            
        	}
    	}
    	
    	
        else if (strcmp(argv[0], "copy") == 0)
        {
            RunCopyCommand(argv);
      	    commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "sortwithsize") == 0)
        {
            SortedListWithSize();
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        
          else if (strcmp(argv[0], "delemptyfiles") == 0)
        {
            DeleteEmptyFiles();
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "systeminfo") == 0)
        {
            GetSystemInfo();
      	    commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        else if (strcmp(argv[0], "append") == 0)
        {
            if (argv[1] == nullptr || argv[2] == nullptr)
            {
                cout << "Error: append <file> <content> is missing" << endl;
                continue;
            }

            AppendStringToFile(argv[1], argv[2]);
            commandHistory.push_back(make_pair(timestamp,argv[0])); // Add the command to history
            continue;
        }
        string command(input);
        commandHistory.push_back(make_pair(timestamp,command)); // Add the command to history
        myExecvp(argv);
    }
  
    
    return 0;
}


void GetSystemInfo(){
	system("uname -a");
}

void myExecvp(char **argv)
{
    pid_t pid;
    int status;
    int childStatus;
    pid = fork();
    if (pid == 0)
    {
        childStatus = execvp(*argv, argv);
        if (childStatus < 0)
        {
            cout << "ERROR: wrong input" << endl;
        }
        exit(0);
    }
    else if (pid < 0)
    {
        cout << "something went wrong!" << endl;
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}






void myGrep(const string& pattern, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: File " << filename << " does not exist." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.find(pattern) != string::npos) {
            cout << line << std::endl;
        }
    }

    file.close();
}


void StrTokenizer(char *input, char **argv)
{
    char *stringTokenized;
    stringTokenized = strtok(input, " ");
    while (stringTokenized != NULL)
    {
        *argv++ = stringTokenized;
        stringTokenized = strtok(NULL, " ");
    }
    *argv = NULL;
}

int GetEnv()
{
    char *path2;
    char *arr2[250];
    char *Tokenized;
    path2 = getenv("PATH");
    Tokenized = strtok(path2, ":");
    int k = 0;
    while (Tokenized != NULL)
    {
        arr2[k] = Tokenized;
        Tokenized = strtok(NULL, ":");
        k++;
    }
    arr2[k] = NULL;
}

void RunSleepCommand(char **argv)
{
    if (argv[1] == nullptr)
    {
        cout << "Error: sleep <seconds> is missing" << endl;
        return;
    }

    unsigned int seconds = atoi(argv[1]);
    cout << "Sleeping for " << seconds << " seconds." << endl;
    sleep(seconds);

    
}




void ChangeDirectory(char **argv)
{
    if (argv[1] == NULL)
    {
        cout << "Error: cd <directory> is missing" << endl;
        return;
    }

    if (chdir(argv[1]) != 0)
    {
        cout << "Error changing directory to " << argv[1] << endl;
    }
    
    
}


void KillProcess(const char *pid)
{
    int processID = atoi(pid);
    if (kill(processID, SIGKILL) == -1)
    {
        perror("kill() error");
       
    }

    cout << "Process terminated successfully!" << endl;
   
}




void SortedListWithSize()
{
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid < 0)
    {
        cout << "Error forking process." << endl;
        return;
    }
    else if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        char* argv1[] = { (char*)"du", (char*)"-h", (char*)"--max-depth=1", (char*)".", NULL };
        execvp(argv1[0], argv1);

    }
    else
    {
        wait(NULL);

        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        char* argv2[] = { (char*)"sort", (char*)"-hr", NULL };
        execvp(argv2[0], argv2);
        exit(EXIT_FAILURE);
    }
}


void RunAlarmCommand(char **argv)
{
    if (argv[1] == nullptr)
    {
        cout << "Error: alarm <seconds> is missing" << endl;
        return;
    }

    unsigned int seconds = atoi(argv[1]);
    cout << "Alarm set for " << seconds << " seconds." << endl;
    alarm(seconds);

    
}


void RunMkdirCommand(char **argv)
{
    if (argv[1] == nullptr)
    {
        cout << "Error: mkdir <directory_name> is missing" << endl;
        return;
    }

    if (mkdir(argv[1], 0777) == 0)
    {
        cout << "Directory created successfully: " << argv[1] << endl;
    }
    else
    {
        cout << "Error creating directory: " << argv[1] << endl;
    }
}

void RunTouchCommand(char **argv)
{
    if (argv[1] == nullptr)
    {
        std::cout << "Error: touch <filename> is missing" << std::endl;
        return;
    }

    ofstream file(argv[1]);
    if (!file)
    {
        std::cout << "Error creating file: " << argv[1] << std::endl;
        return;
    }

    file.close();

    cout << "File created successfully: " << argv[1] << std::endl;
}

void RunEchoCommand(char **argv)
{
    int i = 1;
    while (argv[i] != nullptr)
    {
        cout << argv[i] << " ";
        i++;
    }
    cout << endl;
}
void DeleteEmptyFiles()
{
    char* argv[] = { (char*)"find", (char*)".", (char*)"-type", (char*)"f", (char*)"-empty", (char*)"-delete", nullptr };
    myExecvp(argv);
}


void SwapFiles(const string &file1, const string &file2)
{
    // Read the content of file1
    ifstream file1Input(file1);
    if (!file1Input)
    {
        cout << "Error: File " << file1 << " does not exist." << endl;
        return;
    }

    string file1Content((istreambuf_iterator<char>(file1Input)), (istreambuf_iterator<char>()));
    file1Input.close();

    // Read the content of file2
    ifstream file2Input(file2);
    if (!file2Input)
    {
        cout << "Error: File " << file2 << " does not exist." << endl;
        return;
    }

    string file2Content((istreambuf_iterator<char>(file2Input)), (istreambuf_iterator<char>()));
    file2Input.close();

    // Write the content of file1 to file2
    ofstream file2Output(file2);
    if (!file2Output)
    {
        cout << "Error: Failed to open file " << file2 << " for writing." << endl;
        return;
    }

    file2Output << file1Content;
    file2Output.close();

    // Write the content of file2 to file1
    ofstream file1Output(file1);
    if (!file1Output)
    {
        cout << "Error: Failed to open file " << file1 << " for writing." << endl;
        return;
    }

    file1Output << file2Content;
    file1Output.close();

    cout << "Files " << file1 << " and " << file2 << " swapped successfully." << endl;
}







void RunCatCommand(char **argv)
{
    if (argv[1] == nullptr)
    {
        cout << "Error: cat <filename> is missing" << endl;
        return;
    }

    ifstream file(argv[1]);
    if (!file)
    {
        cout << "Error: File " << argv[1] << " does not exist." << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
}

void RunCopyCommand(char **argv)
{
    if (argv[1] == nullptr || argv[2] == nullptr)
    {
        cout << "Error: copy <source_file> <destination_file> is missing" << endl;
        return;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        cout << "Error: Source file " << argv[1] << " does not exist." << endl;
        return;
    }

    ofstream outputFile(argv[2], ios::app);
    if (!outputFile)
    {
        cout << "Error: Failed to open destination file " << argv[2] << " for appending." << endl;
        return;
    }

    string line;
    while (getline(inputFile, line))
    {
        outputFile << line << endl;
    }

    cout << "File copied successfully from " << argv[1] << " to " << argv[2] << endl;
}


void AppendStringToFile(const string& filename, const string& content)
{
    ofstream file(filename, ios::app);
    if (file.is_open())
    {
        file << content << endl;
        file.close();
        cout << "String appended to file successfully." << endl;
    }
    else
    {
        cout << "Error: Failed to open the file for appending." << endl;
    }
}
