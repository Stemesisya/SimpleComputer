all:
	find . -type f -not -wholename './Makefile' -name Makefile -execdir make \;