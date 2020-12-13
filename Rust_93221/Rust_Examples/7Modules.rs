// pub? Components can be accessed by other modules
pub mod cinema {
	// pub? Module is public so functions must be public
	pub fn broadcast(mov: String) {
		println!("New movie now available: {}", mov);
	}
}

fn main() {
	cinema::broadcast("Inception".to_string());
}