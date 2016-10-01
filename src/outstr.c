#include "ioport.h"
#include "memory.h"
#include "ints.h"

#include <stdint.h>

void set_div_to_write() {
	out8(FIRST + 1, 0);
	out8(FIRST + 3, 1 << 7);
	out8(FIRST, 1);
	out8(FIRST + 3, 3);
}

void write_char(char *ch) {
	out8(FIRST, (uint8_t) *ch);
}

int get_permission_to_write() {
	return (in8(FIRST + 5) & (1 << 5));
}

void write_str(char *str){
	char *i = str;
	while(*i != 0) {
		while (get_permission_to_write() == 0);
		write_char(i);
		i++;
	}
}

void write_string(char *str) {
	//disable_ints();
	set_div_to_write();
	write_str(str);
}	

void write_number(uint8_t num) {
    char word[9];
    for (int i = 7; i >= 0; i--) {
        word[i] = '0' + num % 2;
        num /= 2;
    }
    word[8] = 0;
    write_string(word);
}