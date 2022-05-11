OBJ_dir = obj
OBJECTOS = main.o driver.o client.o memory.o process.o restaurant.o metime.o synchronization.o configuration.o
metime.o = metime.h
configuration.0 = main.h configuration.h
synchronization.o = synchronization.h
main.o = main.h memory.h process.h metime.h synchronization.h configuration.h
driver.o = driver.h main.h memory.h metime.h synchronization.h
client.o = client.h main.h memory.h metime.h synchronization.h
restaurant.o = restaurant.h main.h memory.h synchronization.h
memory.o = memory.h
process.o = main.h process.h driver.h client.h restaurant.h

CC = gcc

vpath %.o $(OBJ_DIR)

out: $(OBJECTOS)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -lrt -lpthread -Wall -o magnaeats 
# -fsanitize=address
%.o: src/%.c $($@)
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<

clean:
	rm -f $(OBJ_dir)/*.o out
	rm -f magnaeats