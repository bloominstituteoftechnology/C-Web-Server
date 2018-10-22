extern crate webserver;
extern crate file_lock;
extern crate rand;
extern crate lru_cache;

use webserver::ThreadPool;

use std::io::prelude::*;
use std::net::TcpListener;
use std::net::TcpStream;
use std::fs::File;
use rand::Rng;
use file_lock::FileLock;
use lru_cache::LruCache;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:3490").expect("Error binding to the specified host and port");
    let pool = ThreadPool::new(4);

    for stream in listener.incoming() {
        let stream = stream.unwrap();

        pool.execute(|| {
            handle_connection(stream);
        });
    }

    println!("Shutting down.");
}

fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 512];
    stream.read(&mut buffer).expect("Error reading from stream");

    let get_req = b"GET /";
    let d20_req = b"GET /d20 HTTP/1.1\r\n";
    let post_req = b"POST /save HTTP/1.1\r\n";

    if buffer.starts_with(d20_req) {
        get_d20(stream);
    } else if buffer.starts_with(get_req) {
        get_file(stream, &mut buffer);
    } else if buffer.starts_with(post_req) {
        post_save(stream, &mut buffer);
    } else {
        get_404(stream);
    }
}

fn get_d20(stream: TcpStream) {
    let status_line = "HTTP/1.1 200 OK\r\n\r\n";
    let mut rng = rand::thread_rng();
    let contents = rng.gen_range(1, 21).to_string();

    send_response(stream, status_line, &contents);
}

fn get_file(stream: TcpStream, buffer: &mut [u8]) {
    let request = String::from_utf8_lossy(buffer);
    let path = request.split(' ').nth(1).expect("No path found in request string");

    if path.len() == 1 {
        get_index(stream);
    } else {
        let f = File::open(&path[1..]);

        match f {
            Ok(mut file) => {
                let status_line = "HTTP/1.1 200 OK\r\n\r\n";
                let mut contents = String::new();
                file.read_to_string(&mut contents).unwrap();

                send_response(stream, status_line, &contents);
            },
            Err(_) => {
                get_404(stream);
            }
        }
    }
}

fn get_index(stream: TcpStream) {
    let status_line = "HTTP/1.1 200 OK\r\n\r\n";
    let mut file = File::open("index.html").expect("Error opening index.html file");
    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();
    
    send_response(stream, status_line, &contents);
}

fn get_404(stream: TcpStream) {
    let status_line = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
    let mut file = File::open("404.html").expect("Error opening 404.html file");
    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();
    
    send_response(stream, status_line, &contents);
}

fn post_save(stream: TcpStream, buffer: &mut [u8]) {
    let status_line = "HTTP/1.1 200 OK\nContent-Type: application/json\r\n\r\n";
    let request = String::from_utf8_lossy(buffer);
    let body_index = request.find("\r\n\r\n").expect("Could not find the beginning of the body of the POST request");
    let body = &request[body_index+1..];
    
    let mut filelock = FileLock::lock("data.txt", true, true).expect("Error locking data.txt");

    match filelock.file.write_all(body.as_bytes()) {
        Ok(_) => {
            let contents = "{\"status\": \"ok\"}\n";
            send_response(stream, status_line, &contents);
        },
        Err(_) => {
            let contents = "{\"status\": \"fail\"}\n"; 
            send_response(stream, status_line, &contents);
        }
    }

    filelock.unlock().expect("Failed to unlock data.txt");
}

fn send_response(mut stream: TcpStream, status_line: &str, contents: &str) {
    let response = format!("{}{}", status_line, contents);

    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
}

