#include "stdafx.h"
#include "DES3.h"

DES3::DES3(SecureKey* keyObj) 
	: DES(keyObj)
{
	keyObjectDES3 = keyObj;
}

uint64 DES3::makeCrypt(uint64 block)
{
	*DES::keyObject = keyObjectDES3[0];
	block = DES::makeCrypt(block);
	*DES::keyObject = keyObjectDES3[1];
	block = DES::makeCrypt(block);
	*DES::keyObject = keyObjectDES3[2];
	return DES::makeCrypt(block);
}
