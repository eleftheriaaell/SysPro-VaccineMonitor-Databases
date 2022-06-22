executable = vaccineMonitor

all : vaccineMonitor

vaccineMonitor : main.c get_string.c bloom.c skiplist.c extra_lists.c
	gcc -o vaccineMonitor main.c get_string.c bloom.c skiplist.c extra_lists.c
clean:
	rm -f $(executable) 