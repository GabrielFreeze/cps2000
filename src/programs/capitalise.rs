let letter: char = 'a';

let _0: bool = false;

fn printLine() -> bool {
    for (let i: int = 0; i < 60; i = i+1) {print ' ';}    
    return true;
}

fn capitalise(letter: char) -> char {
    return letter - 32;
}


print capitalise('a');
_0 = printLine();

