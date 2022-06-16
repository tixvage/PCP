use crate::lexer::Location;

#[derive(Debug)]
pub enum PcpError {
    Lexer(String, Location),
    Parser(String, Location),
}
