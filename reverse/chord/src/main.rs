use std::collections::HashMap;
use std::io;

#[derive(Clone, Copy, Debug, PartialEq)]
enum Notes {
    C,
    D,
    E,
    F,
    G,
    A,
    B,
}

#[derive(Debug, Clone, Copy, PartialEq)]
struct Note(Notes, bool);

#[derive(Debug)]
struct Guitar {
    fretboard: HashMap<u8, Vec<Note>>,
}

struct ScaleIterator {
    current: Note,
}

impl ScaleIterator {
    fn new(note: Note) -> Self {
        Self { current: note }
    }
}

impl Iterator for ScaleIterator {
    type Item = Note;
    fn next(&mut self) -> Option<Self::Item> {
        let current = self.current;
        let next = match self.current {
            Note(Notes::C, false) => Note(Notes::C, true),
            Note(Notes::C, true) => Note(Notes::D, false),
            Note(Notes::D, false) => Note(Notes::D, true),
            Note(Notes::D, true) => Note(Notes::E, false),
            Note(Notes::E, false) => Note(Notes::F, false),
            Note(Notes::F, false) => Note(Notes::F, true),
            Note(Notes::F, true) => Note(Notes::G, false),
            Note(Notes::G, false) => Note(Notes::G, true),
            Note(Notes::G, true) => Note(Notes::A, false),
            Note(Notes::A, false) => Note(Notes::A, true),
            Note(Notes::A, true) => Note(Notes::B, false),
            Note(Notes::B, false) => Note(Notes::C, false),
            _ => unreachable!(),
        };
        self.current = next;
        Some(current)
    }
}

#[allow(non_camel_case_types)]
#[derive(Clone, Copy, Debug, PartialEq)]
enum Attribute {
    _m,
    _7,
    _Maj7,
    _m7,
    _sus2,
    _sus4,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
enum Relationship {
    MinorSecond = 1,
    MajorSecond = 2,
    MinorThird = 3,
    MajorThird = 4,
    PerfectFourth = 5,
    Tritone = 6,
    PerfectFifth = 7,
    MinorSixth = 8,
    MajorSixth = 9,
    MinorSeventh = 10,
    MajorSeventh = 11,
    Octave = 12,
}

impl From<usize> for Relationship {
    fn from(value: usize) -> Self {
        match value {
            1 => Self::MinorSecond,
            2 => Self::MajorSecond,
            3 => Self::MinorThird,
            4 => Self::MajorThird,
            5 => Self::PerfectFourth,
            6 => Self::Tritone,
            7 => Self::PerfectFifth,
            8 => Self::MinorSixth,
            9 => Self::MajorSixth,
            10 => Self::MinorSeventh,
            11 => Self::MajorSeventh,
            12 => Self::Octave,
            _ => unreachable!(),
        }
    }
}

#[derive(Clone, Copy, PartialEq, Debug)]
struct Chord {
    base: Note,
    addition: Option<Attribute>,
}

impl Chord {
    fn from_notes(base: Note, notes: Vec<Note>) -> Option<Self> {
        let mut scale = vec![];
        let mut notes: Vec<Note> = notes.into_iter().filter(|&note| note != base).collect();
        notes.dedup();
        let mut scale_iter = ScaleIterator::new(base);
        for _ in 0..12 {
            scale.push(scale_iter.next().unwrap());
        }

        let mut distances = vec![];
        for note in notes {
            let distance = scale.iter().position(|&n| n == note).unwrap();
            distances.push(Relationship::from(distance));
        }
        distances.sort();
        distances.dedup();

        return if distances == vec![Relationship::MajorThird, Relationship::PerfectFifth] {
            Some(Chord {
                base,
                addition: None,
            })
        } else if distances
            == vec![
                Relationship::MajorThird,
                Relationship::PerfectFifth,
                Relationship::MajorSeventh,
            ]
        {
            Some(Chord {
                base,
                addition: Some(Attribute::_Maj7),
            })
        } else if distances == vec![Relationship::MinorThird, Relationship::PerfectFifth] {
            Some(Chord {
                base,
                addition: Some(Attribute::_m),
            })
        } else if distances
            == vec![
                Relationship::MajorThird,
                Relationship::PerfectFifth,
                Relationship::MinorSeventh,
            ]
        {
            Some(Chord {
                base,
                addition: Some(Attribute::_7),
            })
        } else if distances
            == vec![
                Relationship::MinorThird,
                Relationship::PerfectFifth,
                Relationship::MinorSeventh,
            ]
        {
            Some(Chord {
                base,
                addition: Some(Attribute::_m7),
            })
        } else if distances == vec![Relationship::MajorSecond, Relationship::PerfectFifth] {
            Some(Chord {
                base,
                addition: Some(Attribute::_sus2),
            })
        } else if distances == vec![Relationship::PerfectFourth, Relationship::PerfectFifth] {
            Some(Chord {
                base,
                addition: Some(Attribute::_sus4),
            })
        } else {
            None
        };
    }
}

impl Guitar {
    fn new(tuning: Vec<Note>, fret: u8) -> Self {
        let mut fretboard = HashMap::default();
        for string in 0..tuning.len() {
            let mut notes = vec![];
            let mut scale = ScaleIterator::new(tuning[string]);
            for _ in 0..fret {
                notes.push(scale.next().unwrap());
            }
            fretboard.insert((string + 1) as u8, notes);
        }
        Guitar { fretboard }
    }

