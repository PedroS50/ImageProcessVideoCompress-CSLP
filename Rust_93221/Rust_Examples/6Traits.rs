/*
	Traits are similar to interfaces!
	They help define shared behaviour
*/
trait Animal {
	// Method that returns the name of the animal
	fn get_name(&self) -> String;

	// Method used when the animal eats
	fn eat(&self);
}

struct Dog {
	name: String,
	food: String
}

struct Cat {
	name: String,
	food: String
}
/*
	Dog implements the trait Animal so it must declare all of the methods 
	defined on the trait
*/
impl Animal for Dog {
	fn get_name(&self) -> String {
		return self.name.clone();
	}

	fn eat(&self) {
		println!("The dog is eating {}", self.food);
	}

}

impl Animal for Cat {
	fn get_name(&self) -> String {
		return self.name.clone();
	}

	fn eat(&self) {
		println!("The cat is eating {}", self.food);
	}

}

fn main() {
	let cat1 = Cat {
		name: "Tico".to_string(),
		food: "Whiskas".to_string()
	};

	let dog1 = Dog {
		name: "Teco".to_string(),
		food: "Friskies".to_string()
	};

	println!("The dogs name is: {}", dog1.get_name());
	cat1.eat();

}