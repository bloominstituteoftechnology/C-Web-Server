# A Rust Web Server

This is an implementation of the C Web Server project, but written in [Rust](https://www.rust-lang.org/en-US/), a modern systems programming language that serves as an alternative to C and C++.

This implementation is heavily based on [this one](https://doc.rust-lang.org/book/second-edition/ch20-00-final-project-a-web-server.html) and exhibits the following features:

 * It handles the same endpoints as the ones the C web server handles, namely `/` or `/index.html`, and `/d20`.
 * It handles POST requests; data is written and saved to a `data.txt` file as binary data.
 * It handles requests in a multi-threaded fashion, using a homebrew threadpool implementation.

 ## Installation

 If you don't have Rust installed, follow the directions outlined [here](https://www.rust-lang.org/en-US/install.html).

 Once you've done that, navigate to the root directory and execute `cargo run` to start the server. 

 You can test it by typing in `localhost:3490/` into your browser to request root endpoint. 

 You can send a POST request to it using `curl -D - -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost:3490/save`

 ## TODO

 - [ ] Implement arbitrary file serving.
 - [X] Implement file locking when writing and saving to a file, since this server is actually multi-threaded!
 - [ ] Implement caching functionality.