#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAXLINE 200  /* This is how we declare constants in C */
#define MAXARGS 20

// Used to keep track of cases, sorry for unclear code
static int z = 0;

/* In C, "static" means not visible outside of file.  This is different
* from the usage of "static" in Java.
* Note that end_ptr is an output parameter.
*/
static char * getword(char * begin, char **end_ptr) {
        char * end = begin;

        while ( *begin == ' ' )
                begin++;  /* Get rid of leading spaces. */
        end = begin;
        while ( *end != '\0' && *end != '\n' && *end != ' ' )
                end++;  /* Keep going. */
        if ( end == begin )
                return NULL;  /* if no more words, return NULL */
        *end = '\0';  /* else put string terminator at end of this word. */
        *end_ptr = end;
        if (begin[0] == '$') { /* if this is a variable to be expanded */
                begin = getenv(begin+1); /* begin+1, to skip past '$' */
                if (begin == NULL) {
                        perror("getenv");
                        begin = "UNDEFINED";
                }
        }
        return begin; /* This word is now a null-terminated string.  return it. */
}

/* In C, "int" is used instead of "bool", and "0" means false, any
* non-zero number (traditionally "1") means true.
*/
/* argc is _count_ of args (*argcp == argc); argv is array of arg _values_*/
static void getargs(char cmd[], int *argcp, char *argv[])
{
        char *cmdp = cmd;
        char *end;
        int i = 0;

        /* fgets creates null-terminated string. stdin is pre-defined C constant
        *   for standard intput.  feof(stdin) tests for file:end-of-file.
        */
        if (fgets(cmd, MAXLINE, stdin) == NULL && feof(stdin)) {
                printf("Couldn't read from standard input. End of file? Exiting ...\n");
                exit(1);  /* any non-zero value for exit means failure. */
        }
        while ( (cmdp = getword(cmdp, &end)) != NULL ) { /* end is output param */

                if(*cmdp == '#') {

                        break;

                } else if (*cmdp == '>') {

                        z = 1;

                } else if (*cmdp == '<') {

                        z = 2;

                } else if (*cmdp == '|') {

                        z = 3;

                } else if (*cmdp == '&') {

                        z = 4;

                }

                /* getword converts word into null-terminated string */
                argv[i++] = cmdp;
                /* "end" brings us only to the '\0' at end of string */
                cmdp = end + 1;
        }
        argv[i] = NULL; /* Create additional null word at end for safety. */
        *argcp = i;
}

static void execute(int argc, char *argv[])
{
        pid_t childpid; /* child process ID */
        int pfd[2];
        char *argvChildpid[2];
        int i = 0;

        if (pipe(pfd) == -1) {perror("pipe"); exit(1);}

        // Some of these will only work with very simple inputs

        if ( z == 1 ) {
                if (!fork()) {
                        int fd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                        if (fd == -1) perror("open for writing");
                        close(STDOUT_FILENO);
                        dup2(fd, STDOUT_FILENO);
                        argvChildpid[0] = argv[0];
                        argvChildpid[1] = NULL;
                        execvp(argv[0], argvChildpid);
                } else {

                        wait(NULL);

                }

        z = 0;

        }

        else if ( z == 2 ) {
                if (!fork()) {
                        int fd = open(argv[2], O_RDONLY);
                        if (fd == -1) perror("open for reading");
                        close(STDIN_FILENO);
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                        argvChildpid[0] = argv[0];
                        argvChildpid[1] = NULL;
                        execvp(argv[0], argvChildpid);
                } else {

                        wait(NULL);

                }

        z = 0;
        } else if ( z == 3) {

        pipeline(argv[0], argv[2]);
        z = 0;

        } else if (z == 4) {

                if (!fork()) {
                                argvChildpid[0] = argv[0];
                                argvChildpid[1] = NULL;
                                execvp(argv[0], argvChildpid);
                        }

        z = 0;
		
        } else {
                childpid = fork();
                if (childpid == -1) { /* in parent (returned error) */
                        perror("fork"); /* perror => print error string of last system call */
                        printf("  (failed to execute command)\n");
                }

                if (childpid == 0) {

                        if(z == 0) {
                        if (-1 == execvp(argv[0], argv)) {
                                perror("execvp");
                                printf("  (couldn't find command)\n");
                        }
                        /* NOT REACHED unless error occurred */
                        }
                        exit(1);

                } else {

                        /* parent:  in parent, childpid was set to pid of child process */
                        waitpid(childpid, NULL, 0);  /* wait until child process finishes */
                        z == 0;
                }

        }

        return;
}

