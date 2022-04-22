//Function definition for Power
fn Pow(x:float , n:int) -> float {
    
    let y : float = 1.0;

    if (n > 0) {
        for (; n>0; n=n-1) {
            y = y*x;
        }
    } else {
        for (; n < 0; n=n+1) {
            y = y/x;
        }
    }
    return y;
}

let x : float = Pow(2.1,10);

print x; //prints to console 1667.988