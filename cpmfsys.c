#define _CMPFSYS_C

#include <ctype.h>

#include "cpmfsys.h"

#define PRINT_ROW_SIZE 16


/*
 * COMP 7500/001
 * Matthew Tyler McGlawn
 * Auburn University
 *
 * COMPILE
 *
 * `make`
 *
 *
 * TESTS
 *
 * No Tests
 *
 *
 * USE
 *
 * To run the program you need to do:
 * 1: $ ./cmpRun
 */

bool freeList[NUM_BLOCKS];

void removeWhiteSpace(char *input)
{
  int i = 0;
  for (int j=0; input[j]; j++) {
    input[j] = input[i + j];
    if ((input[j]) == ' ') {
      i++;
      j--;
    }
  }
}

//function to allocate memory for a DirStructType (see above), and populate it, given a
//pointer to a buffer of memory holding the contents of disk block 0 (e), and an integer index
// which tells which extent from block zero (extent numbers start with 0) to use to make the
// DirStructType value to return. 
DirStructType *mkDirStruct(int index,uint8_t *e) {
  DirStructType *my_struct = malloc(sizeof(DirStructType));
  uint8_t *location = (e + index * EXTENT_SIZE);
  my_struct->status = location[0];
  int i = 1;
  for (int j=0; j < 8; j++) {
    my_struct->name[j] = ' ';
    my_struct->name[j] = location[i++];
  }
  my_struct->name[8] = '\0';
  i = 9;
  for (int j=0; j < 3; j++) {
    my_struct->extension[j] = ' ';
    my_struct->extension[j] = location[i++];
  }
  my_struct->extension[3] = '\0';
  i = 16;
  my_struct->XL = location[12];
  my_struct->BC = location[13];
  my_struct->XH = location[14];
  my_struct->RC = location[15];
  for (int j=0; j < 16; j++) {
    my_struct->blocks[j] = location[i++];
  }
  return my_struct;
}

// function to write contents of a DirStructType struct back to the specified index of the extent
// in block of memory (disk block 0) pointed to by e
void writeDirStruct(DirStructType *d, uint8_t index, uint8_t *e) {
  uint8_t *location = (e + index * EXTENT_SIZE);
  location[0] = d->status;
  int i = 1;
  for (int j = 0; j < 8; j++) {
    location[i++] = d->name[j];
  }
  location[9] = ' ';
  i = 9;
  for (int j=0; j < 3; j++) {
    location[i++] = d->extension[j];
  }
  location[11] = ' ';
  i = 16;
  location[12] = d->XL;
  location[13] = d->BC;
  location[14] = d->XH;
  location[15] = d->RC;
  for (int j=0; j < 16; j++) {
    location[i++] = d->blocks[j];
  }
  blockWrite(e, 0);
  makeFreeList();
}

// populate the FreeList global data structure. freeList[i] == true means 
// that block i of the disk is free. block zero is never free, since it holds
// the directory. freeList[i] == false means the block is in use. 
void makeFreeList() {
  uint8_t *blockZero = malloc(BLOCK_SIZE);
  freeList[0] = false;
  for (int i=1; i < NUM_BLOCKS; i++) {
    freeList[i] = true;
  }
  blockRead(blockZero, 0);
  for (int i=0; i < EXTENT_SIZE; i++) {
    DirStructType *extent = mkDirStruct(i, blockZero);
    if (extent->status != 0xe5) {
      for (int j=0; j < BLOCKS_PER_EXTENT; j++) {
        if (extent->blocks[j] != 0) {
          freeList[(int)extent->blocks[j]] = false;
        }
      }
    }
  }
}

// debugging function, print out the contents of the free list in 16 rows of 16, with each 
// row prefixed by the 2-digit hex address of the first block in that row. Denote a used
// block with a *, a free block with a . 
void printFreeList() {



  fprintf(stdout, "FREE BLOCK LIST: (* means in-use)\n");
  int freeListIndex = 0;
  for (int i=0; i < NUM_BLOCKS/PRINT_ROW_SIZE; i++) {
    if (i == 0) {
      fprintf(stdout, "  %x: ", i);
    }
    else {
      fprintf(stdout, " %x0: ", i);
    }
    for (int j=0; j < PRINT_ROW_SIZE; j++) {
      if (freeList[freeListIndex++] == true) {
        fprintf(stdout, ". ");
      }
      else {
        fprintf(stdout, "* ");
      }
    }
    fprintf(stdout, "\n");
  }
}


