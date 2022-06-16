use crate::lexer::Location;

#[derive(PartialEq, Eq, Debug, Clone)]
pub enum TokenType {
    Invalid,
    QuotedString(String),
    Identifier(String),
    Number(i32),
    Semicolon,
    Hash,
    LParen,
    RParen,
    LCurly,
    RCurly,
    Arrow,
    Dot,
    Plus,
    Equal,
    Minus,
    EOF,
}

#[derive(Debug, Clone)]
pub struct Token {
    pub type_: TokenType,
    pub loc: Location,
}

impl Token {
    pub fn new(type_: TokenType, loc: Location) -> Self {
        Self { type_, loc }
    }
}
