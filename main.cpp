#include <cstdio>
#include <cstdlib>


int main(int argc, char* argv[]) {

	for (size_t i = 0; i < argc; i++) {
		printf(argv[i]);
		printf("\n");
	}

	system("pause");
	return 0;
}