let a:int = 0;
let b:int = 1;
let c:int = 0;

let n:int = 10;
let i:int = 0;

for (; i < n;) {
    c = b;
    b = a+b;
    a = c;

    i = i+1;
}

print a;

