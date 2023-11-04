

/*                              *******JAYPPEE INSTITUE OF INFORMATION TECHNOLOGY*******
                                                       SEC-62

                                          *OPERATING SYSTEM AND SYSTEM PROGRAMMING*
                
                                             SEMESTER-5 MINOR PROJECT


                INTRODUCTION:
                *                  *YOUR BUDDY TO HELP YOU LEARN UNIX COMMANDS*                 *
                    
                *                                     *SCRIPTIFY*                               *

                *                        *Where Shell Mastery Amplifies!*                       *


                SUBMITTED TO:-
                -->>KASHAV AJMERA                

                SUMITTED BY:-
                
                1.MADHURA V. JITURI (21803004)
                2.TANYA VASHISTHA   (21803006)
                3.ANSH MISHRA       (21803011)
                4.VIVEK SHAURYA     (21803013)
                

*/

//HEAFER FILES
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <iomanip>
using namespace std;

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() cout << "\033[H\033[J"

//FUNCTION TO OPEN THE PROGRAM IN A NEW TERMINAL
void openNewTerminalWindow()
{
    system("open -a Terminal ./scriptify");
}

//GREETING MESSAGE BEFORE STARTING THE SHELL
void init_shell()
{
    clear();
    cout << "\n\n\n\n";
    cout << endl << setw(20);
    for(int i =0 ; i<60;i++)
    cout<<"*";
    cout<<endl<<endl<<endl;
    cout << setw(55) <<"*  Scriptify  *";
    cout << endl << setw(57) << "*******************" << endl<<endl;
    cout << setw(64)<< "--Where Shell Mastery Amplifies!--" << endl;
    cout <<setw(64) << "**********************************" << endl<<endl;
    cout << endl << setw(20);
    for(int i =0 ; i<60;i++)
    cout<<"*";
    char *username = getenv("USER");
    cout<<endl;
    cout <<setw(74)<<"-->$" << username;
    cout << "\n";
    sleep(3);
    clear();
}

//FUNCTION TO TAKE INPUT
int takeInput(char *str)
{
    char *buf;
    buf = readline("\n-->> ");
    if (strlen(buf) != 0)
    {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    }
    else
    {
        return 1;
    }
}

// FUNCTION TO PRINT THE CURRENT DIRECTORY
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "\nDir: " << cwd;
}


