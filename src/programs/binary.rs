fn printSpace(n:int) -> bool {
    for (let i: int = 0; i < n; i = i+1) {print ' ';}    
    return true;
}


//Prints bit representation of number.
//Limited to byte.

let n: int = 20;
let one:  int = 0;
let zero: int = 0;
let _0: bool = false;

for (let i: int = 1; i <= 256; i = i*2) {
    
    if (n and i == i) {
        print '1';
    } else {
        print '0';
    }
}
_0 = printSpace(60-9);