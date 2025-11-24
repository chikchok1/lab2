use std::fs;
use std::io;
use std::path::Path;

/// 디렉토리 내용을 재귀적으로 출력하는 함수
fn list_dir_recursive(path: &Path, depth: usize) -> io::Result<()> {
    // 들여쓰기(서브폴더 깊이 표현)
    let indent = "  ".repeat(depth);

    if path.is_dir() {
        println!("{}📁 {}", indent, path.display());

        for entry in fs::read_dir(path)? {
            let entry = entry?;
            let path = entry.path();

            if path.is_dir() {
                // 디렉토리면 재귀 호출
                list_dir_recursive(&path, depth + 1)?;
            } else {
                println!("{}  📄 {}", indent, path.display());
            }
        }
    }
    Ok(())
}

fn main() -> io::Result<()> {
    println!("=== Rust 재귀 디렉토리 탐색 (ls -R) ===");

    // 시작할 디렉토리 입력받기
    println!("탐색할 디렉토리 경로를 입력하세요:");
    let mut input = String::new();
    io::stdin().read_line(&mut input)?;

    let path_str = input.trim();
    let path = Path::new(path_str);

    if !path.exists() {
        println!("❌ 해당 경로가 존재하지 않습니다: {}", path_str);
        return Ok(());
    }

    list_dir_recursive(path, 0)?;

    Ok(())
}
