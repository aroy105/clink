FROM emscripten/emsdk:latest
WORKDIR /usr/src/app

COPY public/ public/
COPY src/ src/

RUN emcc src/main.cpp -o public/generated.js -s EXPORTED_FUNCTIONS='["_makeMove", "_undoMove", "_displayBoard", "_getCurrentPlayer", "_getBoard"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'

EXPOSE 8080
CMD ["python3", "-m", "http.server", "8080", "-d", "public"]
