#include <config.h>
#include <crc.h> 
int main() {
	return !crc32("hello", 5);
}
