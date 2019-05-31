#include "bitreader.h"

void fillReservoir(BitReader *bitReader)
{
	bitReader->mReservoir = 0;
  size_t i;
  for (i = 0; bitReader->mSize > 0 && i < 4; ++i)
	{
    bitReader->mReservoir = (bitReader->mReservoir << 8) | *(bitReader->mData);

    ++bitReader->mData;
    --bitReader->mSize;
  }

  bitReader->mNumBitsLeft = 8 * i;
  bitReader->mReservoir <<= 32 - bitReader->mNumBitsLeft;
}

void putBits(BitReader *bitReader, uint32_t x, size_t n)
{
	bitReader->mReservoir = (bitReader->mReservoir >> n) | (x << (32 - n));
  bitReader->mNumBitsLeft += n;

}

void InitBitReader(BitReader *bitReader, uint8_t *data, size_t size)
{
	bitReader->mData = data;
	bitReader->mSize = size;
	bitReader->mReservoir = 0;
	bitReader->mNumBitsLeft = 0;
}

uint32_t GetBits(BitReader *bitReader, size_t n)
{
	uint32_t result = 0;
  while (n > 0)
	{
    if (bitReader->mNumBitsLeft == 0)
		{
      fillReservoir(bitReader);
    }

    size_t m = n;
    if (m > bitReader->mNumBitsLeft)
		{
      m = bitReader->mNumBitsLeft;
    }

    result = (result << m) | (bitReader->mReservoir >> (32 - m));
    bitReader->mReservoir <<= m;
    bitReader->mNumBitsLeft -= m;
    n -= m;
  }

  return result;
}

void SkipBits(BitReader *bitReader, size_t n)
{
	while (n > 32)
	{
    GetBits(bitReader, 32);
    n -= 32;
  }

  if (n > 0)
	{
    GetBits(bitReader, n);
  }
}

size_t numBitsLeft(BitReader *bitReader)
{
	return bitReader->mSize * 8 + bitReader->mNumBitsLeft;
}

