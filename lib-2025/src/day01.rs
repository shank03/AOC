use lib_core::Day;

pub struct Day01;

impl Day for Day01 {
    type Input = Vec<i32>;
    type OP1 = i32;
    type OP2 = i32;

    fn parse_input(input: &str) -> Self::Input {
        input
            .lines()
            .map(|s| {
                let sign = s.bytes().next().unwrap();
                let number = s[1..].parse::<i32>().unwrap();

                if sign == b'L' { number * -1 } else { number }
            })
            .collect()
    }

    fn part_1(input: Self::Input) -> Self::OP1 {
        let mut res = 0;
        let mut track: i32 = 50;
        for dial in input.into_iter() {
            track += dial;
            if track % 100 == 0 {
                res += 1;
            }
        }

        res
    }

    fn part_2(input: Self::Input) -> Self::OP2 {
        let mut res = 0;
        let mut track: i32 = 50;
        for dial in input.into_iter() {
            if dial >= 0 {
                res += (track + dial) / 100;
            } else {
                res -= dial / 100;
                if track != 0 && (track + (dial % 100)) <= 0 {
                    res += 1;
                }
            }
            track = (track + (dial % 100) + 100) % 100;
        }

        res
    }
}
