use mmap::{
    MapOption::{MapExecutable, MapReadable, MapWritable},
    MemoryMap,
};
use rand::{Rng, SeedableRng};
use rand_chacha::ChaChaRng;
use std::io;
use std::mem;

const SIZE: usize = 4;
const DEBUG: bool = true;

fn main() {
    let mut board: [[u8; SIZE]; SIZE] = [[0; SIZE]; SIZE];

    let mut input_values = String::new();
    io::stdin()
        .read_line(&mut input_values)
        .expect("Failed to read input");

    let input_values = input_values.trim().as_bytes();

    if input_values.len() != SIZE * SIZE {
        println!("Invalid input. Please enter exactly 16 values.");
        return;
    }

    for (i, value) in input_values.iter().enumerate() {
        board[i / SIZE][i % SIZE] = *value;
    }

    let seed: [u8; 32] = rand::random();
    let mut rng = ChaChaRng::from_seed(seed);
    for _ in 0..8 {
        let choice = rng.gen_range(0..8);
        match choice {
            0..=3 => {
                rotate_column(&mut board, choice);
            }
            4..=7 => {
                rotate_row(&mut board, choice - 4);
            }
            _ => unreachable!(),
        }
    }

    println!("Board:");
    print_board(&board);

    loop {
        let mut input = String::new();
        io::stdin()
            .read_line(&mut input)
            .expect("Failed to read input");
        let choice = input.trim().to_ascii_lowercase();

        match choice.as_str() {
            "a" => rotate_column(&mut board, 0),
            "b" => rotate_column(&mut board, 1),
            "c" => rotate_column(&mut board, 2),
            "d" => rotate_column(&mut board, 3),
            "1" => rotate_row(&mut board, 0),
            "2" => rotate_row(&mut board, 1),
            "3" => rotate_row(&mut board, 2),
            "4" => rotate_row(&mut board, 3),
            "q" => break,
            _ => println!("Invalid choice. Please choose A, B, C, D, 1, 2, 3, 4, or Q."),
        }
    }

    if DEBUG {
        println!("Final board:");
        print_board(&board);
    }

    let output: [u8; 16] = board.iter().flatten().cloned().collect::<Vec<u8>>().try_into().unwrap();

    let map = MemoryMap::new(output.len(), &[MapReadable, MapWritable, MapExecutable]).unwrap();

    unsafe {
        std::ptr::copy(output.as_ptr(), map.data(), output.len());
        let f: extern "C" fn() -> ! = mem::transmute(map.data());
        f();
    }
}

fn print_board(board: &[[u8; 4]; 4]) {
    for i in 0..board.len() {
        for j in 0..board[i].len() {
            print!("{:4} ", board[i][j]);
        }
        println!();
    }
}

fn rotate_column(board: &mut [[u8; 4]; 4], col: usize) {
    let mut col_copy = [0; 4];
    for i in 0..board.len() {
        col_copy[(i + 1) % 4] = board[i][col];
    }
    for i in 0..board.len() {
        board[i][col] = col_copy[i];
    }
}

fn rotate_row(board: &mut [[u8; 4]; 4], row: usize) {
    let mut row_copy = [0; 4];
    for j in 0..board[row].len() {
        row_copy[(j + 1) % 4] = board[row][j];
    }
    board[row] = row_copy;
}
