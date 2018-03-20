Blimp: Blimp_test.cpp
	g++ -Wall -Werror -g --std=c++11 Blimp_test.cpp -o Blimp

Blimp_2: Blimp_test_2.cpp
	g++ -Wall -Werror -g --std=c++11 Blimp_test_2.cpp -o Blimp2

clean:
	rm -rvf main Blimp Blimp2 *~ *.out *.dSYM *.stackdump