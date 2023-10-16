// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <=0) { throw "len <= 0"; }
	else{
		BitLen = len;
		MemLen = len / 32 + 1;
		pMem = new TELEM[MemLen];
		
		for (int i = 0; i < MemLen; i++) 
		{
			pMem[i] = 0;
		}
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = nullptr;
	BitLen = 0;
	MemLen = 0;
	

}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n > BitLen - 1 || n < 0)
	{
		throw "Индекс вне границ поля ";
	}
	return n/32;
}

TELEM TBitField::GetMemMask(const int n)  const// битовая маска для бита n
{
	TELEM t;
	t= 1;
	int shift;
	shift= n % 32;
	TELEM result;
	result= t << shift;
	return result;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n > BitLen-1)
	{
		throw "Размер <0 или >Bitlen";
	}
	TELEM mask = GetMemMask(n);
	int index = GetMemIndex(n);
	pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen-1)
	{
		throw "Размер <0 или >Bitlen";
	}
	TELEM mask = ~(GetMemMask(n));
	int index = GetMemIndex(n);
	pMem[index] = pMem[index] & mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n > BitLen-1)
	{
		throw "Размер <0 или >Bitlen";
	}
	TELEM  mask = GetMemMask(n);
	int index = GetMemIndex(n);
	TELEM res;
	res=pMem[index] & mask;
	return res;
}

// битовые операции

bool TBitField::operator == (const TBitField &bf) const // сравнение;
{
	if (BitLen != bf.BitLen)
	{
		return false;
    }
	for (int i = 0; i < MemLen - 1; i++)
	{
		if (pMem[i] != bf.pMem[i]) { return false; }
	}
	for (int k = (MemLen - 1) * 32; k < BitLen; k++)
	{
		if (GetBit(k) != bf.GetBit(k)) { return false; }
	}
	return true;
}

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (*this != bf)
	{
		delete[]pMem;
		pMem = new TELEM[bf.MemLen];
		MemLen = bf.MemLen;
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}



TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	/*int maxL = bf.BitLen;
	int minL = BitLen;
	if (BitLen > bf.BitLen)
	{
		maxL = BitLen;
		minL = bf.BitLen;
	}
	TBitField res(maxL);

	int currentMemLen;
	if (minL = BitLen)
	{
		currentMemLen = MemLen;
	}
	else
	{
		currentMemLen = bf.MemLen;
	}
	for (int i = 0; i < currentMemLen-2; i++)
	{
		res.pMem[i] = pMem[i];
	}
	
	for (int k = (currentMemLen - 1) * 32; k < minL; k++)
	{
		if ((bf.GetBit(k) | res.GetBit(k)) == 0)
		{
			res.ClrBit(k);
		}
		else
		{
			res.SetBit(k);
		}
	}
	TBitField helpme(bf);
	for (int k = minL; k < (currentMemLen*32)-1; k++)
	{
		
		helpme.ClrBit(k);
		if ((helpme.GetBit(k) | res.GetBit(k)) == 0)
		{
			res.ClrBit(k);
		}
		else
		{
			res.SetBit(k);
		}
		
	}
	return res;*/
	if (BitLen > bf.BitLen) {
		TBitField ans(*this);
		for (int i = 0; i < bf.MemLen; i++) 
		{
			ans.pMem[i] = pMem[i] | bf.pMem[i];
		}
		for (int i = (bf.MemLen - 1) * 32 + bf.BitLen % 32; i < BitLen; i++) 
		{
			if (GetBit(i)) {
				ans.SetBit(i);
			}
			else {
				ans.ClrBit(i);
			}
		}
		return ans;
	}
	else if (bf.BitLen > BitLen) {
		TBitField ans(bf);
		for (int i = 0; i < MemLen; i++) 
		{
			ans.pMem[i] = pMem[i] | bf.pMem[i];
		}
		for (int i = (MemLen - 1) * 32 + BitLen % 32; i < bf.BitLen; i++) 
		{
			if (bf.GetBit(i)) {
				ans.SetBit(i);
			}
			else {
				ans.ClrBit(i);
			}
		}
		return ans;
	}
	else 
	{
		TBitField ans(bf);
		for (int i = 0; i < MemLen; i++)
		{
			ans.pMem[i] = pMem[i] | bf.pMem[i];
		}
		return ans;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	/*if (BitLen != bf.BitLen)
	{
		throw("Длины Bitlen не совпадают");
	}
	TBitField res(MemLen);
	for (int i = 0; i < MemLen; i++)
	{
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return res;*/
	if (BitLen > bf.BitLen) {
		TBitField ans(*this);
		for (int i = 0; i < bf.MemLen; i++) {
			ans.pMem[i] = bf.pMem[i] & pMem[i];
		}
		for (int i = (bf.MemLen - 1) * 32 + bf.BitLen % 32; i < BitLen; i++) {
			ans.ClrBit(i);
		}
		return ans;
	}
	else if (bf.BitLen > BitLen) {
		TBitField ans(bf);
		for (int i = 0; i < MemLen; i++) {
			ans.pMem[i] = bf.pMem[i] & pMem[i];
		}
		for (int i = (MemLen - 1) * 32 + BitLen % 32; i < bf.BitLen; i++) {
			ans.ClrBit(i);
		}
		return ans;
	}
	else {
		TBitField ans(bf);
		for (int i = 0; i < MemLen; i++) {
			ans.pMem[i] = pMem[i] & bf.pMem[i];
		}
		return ans;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	for (int i = 0; i < MemLen; i++)
	{
		res.pMem[i] = ~(res.pMem[i]);
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char a;
	for (int i = 0; i < bf.MemLen; i++)
	{
		bf.pMem[i] = 0;
	}
	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> a;
		if (a == '1')
		{
			bf.SetBit(i);
		}
		else
		{
			if (a == '0')
			{
				bf.ClrBit(i);
			}
			else
			{
				break;
			}
		}
	}
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i))
		{
			ostr << '1';

		}
		else
		{
			ostr << '0';
		}
	}
	return ostr;

}
