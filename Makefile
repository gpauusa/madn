OBJDIR=obj
BINDIR=bin
SRCDIR=src

CC=gcc
CFLAGS=-g -Wall -Werror #-DTEST

all: madn_queues_test madn_main_struct_test madn_pkt_struct_test madn_cache_test madn_run madn_pkt_struct.o madn_routing_glib_queues.o madn_routing_glib_rt.o madn_routing_glib_nb.o madn_routing.o madn_bloom.o madn_coder.o madn_cache_glib.o madn_entry.o madn_threading_pthreads_nb.o madn_register_local.o MSRStructs.o

test: madn_main_struct_test madn_pkt_struct_test madn_queues_test madn_cache_test
	$(BINDIR)/madn_main_struct_test
	$(BINDIR)/madn_pkt_struct_test
	$(BINDIR)/madn_queues_test
	$(BINDIR)/madn_cache_test

#Binaries
madn_main_struct_test: madn_main_struct_test.o madn_main_struct.o madn_settings.o
	$(CC) $(CFLAGS) \
    $(OBJDIR)/madn_main_struct_test.o \
    $(OBJDIR)/madn_main_struct.o \
    $(OBJDIR)/madn_settings.o \
    -o $(BINDIR)/madn_main_struct_test

madn_pkt_struct_test: madn_pkt_struct_test.o madn_pkt_struct.o madn_settings.o
	$(CC) $(CFLAGS) \
    $(OBJDIR)/madn_pkt_struct_test.o \
    $(OBJDIR)/madn_pkt_struct.o \
    $(OBJDIR)/madn_main_struct.o \
    $(OBJDIR)/madn_settings.o \
    -o $(BINDIR)/madn_pkt_struct_test

madn_queues_test: madn_queues_test.o madn_pkt_struct.o madn_routing_glib_nb.o madn_routing_glib_rt.o madn_routing_glib_queues.o madn_routing.o madn_pkt_struct.o madn_main_struct.o madn_bloom.o madn_cache_glib.o madn_coder.o madn_register_local.o madn_socket_helper.o MSRStructs.o madn_settings.o
	$(CC) $(CFLAGS) `pkg-config --libs glib-2.0` -lpthread\
    $(OBJDIR)/madn_queues_test.o \
    $(OBJDIR)/madn_routing_glib_nb.o \
    $(OBJDIR)/madn_routing_glib_rt.o \
    $(OBJDIR)/madn_routing_glib_queues.o \
    $(OBJDIR)/madn_routing.o \
    $(OBJDIR)/madn_pkt_struct.o \
    $(OBJDIR)/madn_main_struct.o \
    $(OBJDIR)/madn_bloom.o \
    $(OBJDIR)/madn_cache_glib.o \
    $(OBJDIR)/madn_coder.o \
    $(OBJDIR)/madn_register_local.o \
    $(OBJDIR)/madn_socket_helper.o \
    $(OBJDIR)/madn_settings.o \
    $(OBJDIR)/MSRStructs.o \
    -o $(BINDIR)/madn_queues_test

madn_run: madn_queues_test.o madn_pkt_struct.o madn_routing_glib_nb.o madn_routing_glib_rt.o madn_routing_glib_queues.o madn_routing.o madn_pkt_struct.o madn_main_struct.o madn_bloom.o madn_cache_glib.o madn_coder.o madn_entry.o madn_threading_pthreads_nb.o madn_register_local.o MSRStructs.o madn_socket_helper.o madn_settings.o
	$(CC) $(CFLAGS) `pkg-config --libs glib-2.0` -lpthread\
    $(OBJDIR)/madn_routing_glib_nb.o \
    $(OBJDIR)/madn_routing_glib_rt.o \
    $(OBJDIR)/madn_routing_glib_queues.o \
    $(OBJDIR)/madn_routing.o \
    $(OBJDIR)/madn_pkt_struct.o \
    $(OBJDIR)/madn_main_struct.o \
    $(OBJDIR)/madn_bloom.o \
    $(OBJDIR)/madn_cache_glib.o \
    $(OBJDIR)/madn_coder.o \
    $(OBJDIR)/madn_entry.o \
    $(OBJDIR)/madn_threading_pthreads_nb.o \
    $(OBJDIR)/madn_register_local.o \
    $(OBJDIR)/MSRStructs.o \
    $(OBJDIR)/madn_socket_helper.o \
    $(OBJDIR)/madn_settings.o \
    -o $(BINDIR)/madn_run

