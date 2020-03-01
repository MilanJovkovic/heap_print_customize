// heaptest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>

static const char dashes[2 * sizeof(long) + 5] = {
	'-'
};

#  define ERTS_AT_LEAST_GCC_VSN__(MAJ, MIN, PL) 0

#if ERTS_AT_LEAST_GCC_VSN__(3, 0, 4)
#  define erts_align_attribute(SZ) __attribute__ ((aligned (SZ)))
#else
#  define erts_align_attribute(SZ)
#endif

typedef unsigned long long Eterm erts_align_attribute(sizeof(long long));

bool is_arity_value(Eterm val) {
	if (val != 0)
		return true;
	return false;
}

long arityval(Eterm val) {
	return 1;
}

bool is_thing(Eterm val) {
	if (val != 0)
		return true;
	return false;
}

int thing_arityval(Eterm val) {
	return 2;
}

long thing_subtag(Eterm val) {
	return 3;
}

void erts_printf(const char* format, ...) {
	printf(format);
}

static void print_heap(Eterm* pos, Eterm* end)
{
	printf("From: 0x%0*lx to 0x%0*lx\n\r",
		2 * (int)sizeof(long), (unsigned long)pos,
		2 * (int)sizeof(long), (unsigned long)end);
	printf(" | %*s H E A P %*s |\r\n",
		2 * (int)sizeof(long) - 1, "",
		2 * (int)sizeof(long) - 1, "");
	printf(" | %*s | %*s |\r\n",
		2 + 2 * (int)sizeof(long), "Address",
		2 + 2 * (int)sizeof(long), "Contents");
	printf(" |%s|%s|\r\n", dashes, dashes);
	while (pos < end) {
		Eterm val = pos[0];
		printf(" | 0x%0*lx | 0x%0*lx | ",
			2 * (int)sizeof(long), (unsigned long)pos,
			2 * (int)sizeof(long), (unsigned long)val);
		++pos;
		if (is_arity_value(val))
			printf("Arity(%lu)", arityval(val));
		else if (is_thing(val)) {
			unsigned int ari = thing_arityval(val);
			printf("Thing Arity(%u) Tag(%lu)", ari, thing_subtag(val));
			while (ari) {
				printf("\r\n | 0x%0*lx | 0x%0*lx | THING",
					2 * (int)sizeof(long), (unsigned long)pos,
					2 * (int)sizeof(long), (unsigned long)*pos);
				++pos;
				--ari;
			}
		}
		else {
			fflush(stdout);
			erts_printf("%.30T", val);
		}
		printf("\r\n");
	}
	printf(" |%s|%s|\r\n", dashes, dashes);
}

static char* print_heap_custom(Eterm* pos, Eterm* end)
{
	//char* position = new char[(end-pos)*100+1000];
	char position[100000];
	int offset;

	offset = sprintf_s(position, sizeof(position), "From: 0x%0*lx to 0x%0*lx\n\r",
		2 * (int)sizeof(long), (unsigned long)pos,
		2 * (int)sizeof(long), (unsigned long)end);
	offset += sprintf_s(position + offset, sizeof(position) - offset, " | %*s H E A P %*s |\r\n",
		2 * (int)sizeof(long) - 1, "",
		2 * (int)sizeof(long) - 1, "");
	offset += sprintf_s(position + offset, sizeof(position) - offset, " | %*s | %*s |\r\n",
		2 + 2 * (int)sizeof(long), "Address",
		2 + 2 * (int)sizeof(long), "Contents");
	offset += sprintf_s(position + offset, sizeof(position) - offset, " |%s|%s|\r\n", dashes, dashes);
	while (pos < end) {
		Eterm val = pos[0];
		offset += sprintf_s(position + offset, sizeof(position) - offset, " | 0x%0*lx | 0x%0*lx | ",
			2 * (int)sizeof(long), (unsigned long)pos,
			2 * (int)sizeof(long), (unsigned long)val);
		++pos;
		if (is_arity_value(val))
			offset += sprintf_s(position + offset, sizeof(position) - offset, "Arity(%lu)", arityval(val));
		else if (is_thing(val)) {
			unsigned int ari = thing_arityval(val);
			offset += sprintf_s(position + offset, sizeof(position) - offset, "Thing Arity(%u) Tag(%lu)", ari, thing_subtag(val));
			while (ari) {
				offset += sprintf_s(position + offset, sizeof(position) - offset, "\r\n | 0x%0*lx | 0x%0*lx | THING",
					2 * (int)sizeof(long), (unsigned long)pos,
					2 * (int)sizeof(long), (unsigned long)*pos);
				++pos;
				--ari;
			}
		}
		else {
			fflush(stdout);
			erts_printf("%.30T", val);
		}
		offset += sprintf_s(position + offset, sizeof(position) - offset, "\r\n");
	}
	sprintf_s(position + offset, sizeof(position) - offset, " |%s|%s|\r\n", dashes, dashes);
	return position;
}

// void hipe_print_heap(Process *p)
// {
//     print_heap(p->heap, p->htop);
// }

int main()
{
	Eterm* pos = new Eterm;
	Eterm* end = pos + 10;
	print_heap(pos, end);
	char* result = print_heap_custom(pos, end);
	printf("\nCustom result:\n%s", result);
	std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
