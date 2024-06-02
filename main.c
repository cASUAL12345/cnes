#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cpu.h"

int load_file(char *filename, uint16_t start_address) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        perror("Error opening file");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size > 0x10000) {
        fprintf(stderr, "File size is too big.\n");
        fclose(file);
        return 0;
    }

    fseek(file, 0, SEEK_SET);
    fread(&MEMORY[start_address], 1, file_size, file);

    fclose(file);

    return 1;
}

int main(int argc, char **argv)
{

	if(argc < 2 || argc > 4)
	{
		printf("Usage: ./nes <filename> [start_address] [start_pc]\n");
		return -1;
	}

	uint16_t start = 0;

	if(argc == 3)
		start = (uint16_t) strtoul(argv[2], NULL, 0);

	if(load_file(argv[1], start) == 0)
	{
		return -1;
	}

	uint16_t addrs = 0x1000;

	for(int i = 0; i < 0xffff; i++)
	{
		if(MEMORY[i] == 0x4C)
		{
			addrs = i;
			break;
		}
	}

	printf("0x%04X: 0x%02X\n", addrs, MEMORY[addrs]);

	printf("reset: %02X%02X\n", MEMORY[RESET_ADDRESS], MEMORY[RESET_ADDRESS + 1]);

	scanf("%d");

	CPU cpu;

	cpu_init(&cpu);

	if(argc == 4)
		cpu.pc = (uint16_t) strtoul(argv[3], NULL, 0);

	printf("pc reset: %04X\n", cpu.pc);

	for(;;)
	{
		cycle(&cpu);
	}

	return 0;
}
