CC := g++-4.8
FLAG := -std=c++11
INC := -I./include

bin/main : build/Agenda.o build/AgendaUI.o build/AgendaService.o build/Storage.o build/Meeting.o build/Date.o build/User.o
	@mkdir -p bin
	$(CC) $(FLAG) $(INC) $^ -o $@

build/%.o : src/%.cpp
	@mkdir -p build
	$(CC) $(FLAG) $(INC) -c -o $@ $<

clean:
	@rm -rf build
	@rm -rf bin

