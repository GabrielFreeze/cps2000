fn delay(seconds: int) -> bool {
    
    for (let i: float = 0; i < seconds; i = i+0.00001){}

    return true;
}

fn nth_fibonacci(n: int) -> int {
    let a:int = 0;
    let b:int = 1;
    let c:int = 0;

    for (let i:int = 0; i < n; i = i+1) {
        c = b;
        b = a+b;
        a = c;
    }

    return a;
}

for (let i: int = 0; true; i = i+1) {
    print nth_fibonacci(i);
    let _0: bool = delay(1);
}