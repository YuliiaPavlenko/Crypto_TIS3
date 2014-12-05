#include "stdafx.h"
#include "UserInterface.h"
#include <iostream>
#include "DES3.h"
#include <Windows.h>

UserInterface::UserInterface(void)
{
}

void UserInterface::fillSourceFilenameStruct(LPOPENFILENAME OFN) 
{
	//memset(OFN, 0, sizeof(OPENFILENAME));
	OFN->lStructSize = sizeof(OPENFILENAME);
	OFN->hwndOwner = NULL;
	OFN->lpstrFilter = __TEXT("All (*.*)\0*.*\0Text (*.txt)\0*.txt\0\0"); // фильтр для выбора, NULL - фильтр не установлен
	OFN->lpstrCustomFilter = NULL;
	OFN->nFilterIndex = 1;
	*sourceFileName = 0;
	OFN->lpstrFile = sourceFileName; // возвращает полный путь и имя выбранного файла
	OFN->nMaxFile = MAX_PATH; // размер буфера (в байтах - для ANSI, в символах для Unicode), возвращающего полный путь и имя выбранного файла - на единицу меньше, чем sizeof(bufferOutputFilePath)
	OFN->lpstrFileTitle = NULL;
	OFN->lpstrInitialDir = NULL;
	OFN->lpstrTitle = __TEXT("Выберите исходный файл");
	OFN->lpstrDefExt = __TEXT("txt");
	OFN->lpTemplateName = __TEXT("");
	OFN->Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
}
void UserInterface::fillResultFilenameStruct(LPOPENFILENAME OFN)
{
	//memset(OFN, 0, sizeof(OPENFILENAME));
	OFN->lStructSize = sizeof(OPENFILENAME);
	OFN->hwndOwner = NULL;
	OFN->lpstrFilter = __TEXT("All (*.*)\0*.*\0Text (*.txt)\0*.txt\0\0"); // фильтр для выбора, NULL - фильтр не установлен
	OFN->lpstrCustomFilter = NULL;
	OFN->nFilterIndex = 1;
	*resultFileName = 0;
	OFN->lpstrFile = resultFileName; // возвращает полный путь и имя выбранного файла
	OFN->nMaxFile = MAX_PATH; // размер буфера (в байтах - для ANSI, в символах для Unicode), возвращающего полный путь и имя выбранного файла - на единицу меньше, чем sizeof(bufferOutputFilePath)
	OFN->lpstrFileTitle = NULL;
	OFN->lpstrInitialDir = NULL;
	OFN->lpstrTitle = __TEXT("Выберите исходный файл");
	OFN->lpstrDefExt = __TEXT("txt");
	OFN->lpTemplateName = __TEXT("");
	OFN->Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR;
}

bool UserInterface::getOpenFileNames(LPOPENFILENAME sourceFile, LPOPENFILENAME resultFile)
{
	if (!GetOpenFileName(sourceFile))
	{
		printf("Source file was not selected.");
		return false;
	}
/*	
	if (!GetSaveFileName(resultFile))
	{
		printf("Result file was not selected.");
		return false;
	}
*/
	return true;
}

bool keyCheck(std::string KeyStr)
{
	if (KeyStr.size() != 16)
		return false;
	for (unsigned int i=0;i<16;++i)
	{
		if (! (									//symbols check
			((KeyStr[i]>47) & (KeyStr[i]<58)) 
			|| 
			((KeyStr[i]>64) & (KeyStr[i]<71)) 
			|| 
			((KeyStr[i]>96) & (KeyStr[i]<103)) 
			) )
			return false;
		else std::cin.putback(KeyStr[15-i]);
	}
	return true;
}

uint64 UserInterface::readKey()// w\ key check
{
	std::string KeyStr;
	uint64 result;
	while(TRUE)
	{
		std::cout<<"Enter hex key (0-F):"<<std::endl;
		std::cin>>KeyStr;
		if(keyCheck(KeyStr))
		{
			std::cin>>std::hex>>result;
			return result;
		}
		else
			std::cout<<"Wrong input";
	}
		
}

void UserInterface::cryptTypeSelection()
{
	cryptType = 0;
	while(!cryptType)
	{
		std::cout<<"Choose encryption/decryption algorithm type: "<<std::endl<< "DES(4), 3DES-EEE3(1), 3DES-EEE2(2), 3DES-EDE3(3) ..."<<std::endl;
		std::cin>>cryptType;
		//if ((cryptType < 1) || (cryptType > 4))\
		{\
			std::cout<<"Wrong input, try again"<<std::endl;\
			cryptType = 0;\
		}
		
	}
}

void UserInterface::cryptModeSelection()
{
	std::cout<<"Choose encryption/decryption algorithm type: ECB (0), CBC(1) ..."<<std::endl;
	std::cin>>cryptMode;
}
void UserInterface::cryptDecryptSelection(Crypt<uint64>* cryptObj)
{
	bool flag;
	std::cout<<"Choose Encryption(0)/Decryption(1)"<<std::endl;
	std::cin>>flag;
	cryptObj->isEncrypted = flag;
}