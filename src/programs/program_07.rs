let y: bool = bitwiseAnd(2,2);
let x: bool = bitwiseAnd(y,5);

if (y == true) {
    let x: char = '5';
    print x;
} else {
    let x: int = 1;
    print x;
}

print y;
print x;


fn bitwiseAnd(x:int, y:int) -> bool {
    return x and y;
}
