EXEC_NAME=hashtable
INCLUDE_DIRS=lib/json/include
C++_LANG_LEVEL=c++17


debug: *.cpp *.h
	g++ -g -O0 -std=${C++_LANG_LEVEL} -I ${INCLUDE_DIRS} main.cpp -o dbg_${EXEC_NAME}

release:: *.cpp *.h
	g++ -O3 -std=${C++_LANG_LEVEL} -I ${INCLUDE_DIRS} main.cpp -o ${EXEC_NAME}

bench: *.cpp *.h
	g++ -O3 -std=${C++_LANG_LEVEL} -I ${INCLUDE_DIRS} bench/bm_main.cpp -o bm_${EXEC_NAME}

clean:
	rm -f *.o *.lib