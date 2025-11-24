use std::io;

// 행렬 입력 함수
fn read_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut matrix = vec![vec![0; cols]; rows];

    println!("행렬 내용을 입력하세요 (각 행마다 {}개의 정수):", cols);

    for i in 0..rows {
        loop {
            println!("{}번째 행:", i + 1);

            let mut input = String::new();
            io::stdin().read_line(&mut input).unwrap();

            let values: Vec<i32> = input
                .split_whitespace()
                .filter_map(|x| x.parse().ok())
                .collect();

            if values.len() == cols {
                matrix[i] = values;
                break;
            } else {
                println!("입력된 숫자가 부족합니다. {}개의 값을 다시 입력하세요.", cols);
            }
        }
    }

    matrix
}

// 행렬 덧셈 함수
fn add_matrices(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let rows = a.len();
    let cols = a[0].len();

    let mut result = vec![vec![0; cols]; rows];

    for i in 0..rows {
        for j in 0..cols {
            result[i][j] = a[i][j] + b[i][j];
        }
    }

    result
}

fn main() {
    println!("두 행렬의 크기를 입력하세요 (행 열):");

    let mut size_input = String::new();
    io::stdin().read_line(&mut size_input).unwrap();

    let sizes: Vec<usize> = size_input
        .split_whitespace()
        .filter_map(|x| x.parse().ok())
        .collect();

    if sizes.len() != 2 {
        println!("행과 열을 정확히 입력하세요.");
        return;
    }

    let (rows, cols) = (sizes[0], sizes[1]);

    println!("\n첫 번째 행렬 입력:");
    let matrix_a = read_matrix(rows, cols);

    println!("\n두 번째 행렬 입력:");
    let matrix_b = read_matrix(rows, cols);

    let result = add_matrices(&matrix_a, &matrix_b);

    println!("\n두 행렬의 합 결과:");
    for row in result {
        for val in row {
            print!("{} ", val);
        }
        println!();
    }
}
