namespace base64
{
	unsigned char BASE64DECODE[256] =
	{
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3e,0xff,0xff,0xff,0x3f,
		0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0xff,0xff,0xff,0x40,0xff,0xff,
		0xff,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,
		0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0xff,0xff,0xff,0xff,0xff,
		0xff,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
		0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
	};

	void encode(const unsigned char* in,int inlen,unsigned char* out)
	{
		const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		for (; inlen >= 3; inlen -= 3)
		{
			*out++ = base64digits[in[0] >> 2];
			*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
			*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
			*out++ = base64digits[in[2] & 0x3f];
			in += 3;
		}
		if (inlen > 0)
		{
			unsigned char fragment;

			*out++ = base64digits[in[0] >> 2];
			fragment = (in[0] << 4) & 0x30;
			if (inlen > 1)
				fragment |= in[1] >> 4;
			*out++ = base64digits[fragment];
			*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
			*out++ = '=';
		}
		*out = '\0';
	}

	unsigned char *decode(char *base64code, int base64length, unsigned char *out, int &outlen)
	{
		unsigned char *pCode = (unsigned char *)base64code;
		unsigned char *pBegin = out;
		unsigned char *pEnd = out + outlen;  
		unsigned char uCode;
		for (int i = 0; (i < base64length) && (out < pEnd); i++)
		{
			uCode = BASE64DECODE[pCode[i]];
			if (uCode >= 64)
				break;
			switch( i%4 )
			{
				case 0:
					*out = uCode<<2;
					break;
				case 1:
					*out = *out | (uCode >> 4);
					out++;
					if (out >= pEnd)
						break;
					*out = uCode << 4;
					break;
				case 2:
					*out = *out | (uCode >> 2);
					out++;
					if (out >= pEnd)
						break;
					*out = uCode << 6;
					break;
				case 3:
					*out = *out | uCode;
					out++;
					break;
			}
		}
		outlen = out - pBegin;
		return pBegin;
	}
}