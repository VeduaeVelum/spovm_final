#include <stdio.h>
#include <conio.h>
#include <dos.h>

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();                 //get data from registers
void print(int);                //fill in the screen

//IR0-7
//void interrupt (*int8) (void);
//void interrupt (*int9) (void);
//void interrupt(*int10) (void);
//void interrupt(*int11) (void);
//void interrupt(*int12) (void);
//void interrupt(*int13) (void);
//void interrupt(*int14) (void);
//void interrupt(*int15) (void);
//void interrupt(*int70) (void);
//void interrupt(*int71) (void);
//void interrupt(*int72) (void);
//void interrupt(*int73) (void);
//void interrupt(*int74) (void);
//void interrupt(*int75) (void);
//void interrupt(*int76) (void);
//void interrupt(*int77) (void);

//void interrupt int70(void) { get_reg(); int8(); }
//void interrupt int71(void) { get_reg(); int9(); }
//void interrupt int72(void) { get_reg(); int10(); }
//void interrupt int73(void) { get_reg(); int11(); }
//void interrupt int74(void) { get_reg(); int12(); }
//void interrupt int75(void) { get_reg(); int13(); }
//void interrupt int76(void) { get_reg(); int14(); }
//void interrupt int77(void) { get_reg(); int15(); }
//void interrupt int08(void) { get_reg(); int70(); }
//void interrupt int09(void) { get_reg(); int71(); }
//void interrupt int0A(void) { get_reg(); int72(); }
//void interrupt int0B(void) { get_reg(); int73(); }
//void interrupt int0C(void) { get_reg(); int74(); }
//void interrupt int0D(void) { get_reg(); int75(); }
//void interrupt int0E(void) { get_reg(); int76(); }
//void interrupt int0F(void) { get_reg(); int77(); }

void get_reg()
{
	outport(0x20, 0x0B);
	outport(0xA0, 0x0B);
	print(inp(0x20));
	print(inp(0xA0));

	outport(0x20, 0x0A);
	outport(0xA0, 0x0A);
	print(inp(0x20));
	print(inp(0xA0));

	print(inp(0x21));
	print(inp(0xA1));

}

void print(int val)
{
	char temp;
	static int count = 0x000;
	int i;
	struct VIDEO far* screen;
	screen = (struct VIDEO far*)MK_FP(0xB800, count);
	for (i = 7; i >= 0; i--)
	{
		temp = val %2;
		val /= 2;
		screen->symb = temp+'0';
		screen->attr = 0x5E;
		screen++;
	}
	count += 0x0A0;
}

void init()
{
	//int8 = getvect(0x08);
	//int9 = getvect(0x09);
	//int10 = getvect(0x0A);
	//int11 = getvect(0x0B);
	//int12 = getvect(0x0C);
	//int13 = getvect(0x0D);
	//int14 = getvect(0x0E);
	//int15 = getvect(0x0F);
	///*int70 = getvect(0x70);
	//int71 = getvect(0x71);
	//int72 = getvect(0x72);
	//int73 = getvect(0x73);
	//int74 = getvect(0x74);
	//int75 = getvect(0x75);
	//int76 = getvect(0x76);
	//int77 = getvect(0x77);
	//
	//setvect(0x08, int8);  
	//setvect(0x09, int9);
	//setvect(0x10, int10);
	//setvect(0x11, int11);
	//setvect(0x12, int12);
	//setvect(0x13, int13);
	//setvect(0x14, int14);
	//setvect(0x15, int15);*/
	//setvect(0x70, int70);			//дальше
	//setvect(0x71, int71);
	//setvect(0x72, int72);
	//setvect(0x73, int73);
	//setvect(0x74, int74);
	//setvect(0x75, int75);
	//setvect(0x76, int76);
	//setvect(0x77, int77);

	_disable();

	outport(0x20, 0x11);
	outport(0x21, 0x08);
	outport(0x21, 0x04);
	outport(0x21, 0x01);

	outport(0xA0, 0x11);
	outport(0xA1, 0x70);
	outport(0xA1, 0x02);
	outport(0xA1, 0x01);

	outport(0x20, 0x6B);
	outport(0xA0, 0x6B);

	_enable();
}

int main()
{
	unsigned far *fp;
	init();
	get_reg();

	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);

	_dos_keep(0,(_DS - _CS)+(_SP/16)+1);
	return 0;
}
//---------------------------------------------------------------------------
