#pragma once
#include "stdafx.h"
#include "Crypt.h"
#include "SecureKey.h"

class DES : public Crypt<uint64>
{
public:
	DES(SecureKey* keyObj);

	virtual uint64 makeCrypt(uint64 block);
	//void cryptRound();
private:
	char iterationIndex;
	uint64 extension32To48(unsigned word);
	uint64 initialPermutation(uint64 source);
	uint64 endPermutation(uint64 source);
	uint64 pPermutation(uint64 source);
	unsigned fCrypt(unsigned right);	
protected:
	SecureKey* keyObject;
};
