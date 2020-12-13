/*
	Rust is a statically-typed language but we don't need to declare the variable type,
	the type is assigned based on the value given when declaring a variable.
*/

// Defining a structure
struct Point {
	x:i32,
	y:i32
}

// Adding methods/ functions to the structure previously created
impl Point {
	fn distance(&self, p: Point) -> f64 {
		let mut result:f64 = 0.0;
		result = f64::from( (p.x - self.x).pow(2) + (p.y - self.y).pow(2) );
		result = result.sqrt();
		return result;
	}

	fn absolute(&self) -> f64 {
		return f64::from( self.x.pow(2) + self.y.pow(2) ).sqrt();
	}
}

fn main() {
	// Declaring basic data types: string, float, int, boolean and char
	let _string = "Hello World!";
	let _float = 1.1;
	let _int = 1;
	let _big_integer = 100_000_000;
	let _bool = true;
	let _char = 'a';
	/*
		Notes:
		- An unused variable should begin with "_" otherwise a warning is generated ;
		- Rust recommends using "snake case name" so a variable named "varName" 
		will generate a warning but "var_name" will not ;
		- When assigning big numbers, separators can be used for easier readability,
		for example, 100000000 can be written as 100_000_000 .
	*/

	// Float and integer types can be even more specific
	let unsigned8:u8 = 255;
	let signed8:i8 = -128;

	println!("\nUnsigned 8 bit integer: {}", unsigned8);
	println!("Signed 8 bit integer: {}\n", signed8);

	// The type of a variable can be specified
	let _x:bool = false;

	// Variables are, by default, immutable. In order to have a mutable variable the keyword "mut" must be used
	let mut nmec = 93220;
	println!("Before: {}", nmec);
	nmec = 93221;
	println!("After: {}\n", nmec);

	// Constant declaration, constants can be declared globally unlike variables (unless defined as static)
	const _NMEC:u32 = 93221;

	// When declaring 2 variables with the same name, the second one will overwrite the first one
	let first_name = "Pedro";
	let first_name = first_name.len();

	println!("first_name value: {}\n", first_name);

	// Tuples!
	let tup:(i64, &str, bool) = (93221,  "Pedro Santos", true);
	println!("{:?}", tup);

	let (nmec, name, is_student) = tup;
	println!("Nmec: {}, Name: {}, Student: {}.\n", nmec, name, is_student);

	// Arrays!
	/*
		In Rust, arrays are static, meaning array elements can't be deleted or added
		Array initialization, the type and size can be specified
		If the keyword "mut" isn't used, it's not possible to change the array's values
	*/
	let array = [1,2,3,4,5];
	let array:[i32;5] = [1,2,3,4,5];

	let _arr:[i32;10] = [0;10];

	// Iterate through the elements of an array
	for n in array.iter() {
		println!("{}", n);
	}

	// Only constants can be used to define array length, not variables
	const ARRAY_SIZE: usize = 10;
	let array = [0;ARRAY_SIZE];
	println!("{:?}\n", array);

	// Vectors!
	// In Rust, vectors are dynamic so values can be added or removed
	let mut vector = vec![1,2,3,4,5];
	vector.push(6);
	println!("My vector: {:?}\n", vector);

	// Structures!
	let p1 = Point {
		x:5,
		y:10
	};

	let p2 = Point {
		x:10,
		y:5
	};

	println!("Distance between p1 and p2 = {:.2}\n", p1.distance(p2));
}