// internal function, returns -1 for illegal name or name not found
// otherwise returns extent nunber 0-31
int findExtentWithName(char *name, uint8_t *block0) {
  if (checkLegalName(name)) {
    for (int i=0; i < EXTENT_SIZE; i++) {
      DirStructType *directory = mkDirStruct(i, block0);
      char local_name[9];
      char extension[4];
      sprintf(local_name, "%s", directory->name);
      sprintf(extension, "%s", directory->extension);
      removeWhiteSpace(local_name);
      removeWhiteSpace(extension);
      char fullName[14];
      if (strchr(name, '.') == NULL) {
        sprintf(fullName, "%s", local_name);
        if (strcmp(fullName, name) == 0) {
          return i;
        }
      }
      else {
        sprintf(fullName, "%s.%s", local_name, extension);
        if (strcmp(fullName, name) == 0) {
          return i;
        }
      }
    }
  }
  return -1;
}

// internal function, returns true for legal name (8.3 format), false for illegal
// (name or extension too long, name blank, or  illegal characters in name or extension)
bool checkLegalName(char *name) {
  int size = strlen(name);
  if (size == 0) {
    return false;
  }
  return isalnum(name[0]);
}


// print the file directory to stdout. Each filename should be printed on its own line, 
// with the file size, in base 10, following the name and extension, with one space between
// the extension and the size. If a file does not have an extension it is acceptable to print
// the dot anyway, e.g. "myfile. 234" would indicate a file whose name was myfile, with no 
// extension and a size of 234 bytes. This function returns no error codes, since it should
// never fail unless something is seriously wrong with the disk 
void cpmDir() {
  uint8_t *blockZero = malloc(BLOCK_SIZE);
  fprintf(stdout, "DIRECTORY LISTING\n");
  blockRead(blockZero, 0);
  int blockIndex;
  int fileSize;
  for (int i=0; i < EXTENT_SIZE; i++) {
    DirStructType *directory = mkDirStruct(i, blockZero);
    if (directory->status != 0xe5) {
      blockIndex = 0;
      for (int j=0; j < BLOCKS_PER_EXTENT; j++) {
        if (directory->blocks[j] != 0) {
          blockIndex++;
        }
      }
      fileSize = (blockIndex - 1) * BLOCK_SIZE + directory->RC * 128 + directory->BC;
      if (directory->RC == 0 && directory->BC == 0) {
        fprintf(stdout, "error: BC and RC are both 0\n");
      }
      else {
        char name[9];
        char extension[4];
        sprintf(name, "%s", directory->name);
        sprintf(extension, "%s", directory->extension);
        removeWhiteSpace(name);
        removeWhiteSpace(extension);
        fprintf(stdout, "%s.%s %d\n", name, extension, fileSize);
      }
    }
  }
}

// error codes for next five functions (not all errors apply to all 5 functions)  
/* 
    0 -- normal completion
   -1 -- source file not found
   -2 -- invalid  filename
   -3 -- dest filename already exists 
   -4 -- insufficient disk space 
*/ 

//read directory block, 
// modify the extent for file named oldName with newName, and write to the disk
int cpmRename(char *oldName, char * newName) {
  if (checkLegalName(newName)) {
    uint8_t *blockZero = malloc(BLOCK_SIZE);
    blockRead(blockZero, 0);
    int foundExtent = findExtentWithName(oldName, blockZero);
    DirStructType *directory  = mkDirStruct(foundExtent, blockZero);
    if (foundExtent == -1) {
      return -1;
    }
    else {
      char name[9];
      char extension[4];
			sprintf(name, "        ");
			sprintf(extension, "   ");

      int i = 0;
      for (int j=0; j < 8; j++) {
        if (newName[j] == '.') {
          i++;
          break;
        }
        else {
          name[j] = newName[j];
          i++;
        }
      }
      if (strchr(oldName, '.') == NULL) {
        i++;
      }
      for (int j=0; j < 3; j++) {
        if (isalnum(newName[i])) {
          extension[j] = newName[i];
        }
        else {
          extension[j] = ' ';
        }
        i++;
      }
      sprintf(directory->name, "%s", name);
      sprintf(directory->extension, "%s", extension);
      writeDirStruct(directory, foundExtent, blockZero);
      return 0;
    }
  }
  else {
    return -2;
  }
}

// delete the file named name, and free its disk blocks in the free list 
int  cpmDelete(char * name) {
  uint8_t *blockZero = malloc(BLOCK_SIZE);
  blockRead(blockZero, 0);
  int delete = findExtentWithName(name, blockZero);
  if (delete == -1) {
    return -1;
  }
  else {
    DirStructType *directory = mkDirStruct(delete, blockZero);
    directory->status = 0xe5;
    writeDirStruct(directory, delete, blockZero);
    return 0;
  }
}

// following functions need not be implemented for Lab 2 

int  cpmCopy(char *oldName, char *newName); 


int  cpmOpen( char *fileName, char mode); 

// non-zero return indicates filePointer did not point to open file 
int cpmClose(int filePointer); 

// returns number of bytes read, 0 = error 
int cpmRead(int pointer, uint8_t *buffer, int size);

// returns number of bytes written, 0 = error 
int cpmWrite(int pointer, uint8_t *buffer, int size);  

