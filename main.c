int powe(int val, int p);
int IntToStr(char* string, int val);
int StrToInt(char* string, int len);
char GetSeconds(void);
char GetKey(void);
char* GetVideoMem(void);
void SetPix(char *addr, int x, int y, int size, int screen_width, unsigned char r, unsigned char g, unsigned char b);
void PrintChar(char *addr, char *addr_char, int x, int y, int size, int screen_width, unsigned char r, unsigned char g, unsigned char b, int memory_char_height, int memory_char_width);
void PrintStr(char *addr, char* string, char *font_addr, int x, int y, int size, int screen_width, unsigned char r, unsigned char g, unsigned char b, int memory_char_height, int memory_char_width);
void PrintBackground(char* addr, int h, int w, unsigned char r, unsigned char g, unsigned char b);
void Animation(char *addr, int w, int h);


int start()
{
	char *VideoMem = 0x0;
	VideoMem = GetVideoMem();

	char *FontMem = (char*)0xD000;

	char *str[3];
	str[0] = (char*)0xE700;
	str[1] = (char*)0xE800;
	str[2] = (char*)0xE900;
	int strp[2] = {-1, -1};
	int current_str=0;
	int current_operation=0;
	int color = 0;
	int s1int, s2int, rez;
	int str_marer_pos = 60;

	int screen_height = 480;
	int screen_width = 640;

	int memory_char_height = 5;
	int memory_char_width = 3;
	int char_size = 10;

	char ScanCode = 0;

	int i;

	Animation(VideoMem, screen_width, screen_height);

	while(1)
	{
		ScanCode = GetKey();

		if(ScanCode > 0)
		{
			PrintBackground(VideoMem, screen_height, screen_width, 0, 0, 0);

			for(i = 0; i < 50; ++i)
			{
				str[2][i] = 0;
			}
		}

		if((ScanCode >= 0x2) && (ScanCode <= 0x0B) && ((strp[current_str] + 1)*char_size*4 < 640))
		{
			strp[current_str]++;
			str[current_str][strp[current_str]] = ScanCode;
		}


		if(ScanCode == 0x53)
		{
			if(strp[current_str] >= 0)
			{
				str[current_str][strp[current_str]] = 0;
				strp[current_str]--;
			}
		}


		if(ScanCode == 0x48) // up
		{
			if(current_str > 0)
				current_str--;

			str_marer_pos = 60;
		}


		if(ScanCode == 0x50) // down
		{
			if(current_str < 1)
				current_str++;

			str_marer_pos = 260;
		}


		if(ScanCode == 0x4D) // right
		{
			if(current_operation < 3)
			current_operation++;
		}


		if(ScanCode == 0x4B) // left
		{
			if(current_operation > 0)
				current_operation--;
		}
		

		PrintStr(VideoMem, &str[0][0], FontMem, 0, 0, char_size, screen_width, 255, 255, 0, memory_char_height, memory_char_width); //str1

		PrintStr(VideoMem, &str[1][0], FontMem, 0, 200, char_size, screen_width, 255, 255, 0, memory_char_height, memory_char_width); //str2
		
		PrintStr(VideoMem, &str[2][0], FontMem, 0, 400, char_size, screen_width, 255, 255, 0, memory_char_height, memory_char_width); //str3

		
		for(i = 0; i < screen_width/4; ++i)
		{
			SetPix(VideoMem, i*4, str_marer_pos, 4, screen_width, 0, 0, 255);
		}

		if(current_operation == 0)
		{
			if(s2int != 0)
			{
				rez = s1int / s2int;
			}
			color = 0;
		}
		PrintChar(VideoMem, &FontMem[13*3*5 + 6*6],         160, 100, char_size, screen_width, color, 255, 0, 5, 5); // /
		color = 255;

		if(current_operation == 1)
		{
			rez = s1int * s2int;
			color = 0;
		}
		PrintChar(VideoMem, &FontMem[13*3*5 + 6*6 + 5*5],   240, 100, char_size, screen_width, color, 255, 0, 5, 5); // *
		color = 255;

		if(current_operation == 2)
		{
			rez = s1int + s2int;
			color = 0;
		}
		PrintChar(VideoMem, &FontMem[11*3*5],               320, 90, char_size+5, screen_width, color, 255, 0, memory_char_height, memory_char_width); // +
		color = 255;

		if(current_operation == 3)
		{
			rez = s1int - s2int;
			color = 0;
		}
		PrintChar(VideoMem, &FontMem[10*3*5],               400, 90, char_size+5, screen_width, color, 255, 0, memory_char_height, memory_char_width); // -
		color = 255; 


		s1int = StrToInt(str[0], strp[0]+1);
		s2int = StrToInt(str[1], strp[1]+1);
		IntToStr(&str[2][0], rez);
	}
}



