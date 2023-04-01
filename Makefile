OBJDIR = obj
SRCDIR = src
SERVER_DIR = server
CLIENT_DIR = client
SERVER = uchat_server
CLIENT = uchat
DATABASE = database.db

LIBMX_DIR = libraries/libmx
LIBMX = $(LIBMX_DIR)/libmx.a

SRC = $(SRCDIR)/*.c
OBJS = $(OBJDIR)/*.o

all: $(LIBMX) $(SERVER) $(CLIENT)

install:
	sudo apt update && sudo apt install libsqlite3-dev libssl-dev

$(LIBMX):
	make -sC $(LIBMX_DIR)

$(SERVER):
	make -sC $(SERVER_DIR)

$(CLIENT):
	make -sC $(CLIENT_DIR)

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR) 
	rm -rf $(SERVER)
	rm -rf $(CLIENT)
	rm -rf server/$(DATABASE)
	rm -rf server/cout.log
	rm -rf client/cout.log



uninstall:
	make -sC $(SERVER_DIR) $@
	make -sC $(CLIENT_DIR) $@
	make -sC $(LIBMX_DIR) $@
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all