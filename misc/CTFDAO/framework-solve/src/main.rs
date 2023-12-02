use anchor_lang::{InstructionData, ToAccountMetas};
use solana_program::pubkey::Pubkey;
use std::net::TcpStream;
use std::{error::Error, fs, io::prelude::*, io::BufReader, str::FromStr};
use solana_program::system_program;


fn get_line<R: Read>(reader: &mut BufReader<R>) -> Result<String, Box<dyn Error>> {
    let mut line = String::new();
    reader.read_line(&mut line)?;
    let ret = line
        .split(':')
        .nth(1)
        .ok_or("invalid input")?
        .trim()
        .to_string();
    Ok(ret)
}


fn main() -> Result<(), Box<dyn Error>> {
    let mut stream = TcpStream::connect("30.222.0.2:49731")?;
    let mut reader = BufReader::new(stream.try_clone().unwrap());

    let mut line = String::new();

    let so_data = fs::read("./solve/target/deploy/solve.so")?;

    reader.read_line(&mut line)?;
    writeln!(stream, "{}", solve::ID)?;
    reader.read_line(&mut line)?;
    writeln!(stream, "{}", so_data.len())?;
    stream.write_all(&so_data)?;

    let chall_id = chall::ID;

    let admin    = Pubkey::from_str(&get_line(&mut reader)?)?;
    let user     = Pubkey::from_str(&get_line(&mut reader)?)?;
    let dao      = Pubkey::from_str(&get_line(&mut reader)?)?;
    let vault    = Pubkey::from_str(&get_line(&mut reader)?)?;
    let proposal = Pubkey::from_str(&get_line(&mut reader)?)?;

    println!("");
    println!("admin      : {}", admin);
    println!("user       : {}", user);
    println!("dao        : {}", dao);
    println!("vault      : {}", vault);
    println!("proposal   : {}", proposal);
    println!("");

    
    let ix = solve::instruction::Initialize {};
    let data = ix.data();
    let ix_accounts = solve::accounts::Initialize {
        admin,
        user,
        dao,
        vault,
        proposal,
        chall: chall_id,
        system_program: system_program::id(),
    };

    let metas = ix_accounts.to_account_metas(None);

    // if you don't know what this is doing, look at server code and also sol-ctf-framework read_instruction:
    // https://github.com/otter-sec/sol-ctf-framework/blob/rewrite-v2/src/lib.rs#L237
    reader.read_line(&mut line)?;
    writeln!(stream, "{}", metas.len())?;
    for meta in metas {
        let mut meta_str = String::new();
        meta_str.push('m');
        if meta.is_writable {
            meta_str.push('w');
        }
        if meta.is_signer {
            meta_str.push('s');
        }
        meta_str.push(' ');
        meta_str.push_str(&meta.pubkey.to_string());
        writeln!(stream, "{}", meta_str)?;
        stream.flush()?;
    }

    reader.read_line(&mut line)?;
    writeln!(stream, "{}", data.len())?;
    stream.write_all(&data)?;

    stream.flush()?;

    line.clear();
    while reader.read_line(&mut line)? != 0 {
        print!("{}", line);
        line.clear();
    }

    Ok(())
}
