#ifndef __base64__hpp__
#define __base64__hpp__
namespace base64
{
	void encode(const unsigned char* in,int inlen,unsigned char* out);
	unsigned char *decode(char *base64code, int base64length, unsigned char *out, int &outlen);
}
#endif

