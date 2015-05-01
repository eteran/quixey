#!/usr/bin/quixey

string hello(auto x) {
	return "hello, " + x;
}

int f(auto x) {

	if(is_string(x)) {
		foreach(auto ch : x) {
			printf("%c", ch);
		}
		printf("\n");
	}
	
	return 10;
}

int main() {
	string s = hello("Evan");
	
	auto z = f(20);
	auto h = f("hello");

	printf("%d %d\n", z, h);

	for(int i = 0; i < size(s); i += 1) {
		printf("%c", s[i]);
	}
	printf("\n");
	
	f(s);
	
	if(is_string(s)) {
		printf("STRING!\n");
	} else {
		printf("NO?!\n");
	}
	
	auto undefined;
	
	
	int nn = 10;
	while(nn > 0) {
		putchar('A');
		nn = nn - 1;
	}
	
	printf("\n");
	
	nn = 10;
	do {
		putchar('A');
		nn = nn - 1;
	} while(nn > 0);
	
	printf("\n");
	
	printf("%d %d %d %d\n", is_integer(s),         is_string(s),         is_character(s),         is_array(s));
	printf("%d %d %d %d\n", is_integer(undefined), is_string(undefined), is_character(undefined), is_array(undefined));
	
	
	char ch = int(0x42);
	
	string ss = "hello ";
	ss += char(0x41);
	ss += ch;
	ss += "SS";
	puts(ss);
	
}
