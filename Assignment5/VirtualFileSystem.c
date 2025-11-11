#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME_LEN 50
#define MAX_FILE_BLOCKS 32
#define MAX_CMD_LEN 256
#define MAX_DATA_LEN (MAX_FILE_BLOCKS * BLOCK_SIZE)

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

typedef struct FreeBlock {
    int blockIndex;
    struct FreeBlock* next;
    struct FreeBlock* prev;
} FreeBlock;

typedef struct FileNode {
    char name[MAX_NAME_LEN + 1];
    int isDirectory;
    long contentSize;

    int blockPointers[MAX_FILE_BLOCKS];
    int blocksUsed;

    
    struct FileNode* children;
    struct FileNode* nextChild;
    struct FileNode* parent;
} FileNode;

FreeBlock* freeListHead = NULL;
FreeBlock* freeListTail = NULL;

FileNode* root = NULL;
FileNode* cwd = NULL;

int globalTotalBlocks = NUM_BLOCKS;
int globalUsedBlocks = 0;

void addFreeBlock(int blockIndex) {
    FreeBlock* newNode = (FreeBlock*)malloc(sizeof(FreeBlock));
    if (!newNode) {
        printf("Out of memory.\n");
        exit(1);
    }
    newNode->blockIndex = blockIndex;
    newNode->next = NULL;
    newNode->prev = freeListTail;

    if (freeListTail) {
        freeListTail->next = newNode;
    }
    freeListTail = newNode;

    if (!freeListHead) {
        freeListHead = newNode;
    }
}

int allocateBlock() {
    if (!freeListHead) {
        return -1;
    }

    int blockIndex = freeListHead->blockIndex;
    FreeBlock* oldHead = freeListHead;

    freeListHead = freeListHead->next;
    if (freeListHead) {
        freeListHead->prev = NULL;
    } else {
        freeListTail = NULL;
    }

    free(oldHead);
    globalUsedBlocks++;
    return blockIndex;
}

void freeBlock(int blockIndex) {
    addFreeBlock(blockIndex);
    globalUsedBlocks--;
    memset(virtualDisk[blockIndex], 0, BLOCK_SIZE);
}

void initVfs() {
    for (int blockIndex = 0; blockIndex < NUM_BLOCKS; blockIndex++) {
        addFreeBlock(blockIndex);
    }
    globalUsedBlocks = 0;
    root = (FileNode*)malloc(sizeof(FileNode));
    if (!root) {
        printf("Out of memory.\n");
        exit(1);
    }
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->contentSize = 0;
    root->blocksUsed = 0;
    root->children = NULL;
    root->nextChild = NULL;
    root->parent = NULL;
    cwd = root;
}

FileNode* findChild(FileNode* parent, const char* name) {
    if (!parent || !parent->isDirectory || !parent->children) {
        return NULL;
    }

    FileNode* current = parent->children;
    do {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->nextChild;
    } while (current != parent->children);

    return NULL;
}

void addChild(FileNode* parent, FileNode* child) {
    child->parent = parent;
    if (!parent->children) {
        parent->children = child;
        child->nextChild = child;
    } else {
        FileNode* tail = parent->children;
        while (tail->nextChild != parent->children) {
            tail = tail->nextChild;
        }
        tail->nextChild = child;
        child->nextChild = parent->children;
    }
}

int removeChild(FileNode* parent, const char* name) {
    if (!parent || !parent->isDirectory || !parent->children) {
        return 0;
    }

    FileNode* nodeToDelete = NULL;
    FileNode* prevNode = NULL;
    if (strcmp(parent->children->name, name) == 0) {
        nodeToDelete = parent->children;
        if (nodeToDelete->nextChild == nodeToDelete) {
            parent->children = NULL;
        } else {
            FileNode* tail = nodeToDelete;
            while (tail->nextChild != nodeToDelete) {
                tail = tail->nextChild;
            }
            parent->children = nodeToDelete->nextChild;
            tail->nextChild = parent->children;
        }
    } else {
        prevNode = parent->children;
        nodeToDelete = prevNode->nextChild;

        while (nodeToDelete != parent->children) {
            if (strcmp(nodeToDelete->name, name) == 0) {
                prevNode->nextChild = nodeToDelete->nextChild;
                break;
            }
            prevNode = nodeToDelete;
            nodeToDelete = nodeToDelete->nextChild;
        }
        if (nodeToDelete == parent->children) return 0;
    }

    free(nodeToDelete);
    return 1;
}

