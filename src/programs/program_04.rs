fn Sq(x:float) -> float {
    return x*x;
}

fn XGreaterY(x:float, y:float) -> bool {
    let ans:bool = true;
    if (y > x) { ans = false; }
    return ans;
}

//Same functionality as function above but using less code
fn XGreaterY_2(x:float, y:float) -> bool {
    return x > y;
}

fn AverageOfThree(x:float, y:float, z:float) -> float {
    let total:float = x + y + z;
    return total/3;
}

/*
 * Same functionality as function above but using less code.
 * Note the use of the brackets in the expression following
 * the return statement.
 */
fn AverageOfThree_2(x:float, y:float, z:float) -> float {
    return (x + y + z) / 3;
}

//Execution (program entry point) starts at the first statement
//that is not a function declaration.

let x:float = 2.4;
let y:float = Sq(2.5);
let z:float = Sq(x);

print x * z;
print XGreaterY(x, 2.3);
print XGreaterY_2(Sq(1.5), y);
print AverageOfThree(x, y, 1.2);