madn_cache_test: madn_cache_test.o madn_pkt_struct.o madn_routing_glib_nb.o madn_routing_glib_rt.o madn_routing_glib_queues.o madn_routing.o madn_pkt_struct.o madn_main_struct.o madn_bloom.o madn_cache_glib.o madn_coder.o madn_register_local.o MSRStructs.o madn_socket_helper.o madn_settings.o
	$(CC) $(CFLAGS) `pkg-config --libs glib-2.0` -lpthread\
    $(OBJDIR)/madn_cache_test.o \
    $(OBJDIR)/madn_routing_glib_nb.o \
    $(OBJDIR)/madn_routing_glib_rt.o \
    $(OBJDIR)/madn_routing_glib_queues.o \
    $(OBJDIR)/madn_routing.o \
    $(OBJDIR)/madn_pkt_struct.o \
    $(OBJDIR)/madn_main_struct.o \
    $(OBJDIR)/madn_bloom.o \
    $(OBJDIR)/madn_cache_glib.o \
    $(OBJDIR)/madn_coder.o \
    $(OBJDIR)/madn_register_local.o \
    $(OBJDIR)/madn_socket_helper.o \
    $(OBJDIR)/MSRStructs.o \
    $(OBJDIR)/madn_settings.o \
    -o $(BINDIR)/madn_cache_test

#Test Objects
madn_main_struct_test.o: $(SRCDIR)/madn_main_struct_test.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_main_struct_test.c -o $(OBJDIR)/madn_main_struct_test.o

madn_pkt_struct_test.o: $(SRCDIR)/madn_pkt_struct_test.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_pkt_struct_test.c -o $(OBJDIR)/madn_pkt_struct_test.o

madn_queues_test.o: $(SRCDIR)/madn_queues_test.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_queues_test.c -o $(OBJDIR)/madn_queues_test.o

madn_cache_test.o: $(SRCDIR)/madn_cache_test.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_cache_test.c -o $(OBJDIR)/madn_cache_test.o

#Application Objects
madn_main_struct.o: $(SRCDIR)/madn_main_struct.c $(SRCDIR)/madn_main_struct.h
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_main_struct.c -o $(OBJDIR)/madn_main_struct.o

madn_pkt_struct.o: $(SRCDIR)/madn_pkt_struct.c $(SRCDIR)/madn_pkt_struct.h
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_pkt_struct.c -o $(OBJDIR)/madn_pkt_struct.o

madn_routing.o: $(SRCDIR)/madn_routing.c $(SRCDIR)/madn_routing.h
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_routing.c -o $(OBJDIR)/madn_routing.o

madn_routing_glib_queues.o: $(SRCDIR)/madn_routing_glib_queues.c $(SRCDIR)/madn_main_struct.h $(SRCDIR)/madn_routing.h
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_routing_glib_queues.c -o $(OBJDIR)/madn_routing_glib_queues.o

madn_routing_glib_rt.o: $(SRCDIR)/madn_routing_glib_rt.c $(SRCDIR)/madn_main_struct.h $(SRCDIR)/madn_routing.h
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_routing_glib_rt.c -o $(OBJDIR)/madn_routing_glib_rt.o

madn_routing_glib_nb.o: $(SRCDIR)/madn_routing_glib_nb.c $(SRCDIR)/madn_main_struct.h $(SRCDIR)/madn_routing.h
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_routing_glib_nb.c -o $(OBJDIR)/madn_routing_glib_nb.o

madn_bloom.o: $(SRCDIR)/madn_bloom.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_bloom.c -o $(OBJDIR)/madn_bloom.o

madn_coder.o: $(SRCDIR)/madn_coder.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_coder.c -o $(OBJDIR)/madn_coder.o

madn_cache_glib.o: $(SRCDIR)/madn_cache_glib.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_cache_glib.c -o $(OBJDIR)/madn_cache_glib.o

madn_entry.o: $(SRCDIR)/madn_entry.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_entry.c -o $(OBJDIR)/madn_entry.o

madn_threading_pthreads_nb.o: $(SRCDIR)/madn_threading_pthreads_nb.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_threading_pthreads_nb.c -o $(OBJDIR)/madn_threading_pthreads_nb.o

madn_register_local.o: $(SRCDIR)/madn_register_local.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_register_local.c -o $(OBJDIR)/madn_register_local.o

MSRStructs.o: $(SRCDIR)/MSRStructs.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/MSRStructs.c -o $(OBJDIR)/MSRStructs.o

madn_socket_helper.o: $(SRCDIR)/madn_socket_helper.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_socket_helper.c -o $(OBJDIR)/madn_socket_helper.o

madn_settings.o: $(SRCDIR)/madn_settings.c
	$(CC) $(CFLAGS) `pkg-config --cflags glib-2.0` -c $(SRCDIR)/madn_settings.c -o $(OBJDIR)/madn_settings.o

#Misc
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*
