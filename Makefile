.SILENT:
all:
	find . -type f -not -wholename './Makefile' -name Makefile -execdir make \;
	echo "\033[38;2;255;20;140mCompilation complete.\033[0m"