all:
	gcc catan_ui.c TUI.c catan_conversions.c catan.c -o catan -lncurses
	gcc default_ncurses.c -o default_ncurses -lncurses
clean:
	rm -rf catan