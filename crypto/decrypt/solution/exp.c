// Random.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SHFR(x, n) (((x) >> (n)))
#define ROTR(x, n) (((x) >> (n)) | ((x) << ((sizeof(x) << 3) - (n))))
#define ROTL(x, n) (((x) << (n)) | ((x) >> ((sizeof(x) << 3) - (n))))

#define CHX(x, y, z) (((x) &  (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) &  (y)) ^ ( (x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define BSIG1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SSIG0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define SSIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

#define SHA256_BLOCK_SIZE (512/8)
#define SHA256_COVER_SIZE (SHA256_BLOCK_SIZE*2)

#define DWORD unsigned int
#define BYTE unsigned char
#define INT64 long
#define NULL 0
// #define TRUE 1
#define LPWORD (unsigned short*)

static DWORD k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static int ztransform(const BYTE* msg, DWORD* h)
{
	DWORD w[64];
	DWORD a0, b1, c2, d3, e4, f5, g6, h7;
	DWORD t1, t2;

	int i = 0;
	int j = 0;

	for (i = 0; i < 16; i++) {
		w[i] = msg[j] << 24 | msg[j + 1] << 16 | msg[j + 2] << 8 | msg[j + 3];
		j += 4;
	}

	for (i = 16; i < 64; i++) {
		w[i] = SSIG1(w[i - 2]) + w[i - 7] + SSIG0(w[i - 15]) + w[i - 16];
	}

	a0 = h[0];
	b1 = h[1];
	c2 = h[2];
	d3 = h[3];
	e4 = h[4];
	f5 = h[5];
	g6 = h[6];
	h7 = h[7];

	for (i = 0; i < 64; i++) {
		t1 = h7 + BSIG1(e4) + CHX(e4, f5, g6) + k[i] + w[i];
		t2 = BSIG0(a0) + MAJ(a0, b1, c2);

		h7 = g6;
		g6 = f5;
		f5 = e4;
		e4 = d3 + t1;
		d3 = c2;
		c2 = b1;
		b1 = a0;
		a0 = t1 + t2;
	}

	h[0] += a0;
	h[1] += b1;
	h[2] += c2;
	h[3] += d3;
	h[4] += e4;
	h[5] += f5;
	h[6] += g6;
	h[7] += h7;

	return 0;
}

int zsha256(void* src, DWORD len, BYTE* hash)
{
	BYTE* tmp = (BYTE*)src;
	BYTE  cover_data[SHA256_COVER_SIZE];
	DWORD cover_size = 0;

	DWORD i = 0;
	DWORD n = 0;
	DWORD m = 0;
	DWORD h[8];

	h[0] = 0x6a09e667;
	h[1] = 0xbb67ae85;
	h[2] = 0x3c6ef372;
	h[3] = 0xa54ff53a;
	h[4] = 0x510e527f;
	h[5] = 0x9b05688c;
	h[6] = 0x1f83d9ab;
	h[7] = 0x5be0cd19;

	memset(cover_data, 0x00, sizeof(BYTE) * SHA256_COVER_SIZE);

	n = len / SHA256_BLOCK_SIZE;
	m = len % SHA256_BLOCK_SIZE;

	if (m < 56) {
		cover_size = SHA256_BLOCK_SIZE;
	}
	else {
		cover_size = SHA256_BLOCK_SIZE * 2;
	}

	if (m != 0) {
		memcpy(cover_data, tmp + (n * SHA256_BLOCK_SIZE), m);
	}
	cover_data[m] = 0x80;
	cover_data[cover_size - 4] = ((len * 8) & 0xff000000) >> 24;
	cover_data[cover_size - 3] = ((len * 8) & 0x00ff0000) >> 16;
	cover_data[cover_size - 2] = ((len * 8) & 0x0000ff00) >> 8;
	cover_data[cover_size - 1] = ((len * 8) & 0x000000ff);

	for (i = 0; i < n; i++) {
		ztransform(tmp, h);
		tmp += SHA256_BLOCK_SIZE;
	}

	tmp = cover_data;
	n = cover_size / SHA256_BLOCK_SIZE;
	for (i = 0; i < n; i++) {
		ztransform(tmp, h);
		tmp += SHA256_BLOCK_SIZE;
	}

	if (NULL != hash) {
		memcpy(hash, h, sizeof(BYTE) * 32);
	}
	return 0;
}

