document.addEventListener("DOMContentLoaded", function() {
    const boardElement = document.getElementById('board');

    const currentPlayerElement = document.getElementById('current-player');
    const modeSelect = document.getElementById('mode-select');

    const store1 = createStoreElement('store-villain', 13);
    const store2 = createStoreElement('store-hero', 6);
    
    boardElement.appendChild(store1);

    // Hero's pits (indices 0 to 5)
    for (let i = 0; i < 6; i++) {
        const pit = createPitElement(`pit-${i}`, i);
        pit.classList.add('pit-row-hero');
        boardElement.appendChild(pit);
    }

    // Villain's pits (indices 12 down to 7)
    for (let i = 0; i < 6; i++) {
        const pitId = `pit-${12 - i}`;
        const index = 12 - i;
        const pit = createPitElement(pitId, index);
        pit.classList.add('pit-row-villain');
        boardElement.appendChild(pit);
    }

    boardElement.appendChild(store2);

    const resetButton = document.getElementById('reset-button');
    resetButton.addEventListener('click', function() {
        setGameModeAndReset();
    });

    const undoButton = document.getElementById('undo-button');
    undoButton.addEventListener('click', function() {
        Module.ccall('undoMove', 'void', [], []);
        updateBoard();
    });

    modeSelect.addEventListener('change', function() {
        setGameModeAndReset();
    });

    Module.onRuntimeInitialized = function() {
        setGameModeAndReset();
    };

    function createStoreElement(id, index) {
        const store = document.createElement('div');
        store.id = id;
        store.classList.add('store');
        store.dataset.index = index;
        return store;
    }

    function createPitElement(id, index) {
        const pit = document.createElement('div');
        pit.id = id;
        pit.classList.add('pit');
        pit.dataset.index = index;
    
        pit.addEventListener('click', function() {
            handlePitClick(id);
        });
    
        return pit;
    }

    function handlePitClick(id) {
        const pit = document.getElementById(id);
        const index = parseInt(pit.dataset.index);
        Module.ccall('makeMove', 'void', ['number'], [index]);
        updateBoard();
    }

    function updateBoard() {
        const boardState = Module.ccall('getBoard', 'string', [], []);
        const board = JSON.parse(boardState);

        for (let i = 0; i < 6; i++) {
            document.getElementById(`pit-${i}`).textContent = board.rowhero[i];
            document.getElementById(`pit-${12 - i}`).textContent = board.rowvillain[i];
        }

        document.getElementById('store-villain').textContent = board.storevillain;
        document.getElementById('store-hero').textContent = board.storehero;

        const currentPlayer = Module.ccall('getCurrentPlayer', 'number', [], []);

        if (currentPlayer == 0) {
            currentPlayerElement.textContent = "Current Player: Hero";
        } else {
            currentPlayerElement.textContent = "Current Player: Villain";
        }
    }

    function setGameModeAndReset() {
        // Get the selected mode from the dropdown
        const selectedMode = modeSelect.value;
        let mode;
        if (selectedMode === 'capture') {
            mode = 0;
        } else if (selectedMode === 'avalanche') {
            mode = 1;
        } else if (selectedMode === 'avalanche-capture') {
            mode = 2;
        } else {
            // Default to Capture 
            mode = 0;
        }

        Module.ccall('setGameMode', 'void', ['number'], [mode]);
        Module.ccall('resetGame', 'void', [], []);

        updateBoard();
    }

});
