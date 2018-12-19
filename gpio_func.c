#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "ls1c_regs.h"

int main(int argc, char *argv[])
{
	uint32_t * map_base;
	FILE *f;
	int n, fd;
	uint32_t addr_base;
	uint16_t addr_offset,addr;
	uint8_t func,gpio,i,reg,bit;
	if(argc!=3) {
		printf("gpio_func 4 85 ;set gpio85 to function 4 \r\n");
		return (-2);
	}
	addr_base=0x1fd01000;
	addr_offset=0x1c0;
	func=atoi(argv[1]);
	gpio=atoi(argv[2]);
	reg=(uint8_t)gpio/32;
	bit=gpio%32;
	if(func>5) {
		printf("func <=5 \r\n");
		return(-3);
	}
	if(gpio>=128) {
		printf("gpio <128 \r\n");
		return(-4);
	}

	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd == -1)  {
		return (-1);
	}
	/* 把xxxxx000开始0x1000字节，映射到map_base */
	map_base = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_base);

	if (map_base == 0)  {
		printf("NULL pointer!\n");
	}
	if(func>0) {
		for(i=1;i<func;i++) //disable 前面的func功能
			map_base[(addr_offset+(i-1)*0x10)/sizeof(uint32_t)+reg] &= ~(1<<bit);
		addr=(addr_offset+(func-1)*0x10)/sizeof(uint32_t)+reg;
		printf("[%08x]:%08x",addr_base+addr*sizeof(uint32_t),map_base[addr]);
		map_base[addr] |= (1<<bit);
		printf("=>%08x,bit%d set,%08x",map_base[addr],bit,1<<bit);


	}else {
		for(i=1;i<6;i++) //disable 全部功能
			map_base[(addr_offset+i*0x10)/sizeof(uint32_t)+reg] &= ~(1<<bit);

	}

	close(fd);
	munmap(map_base, 0xff);
	return (0);
}
