/*============================================================================*/
#include "g2stdioRemap.h"
#include "stm32f4xx.h"
/*============================================================================*/
int i = 0, j = 0, k = 0;
void f1(char c)
{
	i+= c;
}

void f2(char c)
{
	j+= c;
}
void f3(char c)
{
	k-= c;
}
/*============================================================================*/
int main(void)
{
	FILE *a, *b, *c;
	
	initializeStdio();
	
	addStdioEntry("pierwszy", f1, NULL);
	addStdioEntry("drugi", f2, NULL);
	addStdioEntry("trzeci", f3, NULL);
	
	a = fopen("pierwszy", "w+");
	b = fopen("drugi", "w+");
	c = fopen("trzeci", "w+");
	
	printf("Witaj swiecie :)\n");
	
	fprintf(a, "Hello :)");
	fprintf(b, "Hello world :)");
	fprintf(c, "Witaj swiecie haha");
	
	printf("Wartosci zmiennych:\ni = %d\nj = %d\nk = %d\n", i, j, k);
	
	return 88;
}
/*============================================================================*/
/*                                    EOF                                     */
/*============================================================================*/
