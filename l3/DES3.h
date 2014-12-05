#pragma once
#include "DES.h"

class DES3 :
	public DES
{
public:
	DES3(SecureKey* keyObj);
	virtual uint64 makeCrypt(uint64 block);
	
private:
	SecureKey* keyObjectDES3;
};