int powe(int val, int p)
{
	int i;
	int ret = val;

	if(p == 0)
		return 1;

	p--;

	for(i = 0; i < p; ++i)
	{
		val = ret*val;
	}

	return val;
}


int StrToInt(char* string, int len)
{
	int i;
	int ret = 0;
	int buff;

	for(i = 0; i < len; ++i)
	{
		buff = string[i];

		if(buff == 0x0B)
		{
			buff = 0;
		}
		else
		{
			buff--;
		}

		ret += (buff)*powe(10, len-i-1);
	}

	return ret;
}


int IntToStr(char* string, int val)
{
	int i = 0, j = 0;
	int buff, len;

	while((float)val / powe(10, i) >= 1)
	{
		i++;
	}

	len = i;

	string[0] = 0x0B;

	while(i > 0)
	{
		i--;
		buff = val / powe(10, i);
		val = val - (buff*powe(10, i));
		string[j] = buff;
		j++;
	}

	for(i = 0; i < len; ++i)
	{
		if(string[i] == 0)
		{
			string[i] = 0x0B;
		}
		else
		{
			string[i] += 1;
		}
	}

	return len;
}


char GetSeconds(void)
{
	asm("mov al, 0");
	asm("out 0x70, al");
    asm("in al, 0x71");
}


char GetKey(void)
{
	asm("mov al, 0b0100110");
	asm("out 0x60, al");
	asm("in al, 0x60");
}


char* GetVideoMem(void)
{
	asm("mov eax, esi");
}


void PrintBackground(char* addr, int h, int w, unsigned char r, unsigned char g, unsigned char b)
{
	int i;
	int end = h*w*3;

	for(i = 0; i < end-3; i += 3)
	{
		addr[i] = b;
		addr[i+1] = g;
		addr[i+2] = r;
	}
}


void PrintStr(char *addr, char* string, char *font_addr, int x, int y, int size, int screen_width, unsigned char r, unsigned char g, unsigned char b, int memory_char_height, int memory_char_width)
{
	int i;
	int CharPos = 0;


	for(i = 0; string[i] != 0; ++i)
	{
		if((string[i] >= 0x2) && (string[i] <= 0x0B))
		{
			CharPos = memory_char_height*memory_char_width*(string[i] - 2);
		}

		PrintChar(addr, &font_addr[CharPos], x+i*(memory_char_width+1)*size, y, size, screen_width, r, g, b, memory_char_height, memory_char_width);
	}
}




void PrintChar(char *addr, char *addr_char, int x, int y, int size, int screen_width, unsigned char r, unsigned char g, unsigned char b, int memory_char_height, int memory_char_width)
{
	int i, j;

	for(i = 0; i < memory_char_height; ++i)
	{	
		for(j = 0; j < memory_char_width; ++j)
		{
		 	if(addr_char[i*memory_char_width + j] == 1)
			{
				SetPix(addr, x+j*size, y+i*size, size, screen_width, r, g, b);
			}
		}
	}
}



void SetPix(char *addr, int x, int y, int size, int screen_width, unsigned char r, unsigned char g, unsigned char b)
{
	int pos;
	int i, j;

	for(i = 0; i < size; ++i)
		for(j = 0; j < size; ++j)
		{
			pos = (x + i)*3 + (y + j)*screen_width*3;
			addr[pos]= b;
			addr[pos+1]= g;
			addr[pos+2]= r;
		}
}




void Animation(char *addr, int w, int h)
{
	int i, j;
	int time;

	for(i = 0; i < w; i += 140)
	{
		time = GetSeconds();
		while(time == GetSeconds())
		{}

		for(j = 0; j < 7; ++j)
		{
			SetPix(addr, i + j*20, h/2, 15, w, 0, 0, 255);
		}	
	}

	PrintBackground(addr, h, w, 0, 0, 0);
}
