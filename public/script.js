document.addEventListener("DOMContentLoaded", function() {
    console.log("DOM fully loaded and parsed");
    const boardElement = document.getElementById("board");

    const board = [
        4, 4, 4, 4, 4, 4, 0,
        4, 4, 4, 4, 4, 4, 0
    ];

    function renderBoard() {
        boardElement.innerHTML = "";
        for (let i = 0; i < board.length; i++) {
            const pit = document.createElement("div");
            pit.className = "pit";
            pit.textContent = board[i];
            pit.dataset.index = i;

            // placeholder, plug this in later
            // pit.addEventListener("click", function() {
            //     handlePitClick(i); 
            // });

            boardElement.appendChild(pit);
        }
    }


    // Initial render of the board
    renderBoard();
});