void freeTree(FileNode* node) {
    if (!node) return;

    if (node->isDirectory && node->children) {
        FileNode* head = node->children;
        FileNode* tail = head;
        while (tail->nextChild != head) {
            tail = tail->nextChild;
        }
        tail->nextChild = NULL;

        FileNode* current = head;
        while (current) {
            FileNode* next = current->nextChild;
            freeTree(current);
            current = next;
        }
    }
    free(node);
}

void getPromptPath(char* promptString) {
    if (cwd == root) {
        strcpy(promptString, "/ > ");
        return;
    }

    char path[1024] = "";
    FileNode* temp = cwd;
    char tempPath[1024] = "";

    while (temp != root) {
        strcpy(tempPath, "/");
        strcat(tempPath, temp->name);
        strcat(tempPath, path);

        strcpy(path, tempPath);
        temp = temp->parent;
    }
    
    strcpy(promptString, path);
    strcat(promptString, " > ");
}


void getWriteCommand(char* input, char** commandName, char** fileName, char** dataContent) {
    *commandName = strtok(input, " \n");
    if (!*commandName) return;

    *fileName = strtok(NULL, " \n");
    if (!*fileName) return;

    char* searchStart = *fileName + strlen(*fileName) + 1;

    char* dataStart = strchr(searchStart, '\'');
    if (!dataStart) {
        dataStart = strchr(searchStart, '"');
    }

    if (dataStart) {
        char quoteChar = *dataStart;
        dataStart++;
        
        char* dataEnd = strrchr(dataStart, quoteChar);
        if (dataEnd) {
            *dataEnd = '\0';
            *dataContent = dataStart;
        } else {
            *dataContent = NULL;
        }
    } else {
        *dataContent = NULL;
    }
}

void vfsLs() {
    if (!cwd->children) {
        printf("(empty)\n");
        return;
    }

    FileNode* current = cwd->children;
    do {
        printf("%s", current->name);
        if (current->isDirectory) {
            printf("/");
        }
        printf("\n");
        current = current->nextChild;
    } while (current != cwd->children);
}

void vfsMkdir(const char* name) {

    if (strlen(name) > MAX_NAME_LEN) {
        printf(" Name is too long (max %d chars).\n", MAX_NAME_LEN);
        return;
    }
    if (findChild(cwd, name)) {
        printf("Name already exists in current directory.\n");
        return;
    }

    FileNode* newDir = (FileNode*)malloc(sizeof(FileNode));
    strcpy(newDir->name, name);
    newDir->isDirectory = 1;
    newDir->contentSize = 0;
    newDir->blocksUsed = 0;
    newDir->children = NULL;

    addChild(cwd, newDir);
    printf("Directory '%s' created successfully.\n", name);
}

void vfsCreate(const char* name) {

    if (strlen(name) > MAX_NAME_LEN) {
        printf("Name is too long (max %d chars).\n", MAX_NAME_LEN);
        return;
    }
    if (findChild(cwd, name)) {
        printf("Name already exists in current directory.\n");
        return;
    }
    FileNode* newFile = (FileNode*)malloc(sizeof(FileNode));
    strcpy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->contentSize = 0;
    newFile->blocksUsed = 0;

    addChild(cwd, newFile);
    printf("File '%s' created successfully.\n", name);
}

