/*
Implementation by Shuohang Peng 

Thanks for Ronny Van Keer's pioneering implementation of the Xoodyak C code,
our Xoodyak software protection implementation was adapted from his/her reference code. 

The reference link for the original C code is as follows:
https://csrc.nist.gov/Projects/lightweight-cryptography/finalists
*/

#include "StrOp.h"

void StrToBytes(uint8_t *data, char *str, uint8_t strlen)
{
    int i;
	
	for(i = 0; i < strlen; i++)
	{
		switch(str[i]){
			case '0' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x0;}
					   else{data[i / 2] = data[i / 2] | (0x0 << 4);}break;
		    case '1' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x1;}
					   else{data[i / 2] = data[i / 2] | (0x1 << 4);}break;
			case '2' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x2;}
					   else{data[i / 2] = data[i / 2] | (0x2 << 4);}break;
			case '3' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x3;}
					   else{data[i / 2] = data[i / 2] | (0x3 << 4);}break;
			case '4' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x4;}
					   else{data[i / 2] = data[i / 2] | (0x4 << 4);}break;
		    case '5' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x5;}
					   else{data[i / 2] = data[i / 2] | (0x5 << 4);}break;
			case '6' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x6;}
					   else{data[i / 2] = data[i / 2] | (0x6 << 4);}break;
			case '7' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x7;}
					   else{data[i / 2] = data[i / 2] | (0x7 << 4);}break;
			case '8' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x8;}
					   else{data[i / 2] = data[i / 2] | (0x8 << 4);}break;
		    case '9' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0x9;}
					   else{data[i / 2] = data[i / 2] | (0x9 << 4);}break;
			case 'a' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0xa;}
					   else{data[i / 2] = data[i / 2] | (0xa << 4);}break;
			case 'b' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0xb;}
					   else{data[i / 2] = data[i / 2] | (0xb << 4);}break;
			case 'c' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0xc;}
					   else{data[i / 2] = data[i / 2] | (0xc << 4);}break;
		    case 'd' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0xd;}
					   else{data[i / 2] = data[i / 2] | (0xd << 4);}break;
			case 'e' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0xe;}
					   else{data[i / 2] = data[i / 2] | (0xe << 4);}break;
			case 'f' : if((i % 2) != 0){data[i / 2] = (data[i / 2] & 0xf0) | 0xf;}
					   else{data[i / 2] = data[i / 2] | (0xf << 4);}break;
			default : data[i / 2] = 0;break;
		}
	}
}

void BytesToStr(char *str, uint8_t *data, uint8_t dlen)
{
    int i, j;
	uint8_t temp = 0;
	
	for(i = 0; i < dlen; i++)
	{
		temp = data[i];
		for(j = 0; j < 2; j++)
		{
			switch((temp >> 4) & 0xf){
				case 0x0 : str[2 * i + j] = '0';break;
				case 0x1 : str[2 * i + j] = '1';break;
				case 0x2 : str[2 * i + j] = '2';break;
				case 0x3 : str[2 * i + j] = '3';break;
				case 0x4 : str[2 * i + j] = '4';break;
				case 0x5 : str[2 * i + j] = '5';break;
				case 0x6 : str[2 * i + j] = '6';break;
				case 0x7 : str[2 * i + j] = '7';break;
				case 0x8 : str[2 * i + j] = '8';break;
				case 0x9 : str[2 * i + j] = '9';break;
				case 0xa : str[2 * i + j] = 'a';break;
				case 0xb : str[2 * i + j] = 'b';break;
				case 0xc : str[2 * i + j] = 'c';break;
				case 0xd : str[2 * i + j] = 'd';break;
				case 0xe : str[2 * i + j] = 'e';break;
				case 0xf : str[2 * i + j] = 'f';break;
				default : str[2 * i + j] = 'z';break;
			}
			temp = temp << 4;
		}
	}
	str[2 * dlen] = '\n';
	str[2 * dlen + 1] = '\0';
}
