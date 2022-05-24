let a:int = 0;
let b:int = 1;
let c:int = 0;

let n:int = 10;
let i:int = 0;

for (; i < n; i = i+1) {
    c = b;
    b = a+b;
    a = c;
}

print a;

