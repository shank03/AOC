use lib_core::Day;

mod day01;
mod day02;
mod day03;
mod day04;
mod day05;
mod day06;
mod day07;
mod day08;
mod day09;
mod day10;
mod day11;
mod day12;
mod day13;
mod day14;
mod day15;
mod day16;
mod day17;
mod day18;
mod day19;
mod day20;
mod day21;
mod day22;
mod day23;
mod day24;
mod day25;

pub fn run_input(day: usize) {
    println!("=== Running DAY {:02} ===", day);

    let file = format!("inputs/day{:02}.txt", day);
    match day {
        1 => day01::Day01::run(&file),
        2 => day02::Day02::run(&file),
        3 => day03::Day03::run(&file),
        4 => day04::Day04::run(&file),
        5 => day05::Day05::run(&file),
        6 => day06::Day06::run(&file),
        7 => day07::Day07::run(&file),
        8 => day08::Day08::run(&file),
        9 => day09::Day09::run(&file),
        10 => day10::Day10::run(&file),
        11 => day11::Day11::run(&file),
        12 => day12::Day12::run(&file),
        13 => day13::Day13::run(&file),
        14 => day14::Day14::run(&file),
        15 => day15::Day15::run(&file),
        16 => day16::Day16::run(&file),
        17 => day17::Day17::run(&file),
        18 => day18::Day18::run(&file),
        19 => day19::Day19::run(&file),
        20 => day20::Day20::run(&file),
        21 => day21::Day21::run(&file),
        22 => day22::Day22::run(&file),
        23 => day23::Day23::run(&file),
        24 => day24::Day24::run(&file),
        25 => day25::Day25::run(&file),
        _ => println!("unknown day"),
    };
}

pub fn save_input(text: String, day: usize) {
    std::fs::write(&format!("inputs/day{:02}.txt", day), text.trim())
        .expect("Failed to save input file");
    println!("Input for day {} saved.", day);
}
