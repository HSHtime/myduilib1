#include "stdafx.h"
#include "Base64Coder.h"

char CBase64Coder::ch64[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '='
};

CBase64Coder::CBase64Coder()
{
	buf = NULL;
	size = 0;
}

CBase64Coder::~CBase64Coder()
{
	if (buf)
		free(buf);
}

void CBase64Coder::allocMem(int NewSize)
{
	if (buf)
		buf = (char*)realloc(buf, NewSize);
	else
		buf = (char*)malloc(NewSize);
	memset(buf, 0, NewSize);
}

const  char* CBase64Coder::encode(const string& buffer)
{
	return encode(buffer.c_str(), buffer.length());
}

const char* CBase64Coder::encode(const char* buffer, int buflen)
{
	int nLeft = 3 - buflen % 3;
	//����BASE64�㷨���ܳ��Ȼ���ԭ����4/3��
	//�����ڴ���䣽length*4/3����1λ��Ϊ��������(0)
	allocMem((buflen + nLeft) * 4 / 3 + 1);
	//��ʱ������
	char ch[4];
	int i, j;
	for (i = 0, j = 0; i < (buflen - buflen % 3); i += 3, j += 4)
	{
		ch[0] = (char)((buffer[i] & 0xFC) >> 2);
		ch[1] = (char)((buffer[i] & 0x03) << 4 | (buffer[i + 1] & 0xF0) >> 4);
		ch[2] = (char)((buffer[i + 1] & 0x0F) << 2 | (buffer[i + 2] & 0xC0) >> 6);
		ch[3] = (char)((buffer[i + 2] & 0x3F));
		//��ѯ���������ȡ�������ַ�
		buf[j] = ch64[ch[0]];
		buf[j + 1] = ch64[ch[1]];
		buf[j + 2] = ch64[ch[2]];
		buf[j + 3] = ch64[ch[3]];
	}

	if (nLeft == 2)
	{
		ch[0] = (char)((buffer[i] & 0xFC) >> 2);
		ch[1] = (char)((buffer[i] & 0x3) << 4);
		ch[2] = 64;
		ch[3] = 64;

		//��ѯ���������ȡ�������ַ�
		buf[j] = ch64[ch[0]];
		buf[j + 1] = ch64[ch[1]];
		buf[j + 2] = ch64[ch[2]];
		buf[j + 3] = ch64[ch[3]];
	}
	else if (nLeft == 1)
	{
		ch[0] = (char)((buffer[i] & 0xFC) >> 2);
		ch[1] = (char)((buffer[i] & 0x03) << 4 | (buffer[i + 1] & 0xF0) >> 4);
		ch[2] = (char)((buffer[i + 1] & 0x0F) << 2);
		ch[3] = 64;

		//��ѯ���������ȡ�������ַ�
		buf[j] = ch64[ch[0]];
		buf[j + 1] = ch64[ch[1]];
		buf[j + 2] = ch64[ch[2]];
		buf[j + 3] = ch64[ch[3]];
	}
	return buf;
}

const char* CBase64Coder::decode(const char* buffer, int Length)
{
	int length = Length;
	if (length % 4 != 0)
		return NULL;

	allocMem(length * 3 / 4 + 1);

	char p;
	char ch[4];
	int i, j;
	for (i = 0, j = 0; i < length; i += 4, j += 3)
	{
		for (int z = 0; z < 4; z++)
		{
			//����2�ַ�����
			p = (char)BinSearch(buffer[i + z]);
			if (p == -1)
				return NULL;
			ch[z] = p;
		}

		buf[j] = (char)((ch[0] & 0x3F) << 2 | (ch[1] & 0x30) >> 4);
		buf[j + 1] = (char)((ch[1] & 0xF) << 4 | (ch[2] & 0x3C) >> 2);
		buf[j + 2] = (char)((ch[2] & 0x03) << 6 | (ch[3] & 0x3F));
	}
	return buf;
}

//���ö��ַ�����p��ch64�����е�λ�ã������ء�����Ҳ����򷵻�-1
int CBase64Coder::BinSearch(char p)
{
	if (p >= 'A' && p <= 'Z')
		return (p - 'A');
	else if (p >= 'a' && p <= 'z')
		return (p - 'a' + 26);
	else if (p >= '0' && p <= '9')
		return (p - '0' + 26 + 26);
	else if (p == '+')
		return 62;
	else if (p == '/')
		return 63;
	else if (p == '=')
		return 64;
	return -1;
}