unsigned char CheckData[256 + 32] = {
	0x39, 0x9D, 0x75, 0x31, 0x6F, 0x4D, 0xE2, 0x51, 0x16, 0x83, 0x8A, 0x25, 0x00, 0x59, 0xAC, 0xAD,
	0xCE, 0x6C, 0xAA, 0xB3, 0x5F, 0x0A, 0x72, 0x47, 0x94, 0x89, 0x20, 0xF1, 0xD6, 0xF5, 0xBE, 0xC8,
	0xD1, 0xB2, 0xA5, 0x30, 0x2B, 0x6E, 0x01, 0xE2, 0x0E, 0xC0, 0xAB, 0xEB, 0x47, 0x99, 0x58, 0x96,
	0xDD, 0x1D, 0x66, 0x1E, 0xC1, 0x9B, 0xAB, 0x64, 0x30, 0xD6, 0xE9, 0x52, 0x6B, 0xEF, 0x09, 0x01,
	0xB3, 0x32, 0xEA, 0xB9, 0xC7, 0x71, 0x4F, 0x04, 0x0B, 0xE7, 0xBC, 0x1D, 0xC7, 0xEE, 0xCB, 0x39,
	0x58, 0xB5, 0x47, 0xDB, 0xCA, 0x1F, 0xC8, 0xFC, 0xDC, 0xCE, 0x29, 0xBE, 0x6B, 0x99, 0xB1, 0x20,
	0x77, 0x88, 0xAD, 0xAF, 0x60, 0x21, 0xE3, 0x36, 0x09, 0x38, 0x75, 0x2F, 0x01, 0x46, 0xBD, 0x9D,
	0x4C, 0x82, 0x72, 0x1B, 0x85, 0xBE, 0x78, 0x0A, 0x73, 0xA3, 0x00, 0x5C, 0x41, 0x06, 0xA7, 0x07,
	0x3B, 0x81, 0x4A, 0x4D, 0xD2, 0x40, 0x4F, 0xA5, 0x35, 0xDF, 0x58, 0xC6, 0xF6, 0xE3, 0xDC, 0xD4,
	0xAF, 0x61, 0x98, 0x4F, 0x12, 0x8D, 0x4E, 0xA0, 0xB3, 0x0E, 0xB7, 0xF2, 0x11, 0xFF, 0x94, 0x6C,
	0x23, 0xDA, 0x5C, 0x51, 0xA7, 0x9A, 0x7D, 0xD3, 0xDB, 0x65, 0xDF, 0x2C, 0x21, 0x07, 0x64, 0x8B,
	0x1D, 0x58, 0x60, 0x13, 0x64, 0x8E, 0xEF, 0x11, 0x5B, 0x4D, 0x86, 0x9E, 0x67, 0x43, 0x54, 0x70,
	0x47, 0x31, 0x6E, 0x85, 0x10, 0x2D, 0x5A, 0x69, 0x9B, 0xA7, 0x0B, 0xBB, 0xDE, 0x07, 0x94, 0x70,
	0x3D, 0x7D, 0xEE, 0x77, 0x84, 0x44, 0x33, 0x9B, 0xD9, 0xDA, 0xAC, 0xE2, 0xC4, 0x03, 0x93, 0x0D,
	0x00, 0x52, 0x83, 0x82, 0xAE, 0x1F, 0xDF, 0x06, 0xD4, 0x08, 0x0C, 0x67, 0x2F, 0x6A, 0x32, 0x24,
	0x61, 0xE9, 0xF0, 0x69, 0x75, 0xD3, 0x46, 0xE6, 0x9D, 0xE5, 0x69, 0x6C, 0x46, 0x90, 0x60, 0x8B,

	0xA0, 0xCE, 0xE6, 0x1B, 0x65, 0xFB, 0x90, 0x53, 0x46, 0xBA, 0x51, 0x02, 0x66, 0x10, 0xBA, 0xF6,
	0x45, 0x0E, 0xB9, 0x10, 0x9D, 0x39, 0x43, 0xC9, 0x08, 0xA2, 0x38, 0x6E, 0x19, 0x68, 0x0F, 0xBB
}; 

int Chg(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0' + 0;
	}
	if (c >= 'a' && c <= 'z')
	{
		return c - 'a' + 10;
	}
	if (c >= 'A' && c <= 'Z')
	{
		return c - 'A' + 10 + 26;
	}
	if(c == '-')
		return 62;
	if (c == '_')
		return 63;
	return -1;
}

