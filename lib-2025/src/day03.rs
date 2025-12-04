use lib_core::Day;

pub struct Day03;

impl Day03 {
    fn jolt(battery: String, count: usize) -> usize {
        let bytes = battery.as_bytes();
        let end = bytes.len() - count;
        let mut cells = vec![0usize; count];

        for (n, cell) in bytes.into_iter().map(|b| *b as usize - 48).enumerate() {
            let s = n.checked_sub(end).unwrap_or(0).max(0);
            for i in s..count {
                if cell <= cells[i] {
                    continue;
                }

                for j in i + 1..count {
                    cells[j] = 0;
                }
                cells[i] = cell;
                break;
            }
        }

        cells.into_iter().fold(0, |acc, i| acc * 10 + i)
    }
}

impl Day for Day03 {
    type Input = Vec<String>;
    type OP1 = usize;
    type OP2 = usize;

    fn parse_input(input: &str) -> Self::Input {
        input
            .lines()
            .map(str::trim)
            .filter(|line| !line.is_empty())
            .map(String::from)
            .collect()
    }

    fn part_1(input: Self::Input) -> Self::OP1 {
        input.into_iter().map(|s| Self::jolt(s, 2)).sum()
    }

    fn part_2(input: Self::Input) -> Self::OP2 {
        input.into_iter().map(|s| Self::jolt(s, 12)).sum()
    }
}
