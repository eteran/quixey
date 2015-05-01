#!/usr/bin/quixey

int f(int s) {
	s = 10;
	printf("%d\n", s);
}
int main() {
	int s = 5;
	f(s);
	printf("%d\n", s);
	
}