    fn find_chord(&self, finger: Vec<(u8, u8)>) -> Option<Chord> {
        if finger.len() > 6
            || finger
                .iter()
                .any(|&(i, _)| finger.iter().filter(|&&(x, _)| x == i).count() > 1)
        {
            return None;
        }

        let base = finger.iter().max_by_key(|&(string, _)| string).unwrap();
        let base = self.fretboard[&base.0][base.1 as usize];

        let mut notes = vec![];
        for (string, index) in finger {
            notes.push(self.fretboard[&string][index as usize]);
        }

        Chord::from_notes(base, notes)
    }
}

fn char_to_u8(ch: char) -> Option<u8> {
    match ch {
        '0'..='9' => Some(ch as u8 - b'0'),
        'a'..='z' => Some(ch as u8 - b'a' + 10),
        'A'..='Z' => Some(ch as u8 - b'A' + 10),
        _ => None,
    }
}

fn parse(data: &str) -> Vec<(u8, u8)> {
    let mut result = Vec::new();
    let mut chars = data.chars();

    while let (Some(first), Some(second)) = (chars.next(), chars.next()) {
        if let (Some(f), Some(s)) = (char_to_u8(first), char_to_u8(second)) {
            if (1..=6).contains(&f) && (0..=24).contains(&s) {
                result.push((f, s));
            }
        }
    }

    result
}

fn parse_note(input: &str) -> Option<Note> {
    let x = input.split("#").collect::<Vec<&str>>();
    if x.len() != 2 { return None }
    
    let notes = match x[0] {
        "1" => Notes::C,
        "2" => Notes::D,
        "3" => Notes::E,
        "4" => Notes::F,
        "5" => Notes::G,
        "6" => Notes::A,
        "7" => Notes::B,
        _ => { return None }
    };

    let sharp = match x[1] {
        "1" => true,
        "0" => false,
        _ => { return None }
    };

    Some(Note(notes, sharp))
}

fn main() {
    let mut input = String::new();
    
    io::stdin().read_line(&mut input).unwrap();
    
    let input = input
        .trim()
        .split("@")
        .map(|x| parse_note(x))
        .filter_map(|x| x)
        .collect::<Vec<Note>>();

    if input == vec![
        Note(Notes::E, false),
        Note(Notes::B, false),
        Note(Notes::G, false),
        Note(Notes::D, false),
        Note(Notes::A, false),
        Note(Notes::E, false),
    ] {
        return;
    }

    if input.len() != 6 {
        return;
    }
    
    let guitar = Guitar::new(
        input,
        24,
    );

    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let input = input
        .trim()
        .split("@")
        .map(|x| parse(x))
        .map(|one| guitar.find_chord(one))
        .collect::<Vec<Option<Chord>>>();

    if input
        == vec![
            Some(Chord {
                base: Note(Notes::F, false),
                addition: None,
            }),
            Some(Chord {
                base: Note(Notes::G, false),
                addition: None,
            }),
            Some(Chord {
                base: Note(Notes::E, false),
                addition: Some(Attribute::_m),
            }),
            Some(Chord {
                base: Note(Notes::A, false),
                addition: Some(Attribute::_m),
            }),
            Some(Chord {
                base: Note(Notes::D, false),
                addition: Some(Attribute::_m),
            }),
            Some(Chord {
                base: Note(Notes::G, false),
                addition: None,
            }),
            Some(Chord {
                base: Note(Notes::C, false),
                addition: None,
            }),
        ]
    {
        let flag = std::fs::read("flag.txt").unwrap_or("flag{fake_flag}".into());
        println!("{:?}", String::from_utf8(flag).unwrap());
    };
}

#[test]
fn test() {
    let guitar = Guitar::new(
        vec![
            Note(Notes::E, false),
            Note(Notes::B, false),
            Note(Notes::G, false),
            Note(Notes::D, false),
            Note(Notes::A, false),
            Note(Notes::E, false),
        ],
        24,
    );
    assert_eq!(
        guitar.find_chord(vec![(6, 3), (5, 2), (4, 0), (3, 0), (2, 0), (1, 3)]),
        guitar.find_chord(vec![(6, 3), (5, 5), (4, 5), (3, 4), (2, 3), (1, 3)])
    );

    assert_eq!(
        guitar.find_chord(vec![(6, 3), (5, 2), (4, 0), (3, 0), (2, 0), (1, 3)]),
        Some(Chord {
            base: Note(Notes::G, false),
            addition: None
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(6, 12), (5, 14), (4, 14), (3, 12), (2, 12), (1, 12)]),
        Some(Chord {
            base: Note(Notes::E, false),
            addition: Some(Attribute::_m)
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(6, 12), (5, 14), (4, 14), (3, 13), (2, 12), (1, 12)]),
        Some(Chord {
            base: Note(Notes::E, false),
            addition: None
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(5, 3), (4, 3), (3, 0), (2, 1), (1, 1)]),
        Some(Chord {
            base: Note(Notes::C, false),
            addition: Some(Attribute::_sus4)
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(1, 2), (2, 3), (3, 2), (4, 0)]),
        Some(Chord {
            base: Note(Notes::D, false),
            addition: None
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(6, 1), (5, 3), (4, 3), (3, 2), (2, 1), (1, 1)]),
        Some(Chord {
            base: Note(Notes::F, false),
            addition: None
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(5, 4), (4, 6), (3, 5), (2, 6), (1, 4)]),
        Some(Chord {
            base: Note(Notes::C, true),
            addition: Some(Attribute::_Maj7)
        })
    );

    assert_eq!(
        guitar.find_chord(vec![(5, 1), (4, 3), (3, 1), (2, 2), (1, 1)]),
        Some(Chord {
            base: Note(Notes::A, true),
            addition: Some(Attribute::_m7)
        })
    )
}
