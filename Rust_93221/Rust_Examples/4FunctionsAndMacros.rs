fn main() {
	let num1 = 2;
	let num2 = 5;

	println!("{} + {} = {}", num1, num2, sum(num1, num2));

	let result = 0;
	println!("{} x {} = {}", num1, num2, multiply(num1, num2, result));

	let mut value = 100;
	println!("\nBefore: {}", value);
	double(&mut value);
	println!("After: {}\n", value);

	println!("Sum: {}\n", sum!());
}

// Simple function with 2 arguments that returns an integer
fn sum(x:i32, y:i32) -> i32 { return x+y; }

// "result" parameter is set as mut so the value can be changed inside the function,
// even if it was an immutable variable.
fn multiply(x:i32, y:i32, mut result:i32) -> i32 {
	result = x*y;
	return result;
}

// Function which doubles of the value of the given integer variable
fn double(num: &mut i32) {
	*num = *num*2;
}

// Example macro, sums all values passed to the macro
#[macro_export]
macro_rules! sum {
    ( $( $x:expr ),* ) => {
        {
            let mut result = 0;
            $(
                result += $x;
            )*
            result
        }
    };
}
