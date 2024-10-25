#include <libgeometry/calculator.h>
#include <libgeometry/lexer.h>
#include <libgeometry/parser.h>
#include <libgeometry/token.h>

int main(int argc, char* argv[])
{
    char* file_path;

    if (argc > 1) {
        file_path = argv[1];
    }

    size_t size = 0;

    token** tokens = read_file(file_path, &size);

    shape* shapes = parse_lines(&tokens, &size);

    calculate(&shapes, &size);

    return 0;
}