void vfsCd(const char* name) {
    char path[1024] = "";

    if (strcmp(name, "..") == 0) {
        if (cwd->parent) {
            cwd = cwd->parent;
        }
    } else {
        FileNode* target = findChild(cwd, name);
        if (target && target->isDirectory) {
            cwd = target;
        } else {
            printf("Directory not found.\n");
            return;
        }
    }

    if (cwd == root) {
        strcpy(path, "/");
    } else {
        FileNode* temp = cwd;
        char tempPath[1024] = "";
        while (temp != root) {
            strcpy(tempPath, "/");
            strcat(tempPath, temp->name);
            strcat(tempPath, path);
            
            strcpy(path, tempPath);
            temp = temp->parent;
        }
    }
    printf("Moved to %s\n", path);
}

void vfsPwd() {
    if (cwd == root) {
        printf("/\n");
        return;
    }
    
    char path[1024] = "";
    FileNode* temp = cwd;
    char tempPath[1024] = "";

    while (temp != root) {
        strcpy(tempPath, "/");
        strcat(tempPath, temp->name);
        strcat(tempPath, path);
        
        strcpy(path, tempPath);
        temp = temp->parent;
    }
    printf("%s\n", path);
}

void vfsWrite(const char* name, const char* data) {
    FileNode* file = findChild(cwd, name);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }
    
    for (int index = 0; index < file->blocksUsed; index++) {
        freeBlock(file->blockPointers[index]);
    }
    file->blocksUsed = 0;
    file->contentSize = 0;
    
    long dataLen = strlen(data);
    if (dataLen == 0) {
        printf("Data written successfully (size=0 bytes).\n");
        return;
    }
    if (dataLen > MAX_DATA_LEN) {
        printf("Error: Data is too large for this file.\n");
        return;
    }

    int blocksNeeded = (int)ceil((double)dataLen / BLOCK_SIZE);
    
    if (blocksNeeded > (globalTotalBlocks - globalUsedBlocks)) {
        printf("Not enough free blocks on disk.\n");
        return;
    }
    if (blocksNeeded > MAX_FILE_BLOCKS) {
        printf("Data is too large for a single file.\n");
        return;
    }

    for (int index = 0; index < blocksNeeded; index++) {
        int blockIndex = allocateBlock();
        if (blockIndex == -1) {
            printf("Disk full during write.\n");
            for (int cleanupIndex = 0; cleanupIndex < index; cleanupIndex++) {
                freeBlock(file->blockPointers[cleanupIndex]);
            }
            file->blocksUsed = 0;
            return;
        }

        file->blockPointers[index] = blockIndex;
        
        int offset = index * BLOCK_SIZE;
        int bytesToWrite = (dataLen - offset > BLOCK_SIZE) ? BLOCK_SIZE : (dataLen - offset);
        
        memcpy(virtualDisk[blockIndex], data + offset, bytesToWrite);
    }

    file->blocksUsed = blocksNeeded;
    file->contentSize = dataLen;
    printf("Data written successfully (size=%ld bytes).\n", dataLen);
}

void vfsRead(const char* name) {
    FileNode* file = findChild(cwd, name);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }
    if (file->contentSize == 0) {
        printf("(empty)\n");
        return;
    }

    long bytesRead = 0;
    for (int index = 0; index < file->blocksUsed; index++) {
        int blockIndex = file->blockPointers[index];
        
        int bytesToRead = (file->contentSize - bytesRead > BLOCK_SIZE) ? 
                          BLOCK_SIZE : (file->contentSize - bytesRead);

        fwrite(virtualDisk[blockIndex], 1, bytesToRead, stdout);
        
        bytesRead += bytesToRead;
        if (bytesRead >= file->contentSize) {
            break;
        }
    }
    printf("\n");
}

void vfsDelete(const char* name) {
    FileNode* file = findChild(cwd, name);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }
    for (int index = 0; index < file->blocksUsed; index++) {
        freeBlock(file->blockPointers[index]);
    }
    if (removeChild(cwd, name)) {
        printf("File deleted successfully.\n");
    } else {
        printf("Error: Could not delete file node.\n");
    }
}

