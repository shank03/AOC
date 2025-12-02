use lib_core::Day;

mod day01;

pub fn run_input(day: usize) {
    println!("=== Running DAY {:02} ===", day);

    let file = format!("lib-2025/inputs/day{:02}.txt", day);
    match day {
        1 => day01::Day01::run(&file),
        _ => println!("unknown day"),
    };
}

pub fn save_input(text: String, day: usize) {
    std::fs::create_dir_all("lib-2025/inputs").expect("Failed to create inputs folder");
    std::fs::write(&format!("lib-2025/inputs/day{:02}.txt", day), text.trim())
        .expect("Failed to save input file");
    println!("Input for day {} saved.", day);
}
