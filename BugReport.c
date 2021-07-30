#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX 4;

void pushDirectory();
void popDirectory();
char getCurrentWorkingDirectory();
bool checkIfEmpty();
bool checkIfTheFileIsEmpty();
void enumerateDirectoryStack();

char directoryStack[4][300];
int top = -1;
int subFFCount = 0; //sub files/folders count
char currentWDString[300];

int main(){
    char statusText[8][7];
    char rootDirectory[90];
    int serialNo = 0;

    strcpy(statusText[0], "PASS");
    strcpy(statusText[1], "PASSED");
    strcpy(statusText[2], "pass");
    strcpy(statusText[3], "passed");
    strcpy(statusText[4], "FAIL");
    strcpy(statusText[5], "FAILED");
    strcpy(statusText[6], "fail");
    strcpy(statusText[7], "failed");
    strcpy(rootDirectory, "./TEST LOGS");

    DIR *currentWorkingDirectory_ts, *currentWorkingDirectory_m, *currentWorkingDirectory_id;
    FILE *csvFile, *logFile;
    struct dirent *timestampDirPointer, *moduleDirPointer, *testIDDirPointer;
    remove("outputReport.csv");
    if ((csvFile = fopen("outputReport.csv", "a+")) != NULL){
        fprintf(csvFile, "SNO,TIMESTAMP,MODULE NAME,TEST ID,RESULT\n");
    }
    fclose(csvFile);
    pushDirectory(rootDirectory);
    currentWorkingDirectory_ts = opendir(directoryStack[top]);    
    if (currentWorkingDirectory_ts == NULL){
        printf("------ERROR_00------");
        return (-1);
    }
    if (checkIfEmpty() == true){
        printf("No records Found!");
    }
    else{
        while ((timestampDirPointer = readdir(currentWorkingDirectory_ts))){ 
            if (strcmp(".", timestampDirPointer->d_name) == 0 || strcmp("..", timestampDirPointer->d_name) == 0){
                continue;
            }else{
                strcpy(currentWDString, directoryStack[top]);
                strcat(currentWDString, "/");
                strcat(currentWDString, timestampDirPointer->d_name);                
                pushDirectory(currentWDString);                
                currentWorkingDirectory_m = opendir(directoryStack[top]);
                
                if (currentWorkingDirectory_m == NULL){
                    printf("---ERROR_01---");
                    return (-1);
                }
                if (checkIfEmpty() == true){
                    printf("No Records found");
                }else{                    
                    while ((moduleDirPointer = readdir(currentWorkingDirectory_m))){
                        if (strcmp(".", moduleDirPointer->d_name) == 0 || strcmp("..", moduleDirPointer->d_name) == 0){
                            continue;
                        }else{                            
                            strcpy(currentWDString, directoryStack[top]);
                            strcat(currentWDString, "/");
                            strcat(currentWDString, moduleDirPointer->d_name);
                            pushDirectory(currentWDString);                            
                            currentWorkingDirectory_id = opendir(directoryStack[top]);                            
                            if (currentWorkingDirectory_id == NULL){
                                printf("---ERROR_02---");
                                return (-1);
                            }
                            if (checkIfEmpty() == true){
                                printf("No Records Found");
                            }else{
                                while ((testIDDirPointer = readdir(currentWorkingDirectory_id))){
                                    if (strcmp(".", testIDDirPointer->d_name) == 0 || strcmp("..", testIDDirPointer->d_name) == 0){
                                        continue;
                                    }else{                                        
                                        strcpy(currentWDString, directoryStack[top]);
                                        strcat(currentWDString, "/");
                                        strcat(currentWDString, testIDDirPointer->d_name);                                        
                                        pushDirectory(currentWDString);
                                        char buffer[1000];
                                        auto char status[20];
                                        if ((logFile = fopen(directoryStack[top], "r")) != NULL){
                                            while (fgets(buffer, 1000, logFile) != NULL){
                                                if (strstr(buffer, statusText[0]) ||
                                                    strstr(buffer, statusText[1]) ||
                                                    strstr(buffer, statusText[2]) ||
                                                    strstr(buffer, statusText[3])){
                                                    strcpy(status, statusText[0]);
                                                }else if (strstr(buffer, statusText[4]) ||
                                                         strstr(buffer, statusText[5]) ||
                                                         strstr(buffer, statusText[6]) ||
                                                         strstr(buffer, statusText[7])){
                                                    strcpy(status, statusText[4]);
                                                }                                           
                                            }
                                            fclose(logFile);                                            
                                            serialNo += 1;
                                            if ((csvFile = fopen("outputReport.csv", "a+")) != NULL){
                                                fprintf(csvFile, "%d,%s,%s", serialNo, timestampDirPointer->d_name, moduleDirPointer->d_name);
                                                for (auto int i = 0; testIDDirPointer->d_name[i] != '.'; i++){
                                                    if (i == 0){
                                                        fprintf(csvFile, ",");
                                                    }
                                                    fprintf(csvFile, "%c", testIDDirPointer->d_name[i]);
                                                }
                                                fprintf(csvFile, ",%s\n", status);
                                                strcpy(status, "");
                                                fclose(csvFile);
                                            }
                                        }                                                                         
                                    }
                                    popDirectory();
                                }                                
                            }
                        }
                        popDirectory();
                    } 
                }
            }
            popDirectory();
        } 
    }
    return (0);
}

void enumerateDirectoryStack(){
    for (auto int i = top; i >= 0; i--){
        printf("%s\n", directoryStack[i]);
    }
}
void pushDirectory(char temp[300]){
    top += 1;
    strcpy(directoryStack[top], temp);
}

void popDirectory(){
    auto int i;
    for (i = 0; i < strlen(directoryStack[top]); i++){
        directoryStack[top][i] = '\0';
    }
    top -= 1;
}
bool checkIfEmpty(){
    DIR *checkDIR;
    auto int fileCount = 0;
    struct dirent *dirObj;
    checkDIR = opendir(directoryStack[top]);
    if (checkDIR == NULL){
        return (-1); //error opening directory
    }else{
        while ((dirObj = readdir(checkDIR))){
            fileCount++;
        }
        if (fileCount > 2){
            subFFCount = fileCount;
            return false;
        }else{
            return true;
        }
    }
}
