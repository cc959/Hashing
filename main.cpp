#include <bits/stdc++.h>

#define ull uint64_t
#define uint uint32_t
#define ushort uint16_t
#define uchar uint8_t

using namespace std;

// first 32 bits of the fractional parts of the square roots of the first 8 primes: 2, 3, 5, 7, 11, 13, 17, 19
vector<uint> h = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

// first 32 bits of the fractional parts of the cube roots of the first 64 primes (2 – 311)
vector<uint> k = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
				  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
				  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
				  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
				  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
				  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
				  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
				  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// right -> positive  left -> negative
template <class T>
T rotate(T in, int d)
{
	// d %= 8 * sizeof(T);
	return (in >> d) | (in << (8 * sizeof(T) - d));
}

template <class T, class U>
vector<U> convert(vector<T> in)
{
	auto a = sizeof(T);
	auto b = sizeof(U);

	if (a >= b)
	{
		int m = a / b;
		int n = in.size() * m;
		vector<U> out(n);

		int i = 0;
		for (int k = 0; k < in.size(); k++)
			for (int j = 0; j < m; j++, i++)
				out[n - i - 1] = U(in[k] / (1LL << 8 * b * i) % (1LL << 8 * b));

		return out;
	}
	else
	{
		int m = b / a;

		while (in.size() % m)
			in.push_back(0);

		int n = in.size() / m;

		vector<U> out(n);

		int i = 0;
		for (int k = 0; k < n; k++)
			for (int j = 0; j < m; j++, i++)
			{
				out[k] |= U(in[i]) << (8 * a * (m - j - 1));
			}

		return out;
	}
}

int main()
{
	vector<uchar> data;
	char c;
	while (cin.get(c))
		data.push_back(reinterpret_cast<uchar &>(c));

	ull size = data.size() * 8;

	data.push_back(0b1000000);

	while ((data.size() + 8) % 64)
		data.push_back(0b00000000);

	auto csize = convert<ull, uchar>({size});
	for (int i = 0; i < 8; i++)
		data.push_back(csize[i]);

	for (int i = 0; i < data.size(); i += 64)
	{
		vector<uint> w = convert<uchar, uint>(vector<uchar>(data.begin() + i, data.begin() + i + 64));
		w.resize(64, 0);

		for (int j = 16; j < 64; j++)
		{
			uint s0 = rotate(w[j - 15], 7) ^ rotate(w[j - 15], 18) ^ (w[j - 15] >> 3);
			uint s1 = rotate(w[j - 2], 17) ^ rotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
			w[j] = w[j - 16] + s0 + w[j - 7] + s1;
		}

		auto a = h;

		for (int j = 0; j < 64; j++)
		{
			auto s0 = rotate(a[0], 2) ^ rotate(a[0], 13) ^ rotate(a[0], 22);
			auto s1 = rotate(a[4], 6) ^ rotate(a[4], 11) ^ rotate(a[4], 25);
			auto ch = (a[4] & a[5]) ^ ((~a[4]) & a[6]);
			auto temp1 = a[7] + s1 + ch + k[j] + w[j];
			auto maj = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]);
			auto temp2 = s0 + maj;

			a[7] = a[6];
			a[6] = a[5];
			a[5] = a[4];
			a[4] = a[3] + temp1;
			a[3] = a[2];
			a[2] = a[1];
			a[1] = a[0];
			a[0] = temp1 + temp2;
		}

		for (int i = 0; i < 8; i++)
			h[i] += a[i];
	}

	for (int i = 0; i < 8; i++)
		cout << bitset<32>(h[i]);
	cout << "\n";
}