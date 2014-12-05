#pragma once
#include "stdafx.h" 
#include <windows.h>
#include "crypt.h"
#include "UserInterface.h"

template < typename blocktype > class ReadWrite
{
public:

	ReadWrite(Crypt<blocktype>* cryptObj)
		:cryptObject(cryptObj)
	{
	
	}

	bool openFiles(LPTSTR sourceFileName, LPTSTR resultFileName)//ok
	{
		hSourceFile = CreateFile(sourceFileName,    // ����������� ����
			GENERIC_READ,          // ��������� ��� ������
			0,       // ���������� ������ �� ������������
			NULL,                  // ������ �� ���������
			OPEN_EXISTING,         // ������ ������������ ����
			FILE_FLAG_OVERLAPPED, // ����������� �����
			NULL);                 // ��������� ������� ���

		if (hSourceFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		hResultFile = CreateFile(resultFileName,    // ����������� ����
			GENERIC_WRITE,          // ��������� ��� ������
			0,       // ���������� ������ �� ������������
			NULL,                  // ������ �� ���������
			CREATE_ALWAYS,  // ������ ������������ ����
			FILE_FLAG_OVERLAPPED, // ����������� �����
			NULL);                 // ��������� ������� ���

		if (hResultFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hSourceFile);
			hSourceFile = INVALID_HANDLE_VALUE;
			return false;
		}

		clusterSize = (clusterSizeDetection(sourceFileName) > clusterSizeDetection(sourceFileName)) ? clusterSizeDetection(sourceFileName) : clusterSizeDetection(sourceFileName);

		return true;
	}

	bool closeFiles()//ok
	{
		CloseHandle(hSourceFile);
		hSourceFile = INVALID_HANDLE_VALUE;
		CloseHandle(hResultFile);
		hResultFile = INVALID_HANDLE_VALUE;
		return true;
	}

	bool fileProcessing()
	{
		//��������� ������� �� �����
		if ((hSourceFile == INVALID_HANDLE_VALUE) | (hResultFile == INVALID_HANDLE_VALUE))
			return false;

	    //��������� �������� ��������� ��� ����������� ������ � ������
		OVERLAPPED sourceOverlap;
		sourceOverlap.Internal = 0;
		sourceOverlap.InternalHigh = 0;
		sourceOverlap.Offset = 0;
		sourceOverlap.OffsetHigh = 0;
		sourceOverlap.hEvent=CreateEvent(0,FALSE,FALSE,0);

		OVERLAPPED resultOverlap;
		resultOverlap.Internal = 0;
		resultOverlap.InternalHigh = 0;
		resultOverlap.Offset = 0;
		resultOverlap.OffsetHigh = 0;
		resultOverlap.hEvent=CreateEvent(0,FALSE,FALSE,0);

		DWORD nBytesRead; 
		DWORD nBytesWrite;
		//�� ��������� ��� �������� ���������� ����������
		BOOL readResult = FALSE;
		BOOL writeResult = FALSE;

		//readWriteBuffer = new blocktype[clusterSize];

		//������������� �������� ��� ���������� ������/������
		readBuffers[0] = new unsigned char[clusterSize];
		readBuffers[1] = new unsigned char[clusterSize];
		writeBuffer = new unsigned char[clusterSize];
		//��� ���������� �� ��� �������� ����� ���� ��� � ��������� ��� ��������� ���������
		blockReadBuffers[0] = (blocktype*)( (LPVOID*)(&readBuffers[0][0]) ); 
		blockReadBuffers[1] = (blocktype*)( (LPVOID*)(&readBuffers[1][0]) );
		blockWriteBuffer = (blocktype*)( (LPVOID*)(&writeBuffer[0]) );
		//��������� ���������� ��� ������ ������ ������
		unsigned char readBufferNumber = 0;
		unsigned char tmpReadBufferNumber = 0;
		blocktype *tmpBlockBuffer;
		
		//������ �����
		block64 filesize;
		filesize.lo = GetFileSize(hSourceFile, &(filesize.hi));

		//�������� ����� �� ��������� ��������� ���� � ��������� ��� �������
		unsigned char extensionNeeded = filesize.data % sizeof(blocktype);
		 
		//������� ���������� �������� ��� ���������� � ��������� ���������
		unsigned readWriteIterations =  (filesize.data / clusterSize) + ( (filesize.data % clusterSize) ? 1 : 0) - 1 ;
		unsigned blockCryptIterations = (clusterSize / sizeof(blocktype));
		//-------------------------------
		//������ ����������
		if(ReadFile(hSourceFile,(LPVOID) readBuffers[readBufferNumber], clusterSize,&nBytesRead,&sourceOverlap) == FALSE)
			GetOverlappedResult(hSourceFile,&sourceOverlap,&nBytesRead, TRUE);
		//���� � ����, ��������� ���� ��������� ����� ����� (<)
		for(unsigned i = 0; i < readWriteIterations; ++i)
		{
			//����������� ��������� ������ � �������� �����
			sourceOverlap.Offset += clusterSize;
			//���������� ������ ����� �������, �������� �� ������� �� ������ ���������� ������ ������� 
			tmpReadBufferNumber = !readBufferNumber;

			readResult = ReadFile(hSourceFile,(LPVOID) readBuffers[tmpReadBufferNumber], clusterSize,&nBytesRead,&sourceOverlap);
			
			//���� �� ������ ��������, �� ������ � ���� �� ������ ��������� ������ � ���� �� ���� �� ��������� � ����������� ��������� ������ � �����
			if(i)
			{
				if(!writeResult)     
					WaitForSingleObject(resultOverlap.hEvent, INFINITE);
				resultOverlap.Offset += clusterSize;
			}

			//����������� �� ������� ������� ������ �� ��������� ������ � �� ���� ������� � ������ ��� ������
			
			for(unsigned j = 0; j < blockCryptIterations; ++j)
			{
				blockWriteBuffer[j] = cryptObject->makeCrypt(blockReadBuffers[readBufferNumber][j]);
			}

			//���� �� ������������� ��������, �� ������ � ���������� � ����
			if(i != (readWriteIterations - 1))
				writeResult = WriteFile(hSourceFile, (LPVOID) writeBuffer, clusterSize, &nBytesWrite, &resultOverlap);
			//����������� �������� ������ ������� �������)
			readBufferNumber = tmpReadBufferNumber;
			//���� �� ���� ��������� �� ����� �� ����
			if(!readResult)
				GetOverlappedResult(hSourceFile, &sourceOverlap, &nBytesRead, TRUE);
		}
		//������������ ���-�� �������� ���������� ������ �� ���-�� �������� ������ ��� ���������� �����
		blockCryptIterations = nBytesRead / sizeof(blocktype) + (nBytesRead % sizeof(blocktype) ? 1 : 0);
		//����������� �� ���������� ������� ������ � ���������
		tmpBlockBuffer = blockReadBuffers[readBufferNumber];
		if(!cryptObject->isEncrypted) // ���� �������
		{
			if(readWriteIterations)//���� ���� ������ ������� ������, �� ���������� ������������� �����
			{
				if(!WriteFile(hResultFile, (LPVOID) writeBuffer, clusterSize, &nBytesWrite, &resultOverlap))
					WaitForSingleObject(resultOverlap.hEvent, INFINITE);
				resultOverlap.Offset += clusterSize;
			}
			//������� ��������� �����
			for(unsigned j=0; j < blockCryptIterations; ++j)
				blockWriteBuffer[j] = cryptObject->makeCrypt(tmpBlockBuffer[j]);
			if(extensionNeeded)//���� ����� ��������� ��������� �����
			{
				//����� �������� ������ ����� �� ���������� � ����� ������ � ����������� ���������� ���� ��� ������ �� 1
				*(unsigned char*)(&blockWriteBuffer[blockCryptIterations]) = extensionNeeded;
				nBytesRead += sizeof(blocktype) - extensionNeeded + 1;
			}
		}
		else //���� ���������
		{
			if(nBytesRead > 1) //���� ������� ������ 1 �����
			{
				if(readWriteIterations)
				{
					if(!WriteFile(hResultFile, (LPVOID) writeBuffer, clusterSize, &nBytesWrite, &resultOverlap))
						WaitForSingleObject(resultOverlap.hEvent, INFINITE);
					resultOverlap.Offset += clusterSize;
				}
				for(unsigned j=0; j < blockCryptIterations; ++j)
					blockWriteBuffer[j] = cryptObject->makeCrypt(tmpBlockBuffer[j]);
				if(filesize.data & 1) //��������� �������� �������, ���� �������� �� ����� ������ ����������
				{
					//������ �������� ������ ����� � �������������� ��������� ���������� ���� ��� ������
					extensionNeeded = *(unsigned char*)(&tmpBlockBuffer[blockCryptIterations - 1]);
					nBytesRead = nBytesRead - (sizeof(blocktype) - extensionNeeded) - 1; 				
				}
			}
			else 
			{
				extensionNeeded = *(unsigned char*)(&tmpBlockBuffer[0]);
				nBytesRead = clusterSize - (sizeof(blocktype) - extensionNeeded);
			}
		}
		//������ ���������� �����
		if(!WriteFile(hResultFile, (LPVOID) writeBuffer, nBytesRead, &nBytesWrite, &resultOverlap))
			WaitForSingleObject(resultOverlap.hEvent, INFINITE);

		//-----------------
		CloseHandle(sourceOverlap.hEvent);
		CloseHandle(resultOverlap.hEvent);

		return closeFiles();
	}

private:
	Crypt<blocktype>* cryptObject;
	HANDLE hSourceFile;
	HANDLE hResultFile;
	//blocktype *readWriteBuffer;
	unsigned char *readBuffers[2]; // ������� ��� ���������� ����������/������ �� ������� ��������
	unsigned char *writeBuffer;
	blocktype *blockReadBuffers[2];
	blocktype *blockWriteBuffer;// ������� ��� ��������� ��������� ���������� 
	unsigned int clusterSize;

	unsigned clusterSizeDetection(LPTSTR fileName)//ok
	{
		DWORD sectorPerCluster = 0;
		DWORD bytesPerSector = 0;
		DWORD numberOfFreeClusters = 0;
		DWORD totalNumberOfClusters = 0;
		fileName[3] = '\0';
		if (GetDiskFreeSpace(fileName, &sectorPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters) == 0)
			return sectorPerCluster * bytesPerSector;
		else
			return 4096;
	}
	
	
};










