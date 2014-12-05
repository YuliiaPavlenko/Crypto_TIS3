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
		hSourceFile = CreateFile(sourceFileName,    // открываемый файл
			GENERIC_READ,          // открываем для чтения
			0,       // совместное чтение не используется
			NULL,                  // защита по умолчанию
			OPEN_EXISTING,         // только существующий файл
			FILE_FLAG_OVERLAPPED, // асинхронный режим
			NULL);                 // атрибутов шаблона нет

		if (hSourceFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		hResultFile = CreateFile(resultFileName,    // открываемый файл
			GENERIC_WRITE,          // открываем для чтения
			0,       // совместное чтение не используется
			NULL,                  // защита по умолчанию
			CREATE_ALWAYS,  // только существующий файл
			FILE_FLAG_OVERLAPPED, // асинхронный режим
			NULL);                 // атрибутов шаблона нет

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
		//проверякм открыты ли файлы
		if ((hSourceFile == INVALID_HANDLE_VALUE) | (hResultFile == INVALID_HANDLE_VALUE))
			return false;

	    //объявляем овэрлэпд структуры для асинхронной работы с файлом
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
		//на маленький был ругается ворнингами компилятор
		BOOL readResult = FALSE;
		BOOL writeResult = FALSE;

		//readWriteBuffer = new blocktype[clusterSize];

		//инициализация массивов для побайтного чтения/записи
		readBuffers[0] = new unsigned char[clusterSize];
		readBuffers[1] = new unsigned char[clusterSize];
		writeBuffer = new unsigned char[clusterSize];
		//для указателей на эти приводим через войд тип к блоктайпу для поблочной обработки
		blockReadBuffers[0] = (blocktype*)( (LPVOID*)(&readBuffers[0][0]) ); 
		blockReadBuffers[1] = (blocktype*)( (LPVOID*)(&readBuffers[1][0]) );
		blockWriteBuffer = (blocktype*)( (LPVOID*)(&writeBuffer[0]) );
		//объявляем переменные для выбора буфера чтения
		unsigned char readBufferNumber = 0;
		unsigned char tmpReadBufferNumber = 0;
		blocktype *tmpBlockBuffer;
		
		//размер блока
		block64 filesize;
		filesize.lo = GetFileSize(hSourceFile, &(filesize.hi));

		//проверка нужно ли расширять последний блок и получение его размера
		unsigned char extensionNeeded = filesize.data % sizeof(blocktype);
		 
		//считаем количество итерация для считывания и поблочной обработки
		unsigned readWriteIterations =  (filesize.data / clusterSize) + ( (filesize.data % clusterSize) ? 1 : 0) - 1 ;
		unsigned blockCryptIterations = (clusterSize / sizeof(blocktype));
		//-------------------------------
		//первое считывание
		if(ReadFile(hSourceFile,(LPVOID) readBuffers[readBufferNumber], clusterSize,&nBytesRead,&sourceOverlap) == FALSE)
			GetOverlappedResult(hSourceFile,&sourceOverlap,&nBytesRead, TRUE);
		//вход в цикл, последний блок шифруется после цикла (<)
		for(unsigned i = 0; i < readWriteIterations; ++i)
		{
			//передвигаем указатель сдвига в читаемом файле
			sourceOverlap.Offset += clusterSize;
			//записываем второй номер буффера, отличный от первого во вторую переменную выбора буффера 
			tmpReadBufferNumber = !readBufferNumber;

			readResult = ReadFile(hSourceFile,(LPVOID) readBuffers[tmpReadBufferNumber], clusterSize,&nBytesRead,&sourceOverlap);
			
			//если не первая итерация, то входим и если не успела произойти запись в файл то ждем ее ркрнчания и передвигаем указатель сдвига в файле
			if(i)
			{
				if(!writeResult)     
					WaitForSingleObject(resultOverlap.hEvent, INFINITE);
				resultOverlap.Offset += clusterSize;
			}

			//присваиваем из первого буффера чтения во временный буффер и из него шифруем в буффер для записи
			
			for(unsigned j = 0; j < blockCryptIterations; ++j)
			{
				blockWriteBuffer[j] = cryptObject->makeCrypt(blockReadBuffers[readBufferNumber][j]);
			}

			//если не предпоследняя итерация, то входим и записываем в файл
			if(i != (readWriteIterations - 1))
				writeResult = WriteFile(hSourceFile, (LPVOID) writeBuffer, clusterSize, &nBytesWrite, &resultOverlap);
			//присваиваем прошлому номеру буффера текущий)
			readBufferNumber = tmpReadBufferNumber;
			//если не было прочитано из файла то ждем
			if(!readResult)
				GetOverlappedResult(hSourceFile, &sourceOverlap, &nBytesRead, TRUE);
		}
		//рассчитываем кол-во итераций шифрования блоков по кол-ву считаных байтов для последнего куска
		blockCryptIterations = nBytesRead / sizeof(blocktype) + (nBytesRead % sizeof(blocktype) ? 1 : 0);
		//присваиваем из последнего буффера чтения в временный
		tmpBlockBuffer = blockReadBuffers[readBufferNumber];
		if(!cryptObject->isEncrypted) // если шифруем
		{
			if(readWriteIterations)//если файл больше размера буфера, то записываем предпоследний кусок
			{
				if(!WriteFile(hResultFile, (LPVOID) writeBuffer, clusterSize, &nBytesWrite, &resultOverlap))
					WaitForSingleObject(resultOverlap.hEvent, INFINITE);
				resultOverlap.Offset += clusterSize;
			}
			//шифруем последний кусок
			for(unsigned j=0; j < blockCryptIterations; ++j)
				blockWriteBuffer[j] = cryptObject->makeCrypt(tmpBlockBuffer[j]);
			if(extensionNeeded)//если нужно расширять последний кусок
			{
				//пишем исходный размер блока до расширения в конец буфера и увеличиваем количество байт для записи на 1
				*(unsigned char*)(&blockWriteBuffer[blockCryptIterations]) = extensionNeeded;
				nBytesRead += sizeof(blocktype) - extensionNeeded + 1;
			}
		}
		else //если дешифруем
		{
			if(nBytesRead > 1) //если считано больше 1 байта
			{
				if(readWriteIterations)
				{
					if(!WriteFile(hResultFile, (LPVOID) writeBuffer, clusterSize, &nBytesWrite, &resultOverlap))
						WaitForSingleObject(resultOverlap.hEvent, INFINITE);
					resultOverlap.Offset += clusterSize;
				}
				for(unsigned j=0; j < blockCryptIterations; ++j)
					blockWriteBuffer[j] = cryptObject->makeCrypt(tmpBlockBuffer[j]);
				if(filesize.data & 1) //проверяем четность размера, если нечетный то нужно убрать расширение
				{
					//читаем исходный размер блока и соответственно уменьшаем количество байт для записи
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
		//запись последнего куска
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
	unsigned char *readBuffers[2]; // буфферы для побайтного считывания/записи по размеру кластера
	unsigned char *writeBuffer;
	blocktype *blockReadBuffers[2];
	blocktype *blockWriteBuffer;// буфферы для поблочной обработки считанного 
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










