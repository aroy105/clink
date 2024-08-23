document.addEventListener("DOMContentLoaded", function() {
    const boardElement = document.getElementById('board');

    const store1 = createStoreElement('store-1');
    const store2 = createStoreElement('store-2');
    
    boardElement.appendChild(store1);

    for (let i = 0; i < 6; i++) {
        const pit = createPitElement(`pit-1-${i}`, i);
        pit.classList.add('pit-row-1');
        boardElement.appendChild(pit);
    }

    for (let i = 0; i < 6; i++) {
        const pit = createPitElement(`pit-2-${i}`, i);
        pit.classList.add('pit-row-2');
        boardElement.appendChild(pit);
    }

    boardElement.appendChild(store2);

    function createStoreElement(id) {
        const store = document.createElement('div');
        store.id = id;
        store.classList.add('store');
        store.textContent = '0'; 
        return store;
    }

    function createPitElement(id, index) {
        const pit = document.createElement('div');
        pit.id = id;
        pit.classList.add('pit');
        pit.textContent = '4';
        pit.dataset.index = index;
        pit.addEventListener('click', function() {
            handlePitClick(id);
        });
        return pit;
    }

    function handlePitClick(id) {
        console.log(`Pit ${id} clicked`);
    }
});
