turing: lex.yy.cc turing.cc turing.tab.cc machine_base.cc show_machine.cc interpreter.cc visualizer.cc turing.tab.hh machines.hh
	g++ -std=c++17 lex.yy.cc turing.cc turing.tab.cc machine_base.cc show_machine.cc interpreter.cc visualizer.cc -o turing

lex.yy.cc: turing.l
	flex turing.l

turing.tab.cc turing.tab.hh: turing.y
	bison -d turing.y

.PHONY: clean
clean:
	rm -f lex.yy.cc turing.tab.cc turing.tab.hh location.hh position.hh stack.hh turing temp.asm temp.o temp temp.out test/graphs/* test/output/*

.PHONY: visual
visual: turing
	./turing -v test/interactive/interactive.trm | dot -Tsvg > test/graphs/interactive.svg
	./turing -v test/interactive/binary_palindrome.trm | dot -Tsvg > test/graphs/binary_palindrome.svg
	./turing -v test/interactive/even_zeroes.trm | dot -Tsvg > test/graphs/even_zeroes.svg
	./turing -v test/interactive/divide_by_3.trm | dot -Tsvg > test/graphs/divide_by_3.svg
	./turing -v test/interactive/fast_palindrome.trm | dot -Tsvg > test/graphs/fast_palindrome.svg
	./turing -v test/interactive/binary_addition.trm | dot -Tsvg > test/graphs/binary_addition.svg

.PHONY: outputs
outputs: turing
	./turing -i test/fixed/accept_binary_palindrome.trm > test/output/accept_binary_palindrome.tps
	./turing -i test/fixed/accept_even_zeroes.trm > test/output/accept_even_zeroes.tps
	./turing -i test/fixed/accept_divide_by_3.trm > test/output/accept_divide_by_3.tps
	./turing -i test/fixed/accept_fast_palindrome.trm > test/output/accept_fast_palindrome.tps
	./turing -i test/fixed/accept_binary_addition.trm > test/output/accept_binary_addition.tps
	./turing -i test/fixed/reject_binary_palindrome.trm > test/output/reject_binary_palindrome.tps
	./turing -i test/fixed/reject_even_zeroes.trm > test/output/reject_even_zeroes.tps
	./turing -i test/fixed/reject_divide_by_3.trm > test/output/reject_divide_by_3.tps
	./turing -i test/fixed/reject_fast_palindrome.trm > test/output/reject_fast_palindrome.tps
	./turing -i test/fixed/reject_binary_addition.trm > test/output/reject_binary_addition.tps