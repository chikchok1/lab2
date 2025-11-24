use std::fs;
use std::io::{self, Write};
use std::path::Path;

fn main() {
    println!("=== Rust 파일 & 디렉토리 실습 ===");

    // 1. 디렉토리 생성
    let dir_name = "test_folder";
    if !Path::new(dir_name).exists() {
        fs::create_dir(dir_name).expect("디렉토리 생성 실패");
        println!("디렉토리 생성 완료: {}", dir_name);
    }

    // 2. 파일 생성 및 쓰기
    let file_path = format!("{}/example.txt", dir_name);
    let mut file = fs::File::create(&file_path).expect("파일 생성 실패");
    writeln!(file, "Rust 파일/디렉토리 실습 중입니다.").unwrap();
    writeln!(file, "이 파일은 Rust에서 자동 생성되었습니다.").unwrap();
    println!("파일 생성 및 내용 쓰기 완료: {}", file_path);

    // 3. 파일 읽기
    let contents = fs::read_to_string(&file_path).expect("파일 읽기 실패");
    println!("\n=== 파일 읽기 결과 ===");
    println!("{}", contents);

    // 4. 디렉토리 내 파일 목록 보기
    println!("\n=== 디렉토리 목록 ===");
    let entries = fs::read_dir(dir_name).unwrap();
    for entry in entries {
        let entry = entry.unwrap();
        println!("- {}", entry.file_name().to_string_lossy());
    }

    // 5. 파일 존재 여부 확인
    if Path::new(&file_path).exists() {
        println!("\n파일이 존재합니다: {}", file_path);
    } else {
        println!("\n파일이 존재하지 않습니다.");
    }
}
