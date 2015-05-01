#!/usr/bin/quixey

int i, j, k;
int ret = 5, l = 1 | 2;
int G = ret + 10; // globals can reference previously declared globals!

//-----------------------------------------------------------------------------
// Name: func
//-----------------------------------------------------------------------------
int func() {
	int i = 2;
	do {
		int i = 1;
		{
			printf("TESING LOCALS 1 = %d\n", i);
		}
	} while(0);
	printf("TESING LOCALS 2 = %d\n", i);
}

//-----------------------------------------------------------------------------
// Name: test
//-----------------------------------------------------------------------------
int test(int a, int b, char c, string d) {
	printf("%d,%d,%c,%s\n", a, b, c, d);
}

//-----------------------------------------------------------------------------
// Name: main
//-----------------------------------------------------------------------------
int main() {


	printf("%d\n", G);

	test(5, 10, 'A', "TESTING");

	int i, j;
	int a, b;
	int d = 0x1234;
	int q = 0xffffffff;
	
	printf("%d\n", q);
	
	char z = '\x12345678';
	
	printf("z = %x\n", z);
	
	char ch = '\xff';
	ch += 1;
	printf("%d %%\n", ch);
	
	func();
	
	printf("CHAR TEST '%c'\n", 'A');
	
	if(0) {
		// we should be able to report this non-existant function call
		// but we can't because we only check for validity of syntactically
		// correct code when we execute the code... which never gets reached
		// here.
		// we could add a second pass which would check for calls to functions
		// which don't exist
		fun1234();
	} else {
	}


	
	i = -(-5);

	printf("%d\n", i);
	
	
	printf("z = %x\n", z);
					
	puts("tesing escape sequences: \'\"\\\a\b\f\n\t\v");

	// this would cause a duplicate declaration error if it were uncommented
	// can't have two locals of the same name in the same scope!
	// int i;

	printf("d = %d\n\n", d);
		
	if(d == 1) {
		puts("d is 1!");
	} else if(d == 0) {
		puts("d is 0!");
	} else {
		puts("d is something else!");
	}
	
	// should support arbitrary sub blocks for better scope definition
	{
		{
			{
				{
				
					// output a title
					puts("RPGScript Demo\n");
				}
			}
		}
	}
	
	// test declaration inside a loop
	// this should effectivly declare and destroy 
	// the local var e 1000 times
	for(d = 0; d < 1000; d += 1) {
		int e;
	}
	
	// by the way, you can intersperse declarations and code
	// it is actually harder to enforce them being first then just allowing it..
	int c;
	
	// print out alphabet
	print_alpha();
	
	puts("testing testing 123");
	puts("testing assignment");
	
	a = 2;
	b = 6;

	printf("a == %d\n", a);
	printf("b == %d\n\n", b);
	
	puts("testing left shift operator");
	a <<= 3;
	printf("[a <<= 3] == %d\n\n", a);
	
	puts("testing several bitwise operations");
	a = ( ~a ^ b << 1) >> 1;
	
	printf("[a = ( ~a ^ b << 1) >> 1] == %d\n\n", a);
		
	puts("testing bitwise AND");
	a &= 7;
	printf("[a &= 7] == %d\n\n", a);
	
	puts("testing assignment");
	a = 456;
	b = 123;
	printf("a == %d\n", a);
	printf("b == %d\n\n", b);	
	
	puts("testing XOR swap trick");
	a ^= b;
	b ^= a;
	a ^= b;
	printf("a == %d\n", a);
	printf("b == %d\n\n", b);
	
	puts("testing multiple assignment..");
	a = b = 5;
	printf("a == %d\n", a);
	printf("b == %d\n\n", b);	
	
	printf("this number of bits set in a is: %d\n\n", bits_set(a));
	
	do {
		puts("enter a number (0 to quit):");
		
		i = getnum();
	
		if(i < 0) {
			puts("numbers must be positive, try again");
		} else {
			
			for(j = 1; j <= i; j += 1) {
				printf("%d summed is %d\n", j, sum(j));
			}
		}
	} while(i != 0);
}

//-----------------------------------------------------------------------------
// Name: sum
//-----------------------------------------------------------------------------
int sum(int num) {
	int running_sum = 0;
		
	while(num) {
		running_sum += num;
		num -= 1;
	}
	
	return running_sum;
}

//-----------------------------------------------------------------------------
// Name: print_alpha
//-----------------------------------------------------------------------------
int print_alpha() {
	foreach(auto ch : "ABCDEFGHIJKLMNOPQRSTUVWXYZ") {
		putchar(ch);
	}
	
	putchar('\n');
}

//-----------------------------------------------------------------------------
// Name: bits_set
//-----------------------------------------------------------------------------
int bits_set(int value) {
	
	// this is a global..
	ret = 0;
	while(value) {
		ret += 1;
		value &= (value - 1);
	}
	return ret;
}