int ChgState(DWORD* State, char *pass)
{
	for (int j = 0; j < 6; j++)
	{
		State[j] = 0;
	}
	for (int i = 0; i < 17; i++)
	{
		int b = Chg(pass[i]);
		if (b == -1)
			return 0;
		for (int j = 0; j < 6; j++)
		{
			State[j] |= (b & 1) << (16 - i);
			b = b / 2;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
	//DWORD s1 = 1;
	//DWORD state = s1;
	INT64 ZhouQi[6] = {0};

	FILE *fp;
	DWORD State[6];
	DWORD Mul[6];
	DWORD Add[6];
	BYTE RandData[256 + 32];
	BYTE *Dec = &RandData[256];
	char flag[32];

	for(int i = 0; i < 6; i++)
	{
		State[i] = 1;
		Mul[i] = 0x143FD + i * 0x200;
		Add[i] = 0x169EC3 + i * 0x11;
	}

#ifdef TRUE
	char password[256];
	printf("Input password: \n");
	//strcpy(password, "Th1s-is__y0urflAG");
	scanf_s("%s", password, 256);
	strcpy(flag, password);
	*LPWORD(&flag[30]) = 0;
	for (int i = 0; i < 30; i += 2)
	{
		*LPWORD(&flag[30]) += *LPWORD(&flag[i]);
	}
	if (strlen(password) != 17)
	{
		printf("Check Failed!\n");
		return 0;
	}
	if(!ChgState(State, password))
	{
		printf("Check Failed!\n");
		return 0;
	}
	/*srand(GetTickCount());
	for(int i = 0; i < 6; i++)
	{
		State[i] = rand() * rand() * rand() * rand();
		State[i] &= 0x1FFFF;
	}
	fp = fopen("flag", "wb");
	fwrite(State, 4, 6, fp);
	fclose(fp);*/
	/*fp = fopen("flag", "rb");
	fread(State, 4, 6, fp);
	fclose(fp);*/
	for(int Cnt = 0; Cnt < 256 + 32; Cnt++)
	{
		BYTE b = 0;
		for(int j = 0; j < 8; j++)
		{
			for(int i = 0; i < 6; i++)
			{
				State[i] = State[i] * Mul[i] + Add[i];
			}
			int s = ((State[4] >> 0x10) & 1) * 2 + ((State[5] >> 0x10) & 1);
			b = b * 2 + ((State[s] >> 0x10) & 1);
		}
		RandData[Cnt] = b;
	}
	/*fp = fopen("data", "wb");
	fwrite(RandData, 1, 256 + 32, fp);
	fclose(fp);*/
	if (memcmp(RandData, CheckData, 256))
	{
		printf("Check Failed!\n");
		return 0;
	}
	BYTE hashOK[32] =
	{
		0x84, 0x8D, 0xF1, 0xCE, 0xDD, 0xF7, 0x17, 0x86, 0xE8, 0x19, 0x7F, 0xD2, 0xFE, 0xE5, 0x33, 0x4E,
		0x30, 0xB9, 0xAD, 0x44, 0xE3, 0x2C, 0xE5, 0x57, 0x42, 0xCE, 0xCE, 0xB7, 0x2B, 0xB5, 0x4A, 0xC2
	};
	BYTE hash[32];
	zsha256(Dec, 32, hash);
	/*fp = fopen("data", "wb");
	fwrite(hash, 1, 32, fp);
	fclose(fp); */
	if (memcmp(hash, hashOK, 32))
	{
		printf("Check Failed!\n");
		return 0;
	}
	for (int i = 0; i < 32; i++)
	{
		//Dec[i] = Dec[i] ^ flag[i];
		flag[i] = Dec[i] ^ CheckData[256 + i];
	}
	fp = fopen("data", "wb");
	fwrite(Dec, 1, 32, fp);
	fclose(fp);
	for (int i = 0; i < 30; i += 2)
	{
		*LPWORD(&flag[30]) -= *LPWORD(&flag[i]);
	}
	if (flag[30] || flag[31])
	{
		printf("Check Failed!\n");
		return 0;
	}
	printf("Flag is: flag{%s}\n", flag);
	system("pause");



#else
	fp = fopen("data", "rb");
	fread(RandData, 1, 256, fp);
	fclose(fp);
	BYTE BitData[2048];
	BYTE BitData1[2048];
	BYTE BitData2[2048];
	for(int i = 0; i < 2048; i++)
	{
		BitData[i] = (RandData[i / 8] >> (7 - (i % 8))) & 1;
	}
	DWORD MaxS[6];
	DWORD MaxR[6] = { 0 };
	for(INT64 S = 0; S < (1LL << 17); S++)
	{
		DWORD R;
		for(int j = 0; j < 4; j++)
		{
			State[j] = S;
			R = 0;
			for(int i = 0; i < 2048; i++)
			{
				State[j] = State[j] * Mul[j] + Add[j];
				if(((State[j] >> 0x10) & 1) == BitData[i])
				{
					R++;
				}
				if(i && ((i % 0x100) == 0))
				{
					if(R * 100 < i * 55)
						break;
				}
			}
			if(R >= MaxR[j])
			{
				//printf("S[%d] = %llX: %d -> %d (%f%%)\n", j, S, MaxR[j], R, R * 100.0 / 2048);
				MaxR[j] = R;
				MaxS[j] = S;
			}
		}
	}
	for(int j = 0; j < 4; j++)
	{
		State[j] = MaxS[j];
	}
	int Num[2] = {0};
	for(int i = 0; i < 2048; i++)
	{
		BYTE Bit[4];
		for(int j = 0; j < 4; j++)
		{
			State[j] = State[j] * Mul[j] + Add[j];
			Bit[j] = (State[j] >> 0x10) & 1;
		}
		BitData1[i] = 2;
		BitData2[i] = 2;
		if(Bit[0] == Bit[1] && Bit[2] == Bit[3])
		{
			if(Bit[0] != Bit[2])
			{
				BitData1[i] = (BitData[i] == Bit[2]);
				Num[0]++;
			}
		}
		if(Bit[0] == Bit[2] && Bit[1] == Bit[3])
		{
			if(Bit[0] != Bit[1])
			{
				BitData2[i] = (BitData[i] == Bit[1]);
				Num[1]++;
			}
		}
	}
	for(INT64 S = 0; S < (1LL << 17); S++)
	{
		DWORD R;
		for(int j = 4; j < 6; j++)
		{
			State[j] = S;
			R = 0;
			for(int i = 0; i < 2048; i++)
			{
				State[j] = State[j] * Mul[j] + Add[j];
				if( j == 4)
				{
					if(((State[j] >> 0x10) & 1) == BitData1[i])
					{
						R++;
					}
				}
				else
				{
					if(((State[j] >> 0x10) & 1) == BitData2[i])
					{
						R++;
					}
				}

			}
			if(R >= MaxR[j])
			{
				//printf("S[%d] = %llX: %d -> %d (%f%%)\n", j, S, MaxR[j], R, R * 100.0 / 2048);
				MaxR[j] = R;
				MaxS[j] = S;
			}
		}
	}
	for(int j = 0; j < 6; j++)
	{
		if(j < 4)
			printf("S[%d] = %X: %d (%f%%)\n", j, MaxS[j], MaxR[j], MaxR[j] * 100.0 / 2048);
		else if(j == 4)
			printf("S[%d] = %X: %d (%f%%)\n", j, MaxS[j], MaxR[j], MaxR[j] * 100.0 / Num[0]);
		else
			printf("S[%d] = %X: %d (%f%%)\n", j, MaxS[j], MaxR[j], MaxR[j] * 100.0 / Num[1]);
	}
#endif
	/*for(INT64 S = 0; S < (1LL << 32); S++)
	{
		for(int i = 0; i < 6; i++)
		{
			State[i] = State[i] * Mul[i] + Add[i];
			if(State[i] == 1)
			{
				if(ZhouQi[i] == 0)
				{
					ZhouQi[i] = S;
					printf("%d: %llX(%lld)\n", i, S, S);
				}
			}
		}
	}*/
	/*do
	{
		ZhouQi++;
		//state = state * 0x143FD + 0x169EC3;
		//state = state * 0x343FD + 0x269EC3;
		//state = state * 0x543FD + 0x369EC3;
		//state = state * 0x743FD + 0x469EC3;
		//state = state * 0x943FD + 0x569EC3;
		state = state * 0xA43FD + 0x669EC3;
	}
	while(state != s1);
	printf("%llX(%lld)\n", ZhouQi, ZhouQi);*/
	return 0;
}

