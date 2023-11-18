#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

extern int errno;

//function prototypes
void createFile(char* name);
void writeFile(char* filename, char* content);
void readFile(char* filename);
void deleteFile(char* filename);
void appendFile(char *filename,char *str);


int main(){
    // Tutorial on how to use the File Manager
    printf ("Welcome to the File Manager Terminal.\n");
    printf ("Here are the commands available and how to use them:\n\n");

    printf ("\t(*) create: Create a file with your given name\n");
    printf ("\t    For example if you want to create the file hello.txt then use\n");
    printf ("\t    \"create hello.txt\"\n\n");

    printf ("\t(*) write: Write data to a file.\n");
    printf ("\t    For example if you want to write to hello.txt then use\n");
    printf ("\t    \"write hello.txt\n");
    printf ("\t    Hello World\"\n");
    printf ("\t    It is important you put a newline after the filename, before entering the content!\n\n");

    printf ("\t(*) read: Read the contents of a file.\n");
    printf ("\t    For example if you want to read hello.txt then use\n");
    printf ("\t    \"read hello.txt\"\n\n");

    printf ("\t(*) append: Append data to the contents of a file.\n");
    printf ("\t    For example if you want to append to hello.txt then use\n");
    printf ("\t    \"append hello.txt\n");
    printf ("\t    Hello Again\"\n");
    printf ("\t    It is important you put a newline after the filename, before entering the content!\n\n");

    printf ("\t(*) delete: Delete a file.\n");
    printf ("\t    For example if you want to delete hello.txt then use\n");
    printf ("\t    \"delete hello.txt\"\n\n");

    printf ("\t(*) quit: to quit the file manager.\n");
    printf ("\t    \"quit\"\n");

    char filename[30];
    char content[1000];
    char command[10];
    char temp[1000];

    while (true){
        printf ("\nEnter your command: ");
        scanf("%s", command);

        if (strcmp(command, "create") == 0){
            fgets(temp, sizeof(temp), stdin);
            strncpy(filename, temp+1, strlen(temp)-2);
            filename[strlen(temp)-2] = '\0';

            createFile(filename);
        }

        else if (strcmp(command, "write") == 0){
            fgets(temp, sizeof(temp), stdin);
            strncpy(filename, temp+1, strlen(temp)-2);
            filename[strlen(temp)-2] = '\0';

            fgets(content, sizeof(content), stdin);
            content[strlen(content)-1] = '\0';

            writeFile(filename, content);
        }

        else if (strcmp(command, "read") == 0){
            fgets(temp, sizeof(temp), stdin);
            strncpy(filename, temp+1, strlen(temp)-2);
            filename[strlen(temp)-2] = '\0';

            readFile(filename);
        }

        else if (strcmp(command, "append") == 0){
            fgets(temp, sizeof(temp), stdin);
            strncpy(filename, temp+1, strlen(temp)-2);
            filename[strlen(temp)-2] = '\0';

            fgets(content, sizeof(content), stdin);
            content[strlen(content)-1] = '\0';

            appendFile(filename, content);
        }

        else if (strcmp(command, "delete") == 0){
            fgets(temp, sizeof(temp), stdin);
            strncpy(filename, temp+1, strlen(temp)-2);
            filename[strlen(temp)-2] = '\0';

            deleteFile(filename);
        }

        else if (strcmp(command, "quit") == 0){
            printf("Thank you for using this program.\n\n");
            break;
        }

        else {
            printf("Please write the commands correctly.\n");
        }
    }

    return 0;
}



// Creates a new file with the specified filename.
void createFile(char* filename){
    // The function creat() attempts to create a new file and returns a file descriptor.
    int fileDescriptor = creat(filename, 0644);
    printf("(fileDescriptor = %d) ", fileDescriptor);
    // If creat() returns -1, an error occurred while creating the file.
    if (fileDescriptor == -1) {
        // print the type of error
        printf("Error Number %d\n", errno);
        perror("Program");
    } else {
        // If the file was created successfully, display a success message.
        printf("%s created successfully.\n", filename);
    }
}

// Writes content to a file with the specified filename.
void writeFile(char* filename,char* content){
    // Attempt to open the file in write-only mode, creating it if it doesn't exist, or truncating it if it does.
    int fileDescriptor = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // If opening the file fails (returns -1), handle the error.
    if (fileDescriptor == -1) {
        // print the type of error
        printf("Error Number %d\n", errno);
        perror("Program");
    }
    else{
        size_t contentSize = strlen(content);
        size_t totalBytesWritten = 0;
        // Write the content to the file.
        while (totalBytesWritten < contentSize) {
            int bytesWritten = write(fileDescriptor, content + totalBytesWritten, contentSize - totalBytesWritten);
            // Check for errors or if no bytes were written.
            if (bytesWritten <= 0) {
                // Error handling for writing data
                printf("Write Operation Failed!!! -> ");
                printf("Error Number: %d\n", errno);
                perror("Write");
                break;
            }
            totalBytesWritten += bytesWritten;
        }
        // Close the file descriptor after writing is complete or in case of an error.
        close(fileDescriptor);
    }
}

// Reads the contents of a file with the specified filename and prints it to the console.
void readFile(char* filename){
    // Buffer to hold the read content from the file.
    char buffer[1000];
    // Attempt to open the file in read-only mode.
    int fileDescriptor = open(filename, O_RDONLY);
    // If opening the file fails (returns -1), handle the error.
    if (fileDescriptor == -1) {
        // print the type of error
        printf("Error Number %d\n", errno);
        perror("Program");
    }
    else{
        ssize_t bytesRead;
        // Read data from the file into the buffer and print it to the console.
        while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer)-1)) > 0){
            buffer[bytesRead] = '\0';
            printf("%s", buffer);
        }
        // Handle errors that may occur during the read operation.
        if (bytesRead == -1){
            printf("Read operation failed!!! -> ");
            printf("Error Number %d\n", errno);
            perror("Read");
        }
        else
            printf("\n");
        // Close the file descriptor after reading or in case of an error.
        close(fileDescriptor);
    }
}

// Deletes a file with the specified filename.
void deleteFile(char* filename){
    // Attempt to delete the file using the unlink() function.
    int stat = unlink(filename);
    // Check if the deletion was successful or not.
    if(stat==-1){
        printf("Error Number %d\n", errno);
        perror("Error deleting file");
    }else{
        // If the deletion was successful, display a success message.
        printf("File deleted successfully.\n");
    }
}

// Appends content to a file with the specified filename.
void appendFile(char *filename, char *content) {
    // Attempt to open the file in write-only mode with the append flag.
    int fileDescriptor = open(filename, O_WRONLY | O_APPEND, 0644);
    // If opening the file fails (returns -1), handle the error.
    if (fileDescriptor == -1) {
        // print the type of error
        printf("Error Number %d\n", errno);
        perror("Program");
    }
    else{
        // Write the content to the end of the file.
        ssize_t bytes_written = write(fileDescriptor, content, strlen(content));
        // Handle errors that may occur during the append operation.
        if (bytes_written == -1) {
            printf("Append operation failed!!! -> ");
            printf("Error Number %d\n", errno);
            perror("Append");
        }
        // Close the file descriptor after appending or in case of an error.
        close(fileDescriptor);
    }
}
