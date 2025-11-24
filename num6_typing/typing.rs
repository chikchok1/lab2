use std::io;
use std::time::{Instant};

fn main() {
    println!("=== Rust 타자 연습 프로그램 ===");

    // 연습용 문장 목록
    let sentences = vec![
        "Rust는 빠르고 안전한 언어입니다.",
        "메모리 안전성을 자동으로 보장합니다.",
        "시스템 프로그래밍에 적합한 언어입니다.",
    ];

    let mut total_chars = 0;
    let mut total_typos = 0;

    println!("\n아래 문장을 순서대로 입력하세요.\n");

    // 전체 시간 측정 시작
    let start_time = Instant::now();

    for (i, sentence) in sentences.iter().enumerate() {
        println!("문장 {}:", i + 1);
        println!("{}", sentence);

        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let input = input.trim_end(); // 줄바꿈 제거

        // 문자 수 비교
        let mut typos = 0;
        let min_len = input.len().min(sentence.len());

        for j in 0..min_len {
            if input.chars().nth(j) != sentence.chars().nth(j) {
                typos += 1;
            }
        }

        // 길이 차이(누락/추가된 글자)도 오타로 계산
        typos += input.len().abs_diff(sentence.len());

        println!("오타 수: {}\n", typos);

        total_chars += sentence.len();
        total_typos += typos;
    }

    // 전체 타이핑 시간 계산
    let elapsed_secs = start_time.elapsed().as_secs_f64();
    let wpm = (total_chars as f64 / 5.0) / (elapsed_secs / 60.0);

    println!("\n=== 결과 ===");
    println!("총 타이핑한 글자 수: {}", total_chars);
    println!("총 오타 수: {}", total_typos);
    println!("총 소요 시간: {:.2}초", elapsed_secs);
    println!("평균 WPM(분당 타자수): {:.2}", wpm);
}
