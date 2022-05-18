<<<<<<< HEAD
OBJ_dir = obj
OBJECTOS = main.o driver.o client.o memory.o process.o restaurant.o metime.o synchronization.o configuration.o stats.o log.o
metime.o = metime.h
stats.o = stats.h main.h
log.o = log.h
configuration.o = main.h configuration.h log.h stats.h
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
=======
OBJ_dir = obj
OBJECTOS = main.o driver.o client.o memory.o process.o restaurant.o metime.o synchronization.o configuration.o stats.o log.o
metime.o = metime.h
stats.o = stats.h main.h
log.o = log.h
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
>>>>>>> 944643a45a3dad45a22fe0789c5d1f23a4b1f33e
	rm -f magnaeats