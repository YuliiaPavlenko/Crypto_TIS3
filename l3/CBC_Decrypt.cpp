#include "stdafx.h"
#include "CBC_Decrypt.h"


CBC_Decrypt::CBC_Decrypt(Crypt* cryptObj)
: cryptObject(cryptObj)
{

}

uint64 CBC_Decrypt::makeCrypt(uint64 block)
{
	uint64 tmp = block;
	block = cryptObject->makeCrypt(block) ^ vectorIV;
	vectorIV = tmp;
	return block;
}

void CBC_Decrypt::setIV(uint64 valueIV)
{
	vectorIV = valueIV;
}