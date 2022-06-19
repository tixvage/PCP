use crate::{
    error::PcpError,
    lexer::{Lexer, Location},
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

fn str_as_type(name: &str, loc: Location) -> Result<Type, PcpError> {
    return match name {
        "string" => Ok(Type::String),
        "void" => Ok(Type::Void),
        "i32" => Ok(Type::Integer),
        _ => {
            return Err(PcpError::Parser(format!("wrong return type {name}"), loc));
        }
    };
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

    fn parse_expression(&mut self) -> Result<Expression, PcpError> {
        todo!("parse_expression");
    }

    fn parse_vardecl(&mut self, name: String) -> Result<VarDecl, PcpError> {
        if let TokenType::Identifier(type_) = &self.current_token.type_ {
            let type_ = str_as_type(type_, self.current_token.loc).unwrap();
            self.next();
            if self.current_token.type_ != TokenType::Equal {
                return Err(PcpError::Parser(
                    "expected '=' after type".to_string(),
                    self.current_token.loc,
                ));
            }
            self.next();
            Ok(VarDecl { type_, name })
        } else {
            Err(PcpError::Parser(
                "expected type after ':'".to_string(),
                self.current_token.loc,
            ))
        }
    }

    fn parse_statement(&mut self) -> Result<Statement, PcpError> {
        match &self.current_token.type_ {
            TokenType::Identifier(id) if id == "return" => {
                self.next();
                let expr = self.parse_expression().unwrap();
                return Ok(Statement::Return(expr));
            }
            TokenType::Identifier(id) => {
                let id = id.clone();
                self.next();

                if let TokenType::Colon = self.current_token.type_ {
                    self.next();
                    let var_decl = self.parse_vardecl(id).unwrap();
                    let expr = self.parse_expression();
                }
            }
            _ => {}
        }
        todo!("parse_statement")
    }

    fn parse_block(&mut self) -> Result<Block, PcpError> {
        let mut block = Block { stmts: Vec::new() };
        self.next();
        while self.current_token.type_ != TokenType::EOF {
            match &self.current_token.type_ {
                //TODO: implement child blocks
                TokenType::RCurly => {
                    self.next();
                    return Ok(block);
                }
                TokenType::Semicolon => {
                    self.next();
                }
                _ => {
                    let statement = self.parse_statement();
                    block.stmts.push(statement.unwrap());
                    self.next();
                }
            }
        }

        Err(PcpError::Parser(
            "expected } for closing block".to_string(),
            self.current_token.loc,
        ))
    }

    fn parse_function(&mut self, name: String) -> Result<Function, PcpError> {
        self.next();
        if self.current_token.type_ != TokenType::LParen {
            return Err(PcpError::Parser(
                "expected ( after fn".to_string(),
                self.current_token.loc,
            ));
        }

        let mut args = Vec::new();
        self.next();
        while self.current_token.type_ != TokenType::EOF {
            match &self.current_token.type_ {
                TokenType::RParen => {
                    self.next();
                    break;
                }
                //TODO: proper comma
                TokenType::Comma => self.next(),
                TokenType::Identifier(name) => {
                    let arg_name = name.clone();
                    self.next();

                    if self.current_token.type_ != TokenType::Colon {
                        return Err(PcpError::Parser(
                            "expected ':'".to_string(),
                            self.current_token.loc,
                        ));
                    }

                    self.next();

                    if let TokenType::Identifier(name) = &self.current_token.type_ {
                        let arg_type = str_as_type(name.as_str(), self.current_token.loc).unwrap();
                        args.push((arg_name, arg_type));
                        self.next();
                    } else {
                        return Err(PcpError::Parser(
                            "expected type of argument but found something else".to_string(),
                            self.current_token.loc,
                        ));
                    }
                }
                _ => {
                    return Err(PcpError::Parser(
                        "expected type or name of args in function definition".to_string(),
                        self.current_token.loc,
                    ))
                }
            }
        }

        if self.current_token.type_ != TokenType::Arrow {
            return Err(PcpError::Parser(
                "expected '->' after )".to_string(),
                self.current_token.loc,
            ));
        }

        self.next();

        if let TokenType::Identifier(id) = &self.current_token.type_ {
            let return_type = str_as_type(id.as_str(), self.current_token.loc).unwrap();
            self.next();
            let block = self.parse_block().unwrap();
            let function = Function {
                name,
                args,
                block,
                return_type,
            };
            Ok(function)
        } else {
            Err(PcpError::Parser(
                "expected return type after '->'".to_string(),
                self.current_token.loc,
            ))
        }
    }

    //TODO: not Funtion TopAssignment
    fn parse_top_assignment(&mut self, name: String) -> Result<Function, PcpError> {
        self.next();
        match &self.current_token {
            Token {
                type_: TokenType::Identifier(id),
                loc: _,
            } if id == "fn" => {
                return self.parse_function(name);
            }
            _ => {}
        }
        println!("{:?}", self.current_token);
        todo!()
    }

    pub fn parse_file(&mut self) -> Result<File, PcpError> {
        let mut file = File { fns: Vec::new() };
        while self.current_token.type_ != TokenType::EOF {
            match &self.current_token {
                Token {
                    type_: TokenType::Identifier(name),
                    loc: _,
                } => {
                    let name = name.clone();
                    self.next();
                    if self.current_token.type_ == TokenType::ColonColon {
                        file.fns.push(self.parse_top_assignment(name).unwrap());
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
