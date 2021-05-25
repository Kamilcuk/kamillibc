#include <config.h>
#include <eealloc.h> 
int main() {
	void *a = eemalloc(1);
	free(a);
}
