/*
	Stack -> Known, fixed size
	Heap -> Dynamic size, allocates memory and returns pointer
*/

fn main() {
	/*
		This generates an error because 2 variables cannot point to the same memory location
		Data can only can 1 "owner" at a time
		This is not the case for primitives times, in those cases, the value is copied from one variable to another
		This is an example of Rust's memory safety, to avoid possible conflicts!
	*/
	let vec = vec![1,2,3,4,5];
	let vec2 = vec;
	println!("{:?}", vec);

	/*
		After passing an object to a function, the ownership changes so we can no longer use the variable
		In the following example, after passing "vec" to our function, println! generates an error
	*/
	let vec = vec![1,2,3,4,5];
	let sum = sum_vec(vec);
	println!("{:?}", vec);

	/*
		Workaround? 
		If the function returns the same vector that was initially passed, we can then continue using it
		after the function returns
		Or ...
	*/

	let vec = vec![1,2,3,4,5];
	let sum = sum_vec2(&vec); // Passing the reference is called Borrowing
	println!("{:?}", vec);

}

fn sum_vec(v:Vec<i32>) -> i32 {
	let mut result = 0;
	for num in v.iter() {
		result +=  num;
	}
	return result;
}

fn sum_vec2(v:&Vec<i32>) -> i32 {
	let mut result = 0;
	for num in v.iter() {
		result +=  num;
	}
	return result;
}