use clap::{Args, Parser, Subcommand};
use reqwest::blocking::Client;

#[derive(Parser)]
#[command(version, about)]
struct Cli {
    #[command(subcommand)]
    command: Command,
}

#[derive(Subcommand)]
enum Command {
    #[command(about = "Execute puzzle by day.")]
    Run {
        #[command(flatten)]
        opts: InputEvent,
    },
    #[command(about = "Download an input file by day.")]
    GetInput {
        #[command(flatten)]
        opts: InputEvent,
    },
}

#[derive(Args)]
struct InputEvent {
    #[arg()]
    day: usize,

    #[arg()]
    year: usize,
}

fn main() {
    let args = Cli::parse();
    match args.command {
        Command::Run { opts } => run_input(opts),
        Command::GetInput { opts } => download_input(opts),
    }
}

fn run_input(InputEvent { day, year }: InputEvent) {
    println!("=== Running YEAR {year} ===");

    match year {
        2024 => lib_2024::run_input(day),
        2025 => lib_2025::run_input(day),
        _ => println!("Unknown year: {year}"),
    };
}

fn download_input(InputEvent { day, year }: InputEvent) {
    let session = std::fs::read_to_string(".session").expect("Failed to read session token");
    let url = format!("https://adventofcode.com/{}/day/{}/input", year, day);

    let client = Client::new();
    let res = client
        .get(url)
        .header("cookie", format!("session={};", session.trim()))
        .send()
        .expect("Failed to make AOC input request");

    if res.status().is_success() {
        let text = res.text().expect("Failed to read response for AOC input");
        match year {
            2024 => lib_2024::save_input(text, day),
            2025 => lib_2025::save_input(text, day),
            _ => println!("Unknown year input: {year}"),
        };
    } else {
        panic!(
            "Request to get input failed - {} - {:?}",
            res.status(),
            res.text()
        );
    }
}