void interrupt_handler(int signum);

int main(int argc, char *argv[])
{
        char cmd[MAXLINE];
        char *childargv[MAXARGS];
        int childargc;

        if (argc > 1)
                freopen(argv[1], "r", stdin);

        while (1) {
                signal(SIGINT, interrupt_handler);
                printf("%% "); /* printf uses %d, %s, %x, etc.  See 'man 3 printf' */
                fflush(stdout); /* flush from output buffer to terminal itself */
                getargs(cmd, &childargc, childargv); /* childargc and childargv are
                                                                                         output args; on input they have garbage, but getargs sets them. */
                /* Check first for built-in commands. */
                if ( childargc > 0 && strcmp(childargv[0], "exit") == 0 )
                        exit(0);
                else if ( childargc > 0 && strcmp(childargv[0], "logout") == 0 )
                        exit(0);
                else
                        execute(childargc, childargv);
        }
        /* NOT REACHED */
}

void interrupt_handler(int signum) {
        printf("\t The process was interrupted");
}

/* Just in case these aren't already defined. */
#define STDIN 0
#define STDOUT 1

int pipeline(char* s1, char* s2) {
    int pipe_fd[2];       /* 'man pipe' says its arg is this type */
    int fd;               /* 'man dup' says its arg is this type */
    pid_t child1, child2; /* 'man fork' says it returns type 'pid_t' */
    char * argvChild[2];

    fflush(stdout);  /* Force printing to complete, before continuing. */

    if ( -1 == pipe(pipe_fd) ) perror("pipe");
    child1 = fork();
    /* child1 > 0 implies that we're still the parent. */
    if (child1 > 0) child2 = fork();
    if (child1 == 0) { /* if we are child1, do:  "s1 | ..." */
        if ( -1 == close(STDOUT) ) perror("close");  /* close  */
        fd = dup(pipe_fd[1]); /* set up empty STDOUT to be pipe_fd[1] */
        if ( -1 == fd ) perror("dup");
        if ( fd != STDOUT ) fprintf(stderr, "Pipe output not at STDOUT.\n");
        close(pipe_fd[0]); /* never used by child1 */
        close(pipe_fd[1]); /* not needed any more */
        argvChild[0] = s1;
                argvChild[1] = NULL;
        if ( -1 == execvp(argvChild[0], argvChild) ) perror("execvp");
    } else if (child2 == 0) { /* if we are child2, do:  "... | s2" */
        if ( -1 == close(STDIN) ) perror("close");  /* close  */
        fd = dup(pipe_fd[0]); /* set up empty STDIN to be pipe_fd[0] */
        if ( -1 == fd ) perror("dup");
        if ( fd != STDIN ) fprintf(stderr, "Pipe input not at STDIN.\n");
        close(pipe_fd[0]); /* not needed any more */
        close(pipe_fd[1]); /* never used by child2 */
        argvChild[0] = s2;
                argvChild[1] = NULL;
        if ( -1 == execvp(argvChild[0], argvChild) ) perror("execvp");
    } else { /* else we're parent */
        int status;
        /* Close parent copy of pipes;
         * In particular, if pipe_fd[1] not closed, child2 will hang
         *   forever waiting since parent could also write to pipe_fd[1]
         */
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        if ( -1 == waitpid(child1, &status, 0) ) perror("waitpid");
        /* Optionally, check return status.  This is what main() returns. */
        if (WIFEXITED(status) == 0)
            printf("child1 returned w/ error code %d\n", WEXITSTATUS(status));
        if ( -1 == waitpid(child2, &status, 0) ) perror("waitpid");
        /* Optionally, check return status.  This is what main() returns. */
        if (WIFEXITED(status) == 0)
            printf("child2 returned w/ error code %d\n", WEXITSTATUS(status));
    }
    return 0;  /* returning 0 from main() means success. */
}
