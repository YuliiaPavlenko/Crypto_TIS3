#pragma once
#include <windows.h>
#include <string>
#include "Crypt.h"

#define type3DES_EEE3 1
#define type3DES_EEE2 2
#define type3DES_EDE3 3
#define typeDES 4


class UserInterface
{
public:
	TCHAR sourceFileName[MAX_PATH];
	TCHAR resultFileName[MAX_PATH];
	unsigned char cryptType;
	bool cryptMode;
	bool isEncrypted;

	UserInterface(void);
	void showMenu();
	void fillSourceFilenameStruct(LPOPENFILENAME OFN);
	void fillResultFilenameStruct(LPOPENFILENAME OFN);
	bool getOpenFileNames(LPOPENFILENAME sourceFile, LPOPENFILENAME resultFile);
	uint64 readKey();
	void cryptTypeSelection();
	void cryptModeSelection();	
	void cryptDecryptSelection(Crypt<uint64>* cryptObj);
};

