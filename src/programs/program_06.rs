fn nth_fibonacci(n:int) -> int {
    let a:int = 0;
    let b:int = 1;
    let c:int = 0;

    for (let i:int = 0; i < n; i = i+1) {
        c = b;
        b = a+b;
        a = c;
    }

    retrun a;
}