// l3.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"

#include <iostream>
#include <string>
#include "UserInterface.h"
#include "DES.h"
#include "DES3.h"
#include "ReadWrite.h"
#include "CBC_Encrypt.h"
#include "CBC_Decrypt.h"

void main()
{
	UserInterface Interface;
	OPENFILENAME sourceFile;
	OPENFILENAME resultFile;


	Interface.cryptTypeSelection();
	Interface.cryptModeSelection();
	SecureKey Keys[3];

	Interface.fillResultFilenameStruct(&resultFile);
	Interface.fillSourceFilenameStruct(&sourceFile);
	Interface.getOpenFileNames(&sourceFile, &resultFile);
	_tcscpy(Interface.resultFileName, __TEXT("out.txt"));
	
	int a;
	if(!Interface.cryptMode) //ECB
	{
		std::cout<<"ecb"<<std::endl;

		if (Interface.cryptType = 4) //DES
		{
			std::cout<<"des"<<std::endl;

			SecureKey KeyDES(Interface.readKey());
			DES des(&KeyDES);
			Interface.cryptDecryptSelection(&des);
			KeyDES.roundsKeysGeneration(des.isEncrypted);
			ReadWrite<uint64> RW(&des);

			RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
			RW.fileProcessing();
			

		}
		else						//3DES
		{
			std::cout<<"3des"<<std::endl;
			if(Interface.cryptType = 1)         
			{
				std::cout<<"eee3"<<std::endl;
				DES3 des3(Keys);
				Keys[0].setKey(Interface.readKey());
				Keys[1].setKey(Interface.readKey());
				Keys[2].setKey(Interface.readKey());
				Interface.cryptDecryptSelection(&des3);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				ReadWrite<uint64> RW(&des3);

				RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
				RW.fileProcessing();


			}

			if(Interface.cryptType = 2)
			{
				std::cout<<"ede3"<<std::endl;
				DES3 des3(Keys);
				Keys[0].setKey(Interface.readKey());
				Keys[1].setKey(Interface.readKey());
				Keys[2].setKey(Interface.readKey());
				Interface.cryptDecryptSelection(&des3);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[0].roundsKeysGeneration(!des3.isEncrypted);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				ReadWrite<uint64> RW(&des3);

				RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
				RW.fileProcessing();
			}

			if(Interface.cryptType = 3)
			{
				std::cout<<"eee2"<<std::endl;
				DES3 des3(Keys);
				Keys[0].setKey(Interface.readKey());
				Keys[1].setKey(Interface.readKey());
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[1].roundsKeysGeneration(des3.isEncrypted);
				Keys[2] = Keys[1];
				ReadWrite<uint64> RW(&des3);

				RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
				RW.fileProcessing();

			}

		}
	}
	else					//CBC
	{
		std::cout<<"cbc"<<std::endl;
		if (Interface.cryptType = 4) //DES
		{
			std::cout<<"des"<<std::endl;
			SecureKey KeyDES(Interface.readKey());
			DES des(&KeyDES);
			Interface.cryptDecryptSelection(&des);
			KeyDES.roundsKeysGeneration(des.isEncrypted);
			if (des.isEncrypted)
			{
				CBC_Decrypt cbc_d(&des);
				cbc_d.setIV(Interface.readKey());
				ReadWrite<uint64> RW(&cbc_d);
				RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
				RW.fileProcessing();
			}
			else
			{
				CBC_Encrypt cbc_e(&des);
				cbc_e.setIV(Interface.readKey());
				ReadWrite<uint64> RW(&cbc_e);
				RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
				RW.fileProcessing();
			}
		}
		else						//3DES
		{
			std::cout<<"3des"<<std::endl;
			if (Interface.cryptType = 1)
			{
				std::cout<<"eee3"<<std::endl;
				DES3 des3(Keys);
				Keys[0].setKey(Interface.readKey());
				Keys[1].setKey(Interface.readKey());
				Keys[2].setKey(Interface.readKey());
				Interface.cryptDecryptSelection(&des3);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[1].roundsKeysGeneration(des3.isEncrypted);
				Keys[2].roundsKeysGeneration(des3.isEncrypted);
				if (des3.isEncrypted)
				{
					CBC_Decrypt cbc_d(&des3);
					cbc_d.setIV(Interface.readKey());
					ReadWrite<uint64> RW(&cbc_d);
					RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
					RW.fileProcessing();
				}
				else
				{
					CBC_Encrypt cbc_e(&des3);
					cbc_e.setIV(Interface.readKey());
					ReadWrite<uint64> RW(&cbc_e);
					RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
					RW.fileProcessing();
				}
			}
			if (Interface.cryptType = 2)
			{
				std::cout<<"ede3"<<std::endl;
				DES3 des3(Keys);
				Keys[0].setKey(Interface.readKey());
				Keys[1].setKey(Interface.readKey());
				Keys[2].setKey(Interface.readKey());
				Interface.cryptDecryptSelection(&des3);
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[1].roundsKeysGeneration(!des3.isEncrypted);
				Keys[2].roundsKeysGeneration(des3.isEncrypted);
				if (des3.isEncrypted)
				{
					CBC_Decrypt cbc_d(&des3);
					cbc_d.setIV(Interface.readKey());
					ReadWrite<uint64> RW(&cbc_d);
					RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
					RW.fileProcessing();
				}
				else
				{
					CBC_Encrypt cbc_e(&des3);
					cbc_e.setIV(Interface.readKey());
					ReadWrite<uint64> RW(&cbc_e);
					RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
					RW.fileProcessing();
				}
			}

			if (Interface.cryptType = 3)
			{
				std::cout<<"eee2"<<std::endl;
				DES3 des3(Keys);
				Keys[0].setKey(Interface.readKey());
				Keys[1].setKey(Interface.readKey());
				Keys[0].roundsKeysGeneration(des3.isEncrypted);
				Keys[1].roundsKeysGeneration(des3.isEncrypted);
				Keys[2] = Keys[1];
				if (des3.isEncrypted)
				{
					CBC_Decrypt cbc_d(&des3);
					cbc_d.setIV(Interface.readKey());
					ReadWrite<uint64> RW(&cbc_d);
					RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
					RW.fileProcessing();
				}
				else
				{
					CBC_Encrypt cbc_e(&des3);
					cbc_e.setIV(Interface.readKey());
					ReadWrite<uint64> RW(&cbc_e);
					RW.openFiles(Interface.sourceFileName, Interface.resultFileName);
					RW.fileProcessing();
				}
			}
		}
	}
	

	std::cin>>a;

};