//FUNCTION TO RUN THE SYSTEM COMMAND WITHOUT PIPES '|'
void execArgs(char **parsed)
{
    // Forking a child
    pid_t pid = fork();

    if (pid == -1)
    {
        cout << "\nFailed forking child..";
        return;
    }
    else if (pid == 0)
    {
        // Check for the 'cat' command and execute it using execvp
        if (strcmp(parsed[0], "cat") == 0)
        {
            execvp("/bin/cat", parsed);
        }
        else
        {
            // For other commands, execute using execvp
            if (execvp(parsed[0], parsed) < 0)
            {
                cout << "\nCould not execute command..";
            }
        }
        exit(0);
    }
    else
    {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}


//FUNCTION TO RUN THE SYSTEM COMMAND WITHOUT PIPES '|'
void execArgsPiped(char **parsed, char **parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0)
    {
        cout << "\nPipe could not be initialized";
        return;
    }
    p1 = fork();
    if (p1 < 0)
    {
        cout << "\nCould not fork";
        return;
    }

    if (p1 == 0)
    {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0)
        {
            cout << "\nCould not execute command 1..";
            exit(0);
        }
    }
    else
    {
        // Parent executing
        p2 = fork();

        if (p2 < 0)
        {
            cout << "\nCould not fork";
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0)
            {
                cout << "\nCould not execute command 2..";
                exit(0);
            }
        }
        else
        {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

//BUILTIN HELP COMMAND
void openHelp()
{
    cout << "\nWelcome to Scriptify"
         << "\nCopyright @Vivek Shaurya"
         << "\nWhere Shell Mastery Amplifies!"<<endl
         << "\nList of Commands (Custom Made) supported:"
         << "\n1.hello -> Greetings from the Developers."
         << "\n2.exit -> Exit the Shell."
         << "\n3.help -> Open Help File."
         << "\n4.cd -> Change Directory."
         << "\n5.whichcmd -> Search for a command."
         << "\n> And all other General Commands available in UNIX shell"
         << endl;
}

//FUNCTION TO SEARCH FOR SPECIFIC COMMAND AND THEIR COMMONLY USED FLAGS
void whichcmd( char **parsed)
{
        string input = "";
        for (int i = 1; i < MAXLIST && parsed[i] != NULL; i++)
        {
           input += parsed[i];
            if (parsed[i + 1] != NULL)
            {
                input += " ";
            }
        }
        if (input == "copy file") 
        {
            cout<<"-->>\t\'cp\'"
                <<"\n\t\'-r\' (copy directories recursively)"
                <<"\n\t\'-v\' (verbose, show details)"
                <<"\n\t\'-i\' (prompt before overwriting)"
            <<endl;
        } 
        else if (input == "change directory") 
        {
            cout<<"-->>\t\'cd\'"
                <<"\n\t-No Common Flags."
            <<endl;
        } 
        else if (input == "list files") 
        {
            cout<<"-->>\t\'ls\'"
                <<"\n\t\'-l\' (long format)"
                <<"\n\t\'-a\' (include hidden files)"
                <<"\n\t\'-h\' (human-readable sizes)"
            <<endl;
        } 
        else if (input == "where am i") 
        {
            cout<<"-->>\t\'pwd\'"
                <<"\n\t-No Common Flags."
            <<endl;
        } 
        else if (input == "make new directory") 
        {
            cout<<"-->>\t\'mkdir\'"
                <<"\n\t\'-p\' (create parent directories as needed)"
            <<endl;
        } 
        else if (input == "remove file") 
        {
            cout<<"-->>\t\'rm\'"
                <<"\n\t\'-r\' (remove directories and their contents recursively)"
                <<"\n\t\'-f\' (force removal without confirmation)"
                <<"\n\t\'-i\' (prompt before every removal)"
            <<endl;
        } 
        else if (input == "move file") 
        {
            cout<<"-->>\t\'mv\'"
                <<"\n\t\'-i\' (prompt before overwriting)"
                <<"\n\t\'-v\' (verbose, show details)"
            <<endl;
        } 
        else if (input == "create file") 
        {
            cout<<"-->>\t\'touch\'"
                <<"\n\t-No Common Flags."
            <<endl;
        } 
        else if (input == "display file") 
        {
            cout<<"-->>\t\'cat\'"
                <<"\n\t\'-n\' (number all output lines)"
                <<"\n\t\'-b\' (number non-blank output lines)"
            <<endl;
        } 
        else if (input == "search") 
        {
            cout<<"-->>\t\'grep\'"
                <<"\n\t\'-i\' (case-insensitive search)"
                <<"\n\t\'-r\' (search recursively in directories)"
                <<"\n\t\'-n\' (show line numbers)"
            <<endl;
        } 
        else if (input == "print") 
        {
            cout<<"-->>\t\'echo\'"
                <<"\n\t\'-e\' (enable interpretation of backslash escapes)"
                <<"\n\t\'-n\' (do not output the trailing newline)"
            <<endl;
        } 
        else if (input == "change file permission") 
        {
           cout<<"-->>\t\'chmod\'"
                <<"\n\t\'-R\' (change files and directories recursively)"
            <<endl;
        }
        else if (input == "process status") 
        {
            cout<<"-->>\t\'ps\'"
                <<"\n\t\'-aux\' (display all processes)"
                <<"\n\t\'-e\' (display all processes)"
            <<endl;
        }
        else if (input == "remove a process") 
        {
            cout<<"-->>\t\'kill\'¯"
                <<"\n\t-No Common Flags."
            <<endl;
        }
        else 
        {
            std::cout << "Command not recognized.\nWill be updated soon." << std::endl;
        }
}

//GRETTING FUNCTION FROM THE DEVELOPER.
void greeting(void)
{
    char *username;
    username = getenv("USER");
    cout << "\nHello " << username << "."
         << "\nPLAY AROUND WITH THE COMMANDS."
         << "\nUse \'help\' Command to learn More......." << endl;
}

void Goodbye()
{
    clear();
    cout<<endl<<endl<<endl<<endl;
    cout<<setw(55)<< "Developing Team:\n\n"
        <<setw(56)<< "-->>VIVEK SHAURYA\n"
        <<setw(54)<< "-->>ANSH MISHRA\n"
        <<setw(58)<< "-->>TANYA VASHISTHA\n"
        <<setw(60)<< "-->>MADHURA V. JITURI\n"
        <<endl;
    cout<<setw(57)<<"HAPPY HACKING......";
        sleep(3);
        exit(0);
}
//FUNCTION TO EXECUTE THE BUILTIN COMMANDS
int ownCmdHandler(char **parsed)
{
    int NoOfOwnCmds = 5, i, switchOwnArg = 0;
    const char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;

    //BUILTIN COMMANDS LIST.
    ListOfOwnCmds[0] = "hello";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "exit";
    ListOfOwnCmds[3] = "help";
    ListOfOwnCmds[4] = "whichcmd";



    for (i = 0; i < NoOfOwnCmds; i++)
    {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0)
        {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg)
    {
    case 1:
        greeting();
        return 1;
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        Goodbye();
    case 4:
        openHelp();
        return 1;
    case 5:
        whichcmd(parsed);
        return 1;
    default:
        break;
    }

    return 0;
}


// FUNCTION TO FIND ANY PIPE '|' IN THE COMMAND 
int parsePipe(char *str, char **strpiped)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else
    {
        return 1;
    }
}


// FUNCTION FOR PARSING THE COMMAND WORDS 
void parseSpace(char *str, char **parsed)
{
    int i;
    for (i = 0; i < MAXLIST; i++)
    {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}


//FUNCTION TO PROCESS THE ENTERED INPUT TEXT BY THE USER 
int processString(char *str, char **parsed, char **parsedpipe)
{
    char *strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped)
    {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    }
    else
    {
        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
    {
        return 0;
    }
    else
    {
        return 1 + piped;
    }
}


//MAIN FUNCTION
int main()
{
    openNewTerminalWindow();
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char *parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while (1)
    {
        cout<<endl;
        printDir();
        if (takeInput(inputString))
        {
            continue;
        }

        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
        /*
            exeflag -> '0' -> 'No Command.'
            exeflag -> '1' -> 'Command Without Pipe.'
            exeflag -> '2' -> 'Command With Pipe.'
        */

        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }

    return 0;
}
