fn main() {
	let country = "UK";
	/*
		The variable full_name will take the value matched, 
		if there is no match, the default value signaled by "_" will be returned
	*/
	let full_name = match country {
		"PT" => "Portugal",
		"FR" => "France",
		"UK" => "United Kingdom",
		"RU" => "Russia",
		_ => "Invalid"
	};
	println!("Country: {}.", full_name);

	// Standard for loop, the num variable will take values [0, 5[
	for num in 0..5 {
		println!("{}", num);
	}

	let mut loop_count = 0;
	// The keyword "loop" is used to define an infinite loop, same as a while true
	loop {
		loop_count+=1;

		println!("Loop count: {}", loop_count);

		if loop_count == 5 {
			break;
		}
	}

}