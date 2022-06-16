use crate::{lexer::Lexer, token::Token};

pub struct Parser {
    lexer: Lexer,
    current_token: Token,
}

impl Parser {
    pub fn new(mut lexer: Lexer) -> Self {
        //TODO: empty file?
        Self {
            current_token: lexer.get_next_token().unwrap(),
            lexer,
        }
    }
}
