#[derive(Clone, Copy)]
pub struct Location {
    row: usize,
    col: usize,
}

impl Default for Location {
    fn default() -> Self {
        Self { row: 1, col: 1 }
    }
}

pub struct Lexer {
    source: String,
    current_loc: Location,
    c: u8,
    i: usize,
}

impl Lexer {
    pub fn new(source: String) -> Self {
        Self {
            c: source.as_bytes()[0],
            source,
            current_loc: Default::default(),
            i: 0,
        }
    }

    fn advance(&mut self) {}
}
