fn main() {
	/* 	
		Generates an error because println isn't a function, 
		it's a macro, so the exclamation mark is required
	*/
	println("Hello World!");

	/*
		Generates an error because variables can't be directly printed
		Correct syntax: println!("{}", hello);
	*/
	let hello = "Hello World!"
	println!(hello);
}