void vfsRmdir(const char* name) {
    FileNode* dir = findChild(cwd, name);
    if (!dir || !dir->isDirectory) {
        printf("Directory not found.\n");
        return;
    }
    if (dir->children) {
        printf("Directory not empty. Remove files first.\n");
        return;
    }
    
    if (removeChild(cwd, name)) {
        printf("Directory removed successfully.\n");
    } else {
        printf("Could not remove directory node.\n");
    }
}

void vfsDf() {
    int freeBlocks = globalTotalBlocks - globalUsedBlocks;
    double usage = (globalUsedBlocks == 0) ? 0.0 : ((double)globalUsedBlocks / globalTotalBlocks) * 100.0;
    printf("Total Blocks: %d\n", globalTotalBlocks);
    printf("Used Blocks:  %d\n", globalUsedBlocks);
    printf("Free Blocks:  %d\n", freeBlocks);
    printf("Disk Usage:   %.2f%%\n", usage);
}

void vfsExit() {
    freeTree(root);
    FreeBlock* current = freeListHead;
    while (current) {
        FreeBlock* next = current->next;
        free(current);
        current = next;
    }
    
    printf("Memory released. Exiting program...\n");
    exit(0);
}

int main() {
    initVfs();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char inputBuffer[MAX_CMD_LEN];
    char promptString[MAX_CMD_LEN];
    char writeBufferCopy[MAX_CMD_LEN];

    while (1) {
        getPromptPath(promptString);
        printf("%s", promptString);

        if (!fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
            break;
        }
        char commandName[MAX_NAME_LEN + 1];
        char targetName[MAX_NAME_LEN + 1];
        int inputCount = sscanf(inputBuffer, "%s", commandName);

        if (inputCount < 1) {
            continue;
        }
        if (strcmp(commandName, "exit") == 0) {
            vfsExit();
        } else if (strcmp(commandName, "ls") == 0) {
            vfsLs();
        } else if (strcmp(commandName, "df") == 0) {
            vfsDf();
        } else if (strcmp(commandName, "pwd") == 0) {
            vfsPwd();
        } else if (strcmp(commandName, "mkdir") == 0) {
            inputCount = sscanf(inputBuffer, "%s %s", commandName, targetName);
            if (inputCount == 2) vfsMkdir(targetName);
            else printf("Usage: mkdir <name>\n");
        } else if (strcmp(commandName, "create") == 0) {
            inputCount = sscanf(inputBuffer, "%s %s", commandName, targetName);
            if (inputCount == 2) vfsCreate(targetName);
            else printf("Usage: create <name>\n");
        } else if (strcmp(commandName, "cd") == 0) {
            inputCount = sscanf(inputBuffer, "%s %s", commandName, targetName);
            if (inputCount == 2) vfsCd(targetName);
            else printf("Usage: cd <name>\n");
        } else if (strcmp(commandName, "rmdir") == 0) {
            inputCount = sscanf(inputBuffer, "%s %s", commandName, targetName);
            if (inputCount == 2) vfsRmdir(targetName);
            else printf("Usage: rmdir <name>\n");
        } else if (strcmp(commandName, "delete") == 0) {
            inputCount = sscanf(inputBuffer, "%s %s", commandName, targetName);
            if (inputCount == 2) vfsDelete(targetName);
            else printf("Usage: delete <name>\n");
        } else if (strcmp(commandName, "read") == 0) {
            inputCount = sscanf(inputBuffer, "%s %s", commandName, targetName);
            if (inputCount == 2) vfsRead(targetName);
            else printf("Usage: read <name>\n");
        } else if (strcmp(commandName, "write") == 0) {
            char *cmd, *fileName, *dataContent = NULL;
            strcpy(writeBufferCopy, inputBuffer);
            
            getWriteCommand(writeBufferCopy, &cmd, &fileName, &dataContent);
            
            if (fileName && dataContent) {
                vfsWrite(fileName, dataContent);
            } else {
                printf("Usage: write <name> \"<data>\"\n");
            }
        } else {
            printf("Unknown command: '%s'\n", commandName);
        }
    }

    return 0;
}