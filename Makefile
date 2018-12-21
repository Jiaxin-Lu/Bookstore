CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = code
OBJ = bookstore.cpp

$(TARGET) : $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) $(CXXFLAGS)
	
clean : 
	rm -f