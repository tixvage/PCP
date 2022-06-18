use crate::{
    error::PcpError,
    token::{Token, TokenType},
};

#[derive(Debug, Clone, Copy)]
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

    fn advance(&mut self) {
        if self.i < self.source.len() && self.c != b'\0' {
            self.i += 1;

            if self.i == self.source.len() {
                self.c = b'\0';
            } else {
                self.c = self.source.as_bytes()[self.i];
            }

            if self.c == b'\n' {
                self.current_loc.row += 1;
                self.current_loc.col = 0;
            } else {
                self.current_loc.col += 1;
            }
        }
    }

    fn peek(&self, offset: usize) -> u8 {
        self.source.as_bytes()[self.i + offset]
    }

    fn skip_ws(&mut self) {
        while self.c.is_ascii_whitespace() {
            self.advance();
        }
    }

    fn lex_id(&mut self) -> Result<Token, PcpError> {
        let mut value = String::from("");
        while self.c.is_ascii_alphanumeric() || self.c == b'_' {
            value.push(self.c as char);
            self.advance();
        }

        Ok(Token::new(TokenType::Identifier(value), self.current_loc))
    }

    fn lex_num(&mut self) -> Result<Token, PcpError> {
        let mut value = String::from("");
        while self.c.is_ascii_hexdigit() {
            value.push(self.c as char);
            self.advance();
        }

        return match value.parse::<i32>() {
            Ok(num) => Ok(Token::new(TokenType::Number(num), self.current_loc)),
            Err(err) => Err(PcpError::Lexer(err.to_string(), self.current_loc)),
        };
    }

    fn advance_with(&mut self, token: Token) -> Token {
        self.advance();
        token
    }

    fn advance_two(&mut self, type_: TokenType) -> Token {
        let token = self.advance_with(Token::new(type_, self.current_loc));
        self.advance_with(token)
    }

    fn advance_current(&mut self, type_: TokenType) -> Token {
        let token = Token::new(type_, self.current_loc);
        self.advance();

        token
    }

    fn next(&self, c: u8) -> bool {
        self.peek(1) == c
    }

    pub fn get_next_token(&mut self) -> Result<Token, PcpError> {
        while self.c != b'\0' {
            self.skip_ws();
            if self.c.is_ascii_alphabetic() || self.c == b'_' {
                return self.lex_id();
            } else if self.c.is_ascii_digit() {
                return self.lex_num();
            }
            //TODO: implement lex_str()

            return match self.c {
                b';' => Ok(self.advance_current(TokenType::Semicolon)),
                b'#' => Ok(self.advance_current(TokenType::Hash)),
                b'(' => Ok(self.advance_current(TokenType::LParen)),
                b')' => Ok(self.advance_current(TokenType::RParen)),
                b'}' => Ok(self.advance_current(TokenType::RCurly)),
                b'{' => Ok(self.advance_current(TokenType::LCurly)),
                b'-' => {
                    if self.next(b'>') {
                        return Ok(self.advance_two(TokenType::Arrow));
                    }
                    Ok(self.advance_current(TokenType::Minus))
                }
                b':' => {
                    if self.next(b'=') {
                        return Ok(self.advance_two(TokenType::ColonEqual));
                    } else if self.next(b':') {
                        return Ok(self.advance_two(TokenType::ColonColon));
                    }

                    Ok(self.advance_current(TokenType::Colon))
                }
                b'=' => Ok(self.advance_current(TokenType::Equal)),
                b'.' => Ok(self.advance_current(TokenType::Dot)),
                b'+' => Ok(self.advance_current(TokenType::Plus)),
                b',' => Ok(self.advance_current(TokenType::Comma)),
                b'\0' => break,

                _ => Err(PcpError::Lexer(
                    format!("Unexpected character '{}'", self.c as char),
                    self.current_loc,
                )),
            };
        }

        Ok(Token::new(TokenType::EOF, Location::default()))
    }
}
