CC=g++ 
CXX_STANDART=c++17 
CFLAGS=-std=$(CXX_STANDART) -Wall -g -O3
PATH_BIN=./bin
CONNECT_DYN_LIB=-L$(PATH_BIN) -Wl,-rpath,. -llog 
PATH_LIB=./lib
PATH_SRC=./src

all: build_dyn_lib testUnitLog testLog main 




main: main.o
	$(CC) $(CFLAG) $(PATH_BIN)/main.o -o $(PATH_BIN)/main.out $(CONNECT_DYN_LIB)

build_dyn_lib: log.o
	$(CC) $(CFLAGS) -shared $(PATH_BIN)/log.o -o  $(PATH_BIN)/liblog.so

testUnitLog: testUnitLog.o build_dyn_lib
	$(CC) $(CFLAGS) $(PATH_BIN)/testUnitLog.o -o $(PATH_BIN)/testUnitLog.out -L$(PATH_BIN) -llog
testLog: testLog.o build_dyn_lib 
	$(CC) $(CFLAGS)  $(PATH_BIN)/testLog.o -o $(PATH_BIN)/testLog.out  -L$(PATH_BIN) -llog



log.o: 
	$(CC) $(CFLAG) -DDEBUG -fpic -c $(PATH_LIB)/log.cc -o $(PATH_BIN)/log.o
testLog.o: 
	$(CC) $(CFLAG) -DDEBUG -c $(PATH_LIB)/testLog.cc -o $(PATH_BIN)/testLog.o
testUnitLog.o: 
	$(CC) $(CFLAG) -DDEBUG -c $(PATH_LIB)/testUnitLog.cc -o $(PATH_BIN)/testUnitLog.o
main.o:
	$(CC) $(CLAGS) -DDEBUG $(PATH_SRC)/main.cc -c -o $(PATH_BIN)/main.o

clean:
	rm -rf ./P*.o  *.outa

