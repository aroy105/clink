document.addEventListener("DOMContentLoaded", function() {
    const boardElement = document.getElementById('board');
    
    function updateBoard(boardArray) {
        boardElement.innerHTML = '';
        for (let i = 0; i < boardArray.length; i++) {
            const pit = document.createElement('div');
            pit.className = 'pit';
            pit.textContent = boardArray[i];
            boardElement.appendChild(pit);
        }
    }

    var Module = {
        onRuntimeInitialized: function() {
            const boardPointer = Module._getBoard();
            const boardArray = new Int32Array(Module.HEAP32.buffer, boardPointer, 14);
            updateBoard(boardArray);

            document.querySelector('#board').addEventListener('click', function(event) {
                if (event.target.className === 'pit') {
                    const pitIndex = Array.from(event.target.parentNode.children).indexOf(event.target);
                    Module._makeMove(pitIndex);
                    updateBoard(boardArray);
                }
            });
        }
    };
});
