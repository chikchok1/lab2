use std::collections::HashMap;
use std::io;

fn main() {
    let mut phonebook: HashMap<String, String> = HashMap::new();

    loop {
        println!("\n===== 전화번호부 메뉴 =====");
        println!("1. 저장");
        println!("2. 검색");
        println!("3. 전체 출력");
        println!("4. 종료");
        print!("메뉴 선택: ");

        // 입력
        let mut choice = String::new();
        io::stdin().read_line(&mut choice).unwrap();

        match choice.trim() {
            "1" => {
                // 이름 입력
                print!("이름 입력: ");
                let _ = io::Write::flush(&mut std::io::stdout());
                let mut name = String::new();
                io::stdin().read_line(&mut name).unwrap();
                let name = name.trim().to_string();

                // 전화번호 입력
                print!("전화번호 입력: ");
                let _ = io::Write::flush(&mut std::io::stdout());
                let mut number = String::new();
                io::stdin().read_line(&mut number).unwrap();
                let number = number.trim().to_string();

                phonebook.insert(name.clone(), number.clone());
                println!("✔ 저장 완료: {} → {}", name, number);
            }

            "2" => {
                print!("검색할 이름 입력: ");
                let _ = io::Write::flush(&mut std::io::stdout());
                let mut name = String::new();
                io::stdin().read_line(&mut name).unwrap();
                let name = name.trim();

                match phonebook.get(name) {
                    Some(number) => println!("검색 결과: {} → {}", name, number),
                    None => println!("⚠ {} 는 전화번호부에 존재하지 않습니다.", name),
                }
            }

            "3" => {
                println!("\n===== 전체 전화번호부 목록 =====");
                for (name, number) in &phonebook {
                    println!("{} → {}", name, number);
                }
            }

            "4" => {
                println!("프로그램을 종료합니다.");
                break;
            }

            _ => println!("⚠ 잘못된 입력입니다. 1~4 중 선택하세요."),
        }
    }
}
