use crate::{
    error::PcpError,
    lexer::Lexer,
    token::{Token, TokenType},
};

pub struct FunctionCall {
    pub name: String,
    pub args: Vec<(String, Expression)>,
}

pub struct VarDecl {
    pub name: String,
    pub type_: Type,
}

pub struct Function {
    pub name: String,
    pub args: Vec<(String, Type)>,
    pub block: Block,
    pub return_type: Type,
}

pub struct Block {
    pub stmts: Vec<Statement>,
}

pub struct File {
    pub fns: Vec<Function>,
}

pub enum Type {
    String,
    Void,
    Integer,
}

pub enum Expression {
    Boolean(bool),
    Call(FunctionCall),
    Int(i32),
    QuotedString(String),
    BinaryOp(Box<Expression>, Box<Expression>, Box<Expression>),
    Var(String),
    Operator(Operator),
}

pub enum Operator {
    Add,
    Subtract,
    ReAssign,
    ConstAssign,
    Assign,
}

pub enum Statement {
    Invalid,
    Expression(Expression),
    VarDecl(VarDecl, Expression),
    Return(Expression),
}

pub enum TopAssignment {
    Function(Function),
}

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

    fn next(&mut self) {
        self.current_token = self.lexer.get_next_token().unwrap();
    }

    //TODO: not Funtion TopAssignment
    fn parse_top_assignment(&mut self) -> Result<Function, PcpError> {
        self.next();
        todo!()
    }

    pub fn parse_file(&mut self) -> Result<File, PcpError> {
        let mut file = File { fns: Vec::new() };
        while self.current_token.type_ != TokenType::EOF {
            match &self.current_token {
                Token {
                    type_: TokenType::Identifier(_),
                    loc: _,
                } => {
                    self.next();
                    if self.current_token.type_ == TokenType::ColonColon {
                        file.fns.push(self.parse_top_assignment().unwrap());
                    } else {
                        return Err(PcpError::Parser(
                            String::from("Language just supports const assignments at top for now"),
                            self.current_token.loc,
                        ));
                    }
                }
                _ => {}
            }
        }
        Ok(file)
    }
}
