CC = g++-7
CFLAGS = -Wall -pedantic -std=c++11 -Wfatal-errors -pedantic-errors -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wold-style-cast -Werror

board: Board.cc BoardTester.cc
	$(CC) -o boardTest Board.cc BoardTester.cc $(CFLAGS)

framework: Board.cc MultiGameFramework.cc PlayerFactory.cc RandomPlayer.cc FrameworkTester.cc
	$(CC) -o frTest Board.cc MultiGameFramework.cc PlayerFactory.cc RandomPlayer.cc FrameworkTester.cc $(CFLAGS)

utils: Utils.cc utilTester.cc
	$(CC) -o utilTest Utils.cc utilTester.cc $(CFLAGS)

det: Board.cc MultiGameFramework.cc PlayerFactory.cc RandomPlayer.cc detTester.cc Utils.cc DeterministicPlayer.cc
	$(CC) -o detTest Board.cc MultiGameFramework.cc PlayerFactory.cc RandomPlayer.cc detTester.cc Utils.cc DeterministicPlayer.cc $(CFLAGS)

debugger: Board.cc PlayerFactory.cc RandomPlayer.cc Utils.cc DeterministicPlayer.cc PlayerDebugger.cc
	$(CC) -o dbTest Board.cc PlayerFactory.cc RandomPlayer.cc Utils.cc DeterministicPlayer.cc PlayerDebugger.cc $(CFLAGS)

clean:
	rm *Test
