# names
APP_NAME = geometry
LIB_NAME = libgeometry

# dirs
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj
BIN_DIR = bin

# paths
OBJ_PATH = $(OBJ_DIR)/$(SRC_DIR)
APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(SRC_DIR)/$(LIB_NAME)
LIB = $(OBJ_PATH)/$(LIB_NAME)
TEST = $(OBJ_DIR)/$(TEST_DIR)
# compiler options
CC = gcc
CFLAGS = -c -Wall -Wextra -Werror -I $(SRC_DIR) -MP -MMD
LDFLAGS = -L $(OBJ_PATH)/$(LIB_NAME) -l$(APP_NAME) -lm

# files

LIB_SOURCES = main.c hello.c factorial.c
LIB_OBJECTS = $(SOURCES:.c=.o)

# commands

all: $(APP_PATH)

$(LIB)/$(LIB_NAME).a: $(LIB)/calculator.o $(LIB)/lexer.o $(LIB)/parser.o $(LIB)/error-handler.o $(LIB)/utilities.o $(LIB)/token.o
	ar rcs $@ $^

$(LIB)/calculator.o: $(LIB_PATH)/calculator.c $(LIB_PATH)/calculator.h $(LIB_PATH)/parser.h $(LIB_PATH)/token.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB)/lexer.o: $(LIB_PATH)/lexer.c $(LIB_PATH)/lexer.h $(LIB_PATH)/token.h $(LIB_PATH)/error-handler.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB)/parser.o: $(LIB_PATH)/parser.c $(LIB_PATH)/parser.h $(LIB_PATH)/token.h $(LIB_PATH)/lexer.h $(LIB_PATH)/error-handler.h $(LIB_PATH)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB)/error-handler.o: $(LIB_PATH)/error-handler.c $(LIB_PATH)/error-handler.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB)/utilities.o: $(LIB_PATH)/utilities.c $(LIB_PATH)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB)/token.o: $(LIB_PATH)/token.c $(LIB_PATH)/token.h
	$(CC) $(CFLAGS) -c $< -o $@

$(APP_PATH): $(OBJ_PATH)/$(APP_NAME)/main.o
	$(CC) $^ -o $(APP_PATH) $(LDFLAGS)

$(OBJ_PATH)/$(APP_NAME)/main.o: $(LIB)/$(LIB_NAME).a $(SRC_DIR)/$(APP_NAME)/main.c
	rm -f $(OBJ_DIR)/$(LIB_PATH)/*.d
	rm -f $(OBJ_DIR)/$(LIB_PATH)/*.o
	$(CC) $(CFLAGS) $^ -o $@ -L obj/src/libgeometry -lgeometry

$(TEST)/token_test.o: $(TEST_DIR)/token_test.c $(TEST_DIR)/token_test.c
	$(CC) $(CFLAGS) -I thirdparty -c $< -o $@

$(TEST)/main.o: $(TEST_DIR)/main.c $(TEST_DIR)/main.c
	$(CC) $(CFLAGS) -I thirdparty -c $< -o $@

$(BIN_DIR)/$(APP_NAME)-test: $(TEST)/main.o $(TEST)/token_test.o
	$(CC) $(CFLAGS) -I thirdparty $^ -o $@ -L obj/src/libgeometry -lgeometry

test: $(BIN_DIR)/$(APP_NAME)-test

run:
	$(APP_PATH)

clean:
	rm -f $(APP_PATH)
	rm -f $(OBJ_DIR)/$(SRC_DIR)/$(APP_NAME)/*.o
	rm -f $(OBJ_DIR)/$(LIB_PATH)/$(LIB_NAME).a
	rm -f $(OBJ_DIR)/$(SRC_DIR)/$(APP_NAME)/*.d
	rm -f $(OBJ_DIR)/$(LIB_PATH)/*.d
	rm -f $(OBJ_DIR)/$(LIB_PATH)/*.o