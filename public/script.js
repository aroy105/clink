document.addEventListener("DOMContentLoaded", function() {
    const boardElement = document.getElementById('board');

    const store1 = createStoreElement('store-villain', 13);
    const store2 = createStoreElement('store-hero', 6);
    
    boardElement.appendChild(store1);

    for (let i = 0; i < 6; i++) {
        const pit = createPitElement(`pit-${i}`, i);
        pit.classList.add('pit-row-hero');
        boardElement.appendChild(pit);
    }

    for (let i = 5; i >= 0; i--) {
        const pit = createPitElement(`pit-${12 - i}`, 7 + i); 
        pit.classList.add('pit-row-villan');
        boardElement.appendChild(pit);
    }

    boardElement.appendChild(store2);

    Module.onRuntimeInitialized = function() {
        updateBoard();
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
    
        Module.ccall(
            'makeMove',
            'void',
            ['number'],
            [index]
        );
        
        updateBoard();
    }

    function updateBoard() {
        boardState = Module.ccall('getBoard', 'string', [], []);
        board = JSON.parse(boardState);

        for (let i = 0; i < 6; i++) {
            document.getElementById(`pit-${i}`).textContent = board.rowhero[i];
            document.getElementById(`pit-${i + 7}`).textContent = board.rowvillain[i];
        }
        document.getElementById('store-villain').textContent = board.storevillain;
        document.getElementById('store-hero').textContent = board.storehero;
    }
});
