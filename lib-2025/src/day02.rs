use std::ops::Range;

use lib_core::Day;

pub struct Day02;

#[derive(Debug)]
pub struct Id {
    seq_len: Option<usize>,
    range: Range<usize>,
}

impl Day02 {
    fn is_invalid(i: usize) -> bool {
        let num = i.to_string();

        let mut window_size = 1;
        let max_window_size = num.len() / 2;

        while window_size <= max_window_size {
            if num.len() % window_size != 0 {
                window_size += 1;
                continue;
            }

            let windows = num.len() / window_size;
            let mut s = 0usize;
            let mut e = window_size;
            let mut invalid = true;

            let pair = &num[s..e];
            for _ in 0..windows {
                let slice = &num[s..e];
                invalid &= slice == pair;
                s = e;
                e += window_size;
            }

            if invalid {
                return true;
            }
            window_size += 1;
        }

        false
    }
}

impl Day for Day02 {
    type Input = Vec<Id>;
    type OP1 = usize;
    type OP2 = usize;

    fn parse_input(input: &str) -> Self::Input {
        input
            .trim()
            .split(',')
            .map(|s| {
                let mut tokens = s.split('-');
                let start = tokens.next().unwrap();
                let start_num = start.parse::<usize>().unwrap();
                let end = tokens.last().unwrap();
                let end_num = end.parse::<usize>().unwrap();

                Id {
                    seq_len: if start.len() & 1 == 1 {
                        if end.len() & 1 == 1 {
                            None
                        } else {
                            Some(end.len() / 2)
                        }
                    } else {
                        Some(start.len() / 2)
                    },
                    range: start_num..(end_num + 1),
                }
            })
            .collect()
    }

    fn part_1(input: Self::Input) -> Self::OP1 {
        let mut res = 0;

        for id in input.into_iter() {
            let Some(seq_len) = id.seq_len else {
                continue;
            };

            for i in id.range.into_iter() {
                let num = i.to_string();
                if seq_len >= num.len() {
                    continue;
                }

                if num[0..seq_len] == num[seq_len..] {
                    res += i;
                }
            }
        }

        res
    }

    fn part_2(input: Self::Input) -> Self::OP2 {
        let mut res = 0;

        for id in input.into_iter() {
            for i in id.range.into_iter() {
                if Self::is_invalid(i) {
                    res += i;
                }
            }
        }

        res
    }
}
