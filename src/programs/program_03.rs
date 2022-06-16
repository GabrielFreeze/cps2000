//Function definition for Power
fn Pow(x:float , n:int) -> float {
    
    let y : float = 1.0;

    if (n > 0) {
        for (; n>0; n=n-1) {
            y = y*x;
        }

        if (y == 3) {
            if (x == 2) {
                return y;
                x = 2;
            }
            else {
                return x;
            }
        } else {
            return y;
        }
        
    } else {
        for (; n < 0; n=n+1) {
            y = y/x;
        }

        return x;
    }
}

let x : float = Pow(2.1,10);

print x; //prints to console 1667.988