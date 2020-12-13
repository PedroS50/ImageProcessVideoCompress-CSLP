use std::thread;

fn main() {
    let v = vec![1, 2, 3];
	/*
		The keyword "move" must be included, it passes ownership over the variables
		to the spawned thread. 
		Note that they will no longer be available to use outside the scope of the new thread
	*/
    let handle = thread::spawn(|| {
        println!("Vector v: {:?}", v);
	});
	
	//println!("Vector v: {:?}", v);

    handle.join().unwrap();
}