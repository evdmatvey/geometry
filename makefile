# names
APP_NAME = geometry
LIB_NAME = libgeometry

# dirs
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test
THIRDPARTY_DIR = thirdpatry

# paths
OBJ_PATH = $(OBJ_DIR)/$(SRC_DIR)
APP_PATH = $(BIN_DIR)/$(APP_NAME)
TEST_BIN = $(BIN_DIR)/$(APP_NAME)_test
LIB_PATH = $(SRC_DIR)/$(LIB_NAME)
TEST_PATH = $(OBJ_DIR)/$(TEST_DIR)
LIB = $(OBJ_PATH)/$(LIB_NAME)

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

run:
	$(APP_PATH)

$(TEST_PATH)/main.o: $(TEST_DIR)/main.c $(THIRDPARTY_DIR)/ctest.h
	$(CC) $(CFLAGS) -I thirdpatry -c $< -o $@

$(TEST_PATH)/token_test.o: $(TEST_DIR)/token_test.c $(THIRDPARTY_DIR)/ctest.h  $(LIB)/$(LIB_NAME).a
	$(CC) $(CFLAGS) -I thirdpatry -c $< -o $@ -L obj/src/libgeometry -lgeometry

$(TEST_PATH)/calculator_test.o: $(TEST_DIR)/calculator_test.c $(THIRDPARTY_DIR)/ctest.h  $(LIB)/$(LIB_NAME).a
	$(CC) $(CFLAGS) -I thirdpatry -c $< -o $@ -L obj/src/libgeometry -lgeometry

test: $(TEST_PATH)/main.o $(TEST_PATH)/token_test.o $(TEST_PATH)/calculator_test.o $(THIRDPARTY_DIR)/ctest.h
	$(CC) $^ -o $(TEST_BIN) $(LDFLAGS)
	$(TEST_BIN)

clean:
	rm -f $(APP_PATH)
	rm -f $(OBJ_DIR)/$(SRC_DIR)/$(APP_NAME)/*.o
	rm -f $(OBJ_DIR)/$(LIB_PATH)/$(LIB_NAME).a
	rm -f $(OBJ_DIR)/$(SRC_DIR)/$(APP_NAME)/*.d
	rm -f $(OBJ_DIR)/$(LIB_PATH)/*.d
	rm -f $(OBJ_DIR)/$(LIB_PATH)/*.o