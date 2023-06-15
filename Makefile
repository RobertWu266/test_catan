all:
	gcc catan_ui.c TUI.c catan_conversions.c catan.c -o catan -lncurses -g

clean:
	rm -rf catan