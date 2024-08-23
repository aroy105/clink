FROM emscripten/emsdk:latest
WORKDIR /usr/src/app

COPY public/ public/
COPY src/ src/

RUN emcc src/main.cpp -o public/index.html -s EXPORTED_FUNCTIONS='["_makeMove", "_undoMove", "_getBoard", "_displayBoard", "_getCurrentPlayer"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'

EXPOSE 8080
CMD ["python3", "-m", "http.server", "8080", "-d", "public"]
