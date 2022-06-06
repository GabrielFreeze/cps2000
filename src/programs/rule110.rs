fn getBit(n:int, k:int) -> bool {
    let i:int = 1;

    for (let j:int = 0; j < k; j=j+1) {
        i = i*2;
    }

    return (n and i)== i;
}

fn SHR(n:int, k:int) -> int{
    let m: int = 0;
    let mask: int = 1073741824; //2^31

    for (let i:int = 0; i < k; i=i+1) {
        for (let j:int = 31; j >= 0; j=j-1) {
            m = m or (getBit(n,j)*mask);
            mask = mask/2;  
        }
        
        mask = 1073741824; n = m; m = 0;
    }
    return n;
}

fn SHL(n:int, k:int) -> int{
    let m: int = 0;
    let mask: int = 2;

    for (let i:int = 0; i < k; i=i+1) {
        for (let j:int = 0; j <= 32; j=j+1) {
            m = m or (getBit(n,j)*mask);
            mask = mask*2;
        }
        
        mask = 2; n = m; m = 0;
    }
    return n;
}

fn setBit(n:int, k:int, bit:bool) -> int {
    if (bit) {
        return n or SHL(1,k);
    } else {
        return n and (not SHL(1,k));
    }
}

fn printBoard(n:int) -> bool {
    //-2,147,483,648 == 1 << 31. NB: Most significant bit is 2's Complement 
    for (let i: int = 1; i <= 2147483648; i = i*2) {

        if ((n and i) == i) {
            print '#';
        } else {
            print ' ';
        }
    }

    return true;
}

let _0: bool = 0;

let BOARD_CAP: int = 25;
let board: int = SHL(1,24);


for (let i: int = 0; i < BOARD_CAP; i=i+1) {
    
    //Display Board
    _0 = printBoard(board);

    //Make terminal print in next line
    print ' ';
        
    //Update baord
    let pattern:int = SHL(getBit(board,0),1) or getBit(board,1);

    for (let j:int = 1; j < BOARD_CAP-1; j=j+1) {
        pattern = (SHL(pattern,1) and 7) or (getBit(board,j+1));
        board = setBit(board,j,SHR(110,pattern) and 1);
    }
}

