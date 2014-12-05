#include "stdafx.h"
#include "CBC_Encrypt.h"


CBC_Encrypt::CBC_Encrypt(Crypt* cryptObj)
: cryptObject(cryptObj)
{
}

uint64 CBC_Encrypt::makeCrypt(uint64 block)
{
	//block ^= vectorIV;
	return vectorIV = cryptObject->makeCrypt(block ^ vectorIV);
}

void CBC_Encrypt::setIV(uint64 valueIV)
{
	vectorIV = valueIV;
}