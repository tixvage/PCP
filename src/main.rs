use pcp::{
    lexer::Lexer,
    token::{Token, TokenType},
};

const HELP: &str = "\
PCP compiler

USAGE:
    pcp [OPTIONS]

FLAGS:
    -h, --help          prints help information

OPTIONS:
    --file PATH         compile file
";

fn parse_args() -> Result<String, String> {
    let mut pargs = pico_args::Arguments::from_env();
    if pargs.contains(["-h", "--help"]) {
        print!("{HELP}");
        std::process::exit(0);
    }

    return match pargs.value_from_str("--file") {
        Ok(file_path) => {
            println!("{file_path}");
            Ok(file_path)
        }
        Err(err) => match err {
            pico_args::Error::MissingOption(_) | pico_args::Error::OptionWithoutAValue(_) => {
                Err(String::from("please specify file with --file PATH"))
            }
            _ => todo!(),
        },
    };
}

fn compile(file_path: String) {
    let contents = std::fs::read_to_string(file_path).unwrap();
    let mut lexer = Lexer::new(contents);
    let mut token = Token::new(TokenType::Invalid, Default::default());

    while token.type_ != TokenType::EOF {
        token = lexer.get_next_token().unwrap();
        println!("{:?}", token);
    }
}

fn main() {
    match parse_args() {
        Ok(file_path) => compile(file_path),
        Err(error) => {
            println!("{error}");
            std::process::exit(1);
        }
    }
}
