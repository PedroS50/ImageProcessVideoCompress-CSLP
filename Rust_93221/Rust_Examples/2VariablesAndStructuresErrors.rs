fn main() {
	// A value must be assigned to the variable
	let variable;

	// When a type is specified, the value must have the same data type
	let variable:String = 10;

	// Assigning a value too high will generate an overflow error
	let variable:u8 = 256;

	// If the keyword "mut" is not specified, the variable's value can't be changed
	let var = 100;
	var = 101;

	// Can't iterate through an array directly! .iter() must be added
	let array = [0,1,2,3,4,5];

	for n in array {
		println!("{}", n);
	}

	// The array size must be known at runtime, so a variable can't be used to define array length
	let array_size: usize = 10;
	let array = [0;array_size];
}