#!/usr/bin/quixey

int main() {
	string s = "World!";
	
	
	auto f = function(auto x) {
		x = "Hello";
		printf("%s, ", x);
	};
	
	f(s);
	puts(s);

}
