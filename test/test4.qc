#!/usr/bin/quixey

auto dump_array(auto a) {

	if(is_array(a)) {
		printf("Array(\n");
		foreach(auto e : a) {
			if(is_integer(e)) {
				printf("I: %d\n", e);
			}

			if(is_character(e)) {
				printf("C: %c\n", e);
			}

			if(is_string(e)) {
				printf("S: %s\n", e);
			}

			if(is_function(e)) {
				printf("F: ");
				e();
			}

			if(is_array(e)) {	
				printf("Array(\n");
				dump_array(e);
				printf(")\n");
			}
		}
		printf(")\n");
	}
}

int main() {
	auto x = ['a', 'b', 'c', 1234, "HELLO", [1, 2, 3, 4], function() { printf("W00t!\n"); }];
	dump_array(x);
	printf("%c\n", "test"[1]);
}
