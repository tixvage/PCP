# PCP
**THIS LANGUAGE IS A WORK IN PROGRESS!**

PCP is a simple language inspired by [Rust](https://www.rust-lang.org/), [Zig](https://ziglang.org/) and [Jai](https://github.com/BSVino/JaiPrimer/blob/master/JaiPrimer.md), currently written in [C](https://en.wikipedia.org/wiki/C_(programming_language)) and will be rewritten in [Rust](https://www.rust-lang.org/).

## Examples

planned simple 'Hello, World':


```pcp
#import "io" as io;

fn main() -> void {
    say_hello();
}

fn say_hello() -> void {
    io.println("%s", hello);
}
```
