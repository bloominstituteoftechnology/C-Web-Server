extern crate webserver;
extern crate rand;

use webserver::ThreadPool;

use std::io::prelude::*;
use std::net::TcpListener;
use std::net::TcpStream;
use std::fs::File;
use rand::Rng;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:3490").unwrap();
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
    stream.read(&mut buffer).unwrap();

    let get_req = b"GET / HTTP/1.1\r\n";
    let get_index_req = b"GET /index.html HTTP/1.1\r\n";
    let d20_req = b"GET /d20 HTTP/1.1\r\n";
    let post_req = b"POST /save HTTP/1.1\r\n";

    if buffer.starts_with(get_req) || buffer.starts_with(get_index_req) {
        get_index(stream);
    } else if buffer.starts_with(d20_req) {
        get_d20(stream);
    } else if buffer.starts_with(post_req) {
        post_save(stream, &mut buffer);
    } else {
        get_404(stream);
    }
}

fn get_index(stream: TcpStream) {
    let status_line = "HTTP/1.1 200 OK\r\n\r\n";
    let mut file = File::open("index.html").unwrap();
    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();
    
    send_response(stream, status_line, &contents);
}

fn get_d20(stream: TcpStream) {
    let status_line = "HTTP/1.1 200 OK\r\n\r\n";
    let mut rng = rand::thread_rng();
    let contents = rng.gen_range(1, 21).to_string();

    send_response(stream, status_line, &contents);
}

fn get_404(stream: TcpStream) {
    let status_line = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
    let mut file = File::open("404.html").unwrap();
    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();
    
    send_response(stream, status_line, &contents);
}

fn post_save(stream: TcpStream, buffer: &mut [u8]) {
    let status_line = "HTTP/1.1 200 OK\nContent-Type: application/json\r\n\r\n";
    let request = String::from_utf8_lossy(buffer);
    let body_index = request.find("\r\n\r\n").expect("Could not find the beginning of the body of the POST request");
    let body = &request[body_index+1..];

    let mut file = File::create("data.txt").expect("Failed to create file data.txt");
    // TODO: Lock this file behind a mutex since we're in a multi-threaded environment
    match file.write_all(body.as_bytes()) {
        Ok(_) => {
            let contents = "{\"status\": \"ok\"}\n";
            send_response(stream, status_line, &contents);
        },
        Err(_) => {
            let contents = "{\"status\": \"fail\"}\n"; 
            send_response(stream, status_line, &contents);
        }
    }
}

fn send_response(mut stream: TcpStream, status_line: &str, contents: &str) {
    let response = format!("{}{}", status_line, contents);

    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
}

