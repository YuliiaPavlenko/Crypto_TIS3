#pragma once
#include "Crypt.h"

class CBC_Encrypt :
	public Crypt<uint64>
{
public:
	virtual uint64 makeCrypt(uint64 block);
	CBC_Encrypt(Crypt* cryptObj);
	void setIV(uint64);
private:
	uint64 vectorIV;
	Crypt* cryptObject;
};

